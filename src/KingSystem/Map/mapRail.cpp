#include "KingSystem/Map/mapRail.h"
#include <heap/seadHeapMgr.h>
#include "KingSystem/Utils/SafeDelete.h"

namespace ksys::map {

RailPoint::~RailPoint() = default;

const MubinIter& RailPoint::getIter() const {
    return mIter;
}

const sead::Vector3f& RailPoint::getRotate() const {
    return mSRT.rotate;
}

const sead::Vector3f& RailPoint::getTranslate() const {
    return mSRT.translate;
}

f32 RailPoint::getPrevDistance() const {
    return mPrevDistance;
}

f32 RailPoint::getNextDistance() const {
    return mNextDistance;
}

bool RailPoint::parse(MubinIter* iter, sead::Heap* heap) {
    iter->getScale(&mSRT.scale);
    iter->getRotate(&mSRT.rotate);

    bool success = true;
    success &= iter->tryGetFloatArrayByKey(mSRT.translate.e.data(), "Translate");
    success &= iter->tryGetParamFloatByKey(&mPrevDistance, "PrevDistance");
    success &= iter->tryGetParamFloatByKey(&mNextDistance, "NextDistance");
    iter->tryGetControlPoints(&mCtrlPoints);

    return success;
}

Rail::Rail() = default;

Rail::~Rail() {
    for (s32 i = 0; i < mRailPoints.size(); ++i) {
        if (mRailPoints[i])
            delete mRailPoints[i];
    }
    mRailPoints.freeBuffer();
}

void Rail::init(MubinIter* iter, sead::Heap* heap) {
    parse(iter);

    MubinIter result;
    if (iter->tryGetParamIterByKey(&result, "RailPoints")) {
        s32 size = result.getSize();
        mRailPoints.allocBufferAssert(size, heap);
        for (int i = 0; i < size; ++i) {
            MubinIter node_iter;
            if (result.tryGetIterByIndex(&node_iter, i)) {
                RailPoint* point = allocPointIdx(i, heap);
                point->mIter = node_iter;
                point->parse(&node_iter, heap);
            }
        }
    }

    x_38();
}

RailPoint* Rail::allocPointIdx(s32 idx, sead::Heap* heap) {
    mRailPoints[idx] = allocPoint(heap);
    return mRailPoints[idx];
}

s32 Rail::getNumPoints() const {
    return mRailPoints.size();
}

const RailPoint* Rail::getPoint(s32 idx) const {
    if (idx < 0 || idx >= mRailPoints.size())
        return nullptr;
    return mRailPoints[idx];
}

RailPoint* Rail::getPoint(s32 idx) {
    if (idx < 0 || idx >= mRailPoints.size())
        return nullptr;
    return mRailPoints[idx];
}

const SRT& Rail::getPointSRT(s32 idx) const {
    return mRailPoints[idx]->mSRT;
}

const sead::Vector3f& Rail::getPointRotate(s32 idx) const {
    return mRailPoints[idx]->mSRT.rotate;
}

const sead::Vector3f& Rail::getPointTranslate(s32 idx) const {
    return mRailPoints[idx]->mSRT.translate;
}

const sead::Vector3f& Rail::getControlPoint(s32 rail_idx, s32 pt_idx) const {
    return mRailPoints[rail_idx]->mCtrlPoints[pt_idx];
}

sead::Vector3f Rail::calcTranslate(float progress) const {
    sead::Vector3f pos;
    calcTranslateRotate(&pos, nullptr, progress);
    return pos;
}

void Rail::calcTranslate(sead::Vector3f* pos_out, float progress) const {
    calcTranslateRotate(pos_out, nullptr, progress);
}

bool Rail::isClosed() const {
    return mFlags.isOn(Flag::Closed);
}

bool Rail::isBezier() const {
    return mFlags.isOn(Flag::Bezier);
}

const u32& Rail::getHashId() const {
    return mHashId;
}

void Rail::calcTranslateRotate(sead::Vector3f* pos_out, sead::Vector3f* rot_out,
                               float progress) const {
    u32 num_points = mRailPoints.size();
    if (num_points == 1) {
        auto* p = mRailPoints[0];
        *pos_out = p->getTranslate();
        if (rot_out) {
            *rot_out = sead::Vector3f::zero;
        }
        return;
    }

    if (mFlags.isOn(Flag::Closed)) {
        float f = progress / static_cast<float>(num_points);
        progress -= static_cast<float>(num_points) * std::floor(f);
    }

    u32 idx1 = static_cast<u32>(std::floor(progress));
    if (!mFlags.isOn(Flag::Closed)) {
        if (idx1 == num_points - 1)
            idx1--;
    }

    u32 idx2 = (idx1 + 1) % num_points;
    float t = progress - static_cast<float>(idx1);

    auto* p1 = mRailPoints[idx1];
    auto* p2 = mRailPoints[idx2];

    if (!mFlags.isOn(Flag::Bezier)) {
        *pos_out = p1->getTranslate() + (p2->getTranslate() - p1->getTranslate()) * t;
        if (rot_out) {
            sead::Vector3f diff = p2->getTranslate() - p1->getTranslate();
            float len = diff.length();
            if (len > 0.0f) {
                *rot_out = diff * (1.0f / len);
            }
        }
        return;
    }

    sead::Vector3f p0_pos = p1->getTranslate();
    sead::Vector3f p3_pos = p2->getTranslate();
    sead::Vector3f ctrl1 = getControlPoint(idx1, 1);
    sead::Vector3f ctrl2 = getControlPoint(idx2, 0);

    float omt = 1.0f - t;
    float omt2 = omt * omt;
    float omt3 = omt2 * omt;

    float t2 = t * t;
    float t3 = t2 * t;

    *pos_out = p0_pos * omt3 + (ctrl1 + p0_pos) * (3.0f * t * omt2) +
               (ctrl2 + p3_pos) * (3.0f * t2 * omt) + p3_pos * t3;

    if (rot_out) {
        sead::Vector3f deriv = (ctrl1 + p0_pos - p0_pos) * (-3.0f * omt2) +
                               (ctrl2 + p3_pos - ctrl1 - p0_pos) * (6.0f * t * omt) +
                               (p3_pos - ctrl2 - p3_pos) * (3.0f * t2);
        float len = deriv.length();
        if (len > 0.0f) {
            *rot_out = deriv * (1.0f / len);
        }
    }
}

const char* Rail::getUniqueName() const {
    return mUniqueName;
}

RailPoint* Rail::allocPoint(sead::Heap* heap) {
    return new (heap) RailPoint;
}

bool Rail::parse(MubinIter* iter) {
    bool success = true;

    success &= iter->tryGetParamUIntByKey(&mHashId, "HashId");
    success &= iter->tryGetParamStringByKey(&mUniqueName, "UniqueName");

    bool closed = false;
    if (!iter->tryGetParamBoolByKey(&closed, "Closed")) {
        success = false;
    } else if (closed) {
        mFlags.set(Flag::Closed);
    } else {
        mFlags.reset(Flag::Closed);
    }

    const char* type = nullptr;
    if (!iter->tryGetParamStringByKey(&type, "RailType")) {
        success = false;
    } else {
        mFlags.change(Flag::Bezier, sead::SafeString(type) == "Bezier");
    }

    return success;
}

bool RailGuidePoint::parse(MubinIter* iter, sead::Heap* heap) {
    bool success = true;
    success &= RailPoint::parse(iter, heap);
    success &= iter->tryGetParamFloatByKey(&mWaitFrame, "WaitFrame");
    return success;
}

RailGuide::RailGuide() = default;

RailGuide::~RailGuide() = default;

RailPoint* RailGuide::allocPoint(sead::Heap* heap) {
    return new (heap) RailGuidePoint;
}

bool RailRemainGuidePoint::parse(MubinIter* iter, sead::Heap* heap) {
    bool success = true;
    success &= RailGuidePoint::parse(iter, heap);
    success &= iter->tryGetParamFloatByKey(&mMoveSpeed, "MoveSpeed");
    return success;
}

RailRemainGuide::RailRemainGuide() = default;

RailRemainGuide::~RailRemainGuide() = default;

RailPoint* RailRemainGuide::allocPoint(sead::Heap* heap) {
    return new (heap) RailRemainGuidePoint;
}

RailConnectablePoint::RailConnectablePoint() = default;

RailConnectablePoint::~RailConnectablePoint() {
    sead::HeapMgr::instance()->getCurrentHeap();

    if (mJunctionPoint)
        util::safeDelete(mJunctionPoint);
}

Rail* RailConnectablePoint::getJunctionRail() const {
    return mJunctionRail;
}

RailPoint* RailConnectablePoint::getJunctionPoint() const {
    return mJunctionPoint;
}

bool RailConnectablePoint::parse(MubinIter* iter, sead::Heap* heap) {
    return RailPoint::parse(iter, heap);
}

RailRoutePoint::RailRoutePoint() = default;

RailRoutePoint::~RailRoutePoint() = default;

const char* RailRoutePoint::getCheckPointName() const {
    return mCheckPointName;
}

bool RailRoutePoint::parse(MubinIter* iter, sead::Heap* heap) {
    bool success = true;
    success &= RailConnectablePoint::parse(iter, heap);
    iter->tryGetParamStringByKey(&mEntryPointName, "EntryPointName");
    iter->tryGetParamStringByKey(&mCheckPointName, "CheckPointName");
    return success;
}

RailRoute::RailRoute() {
    mFlags.set(Flag::AutoPlacementEnabled);
    mFlags.set(Flag::EnableHorseTrace);
    mFlags.set(Flag::Walkable);
}

RailRoute::~RailRoute() = default;

bool RailRoute::isRenderEnabled() const {
    return mFlags.isOn(Flag::RenderEnabled);
}

bool RailRoute::isAutoPlacementEnabled() const {
    return mFlags.isOn(Flag::AutoPlacementEnabled);
}

bool RailRoute::isWalkable() const {
    return mFlags.isOn(Flag::Walkable);
}

bool RailRoute::isHorseTraceEnabled() const {
    return mFlags.isOn(Flag::EnableHorseTrace);
}

const char* RailRoute::getRouteId() const {
    return mRouteId;
}

const char* RailRoute::getCheckPointName(s32 idx) const {
    return static_cast<RailRoutePoint*>(mRailPoints[idx])->mCheckPointName;
}

bool RailRoute::parse(MubinIter* iter) {
    bool success = Rail::parse(iter);

    bool result = false;
    if (iter->tryGetParamBoolByKey(&result, "RenderEnabled")) {
        mFlags.change(Flag::RenderEnabled, result);
    }

    if (iter->tryGetParamBoolByKey(&result, "AutoPlacementEnabled")) {
        mFlags.change(Flag::AutoPlacementEnabled, result);
    } else {
        success = false;
    }

    if (iter->tryGetParamBoolByKey(&result, "IsWalkable")) {
        mFlags.change(Flag::Walkable, result);
    } else {
        success = false;
    }

    if (iter->tryGetParamBoolByKey(&result, "IsEnableHorseTrace")) {
        mFlags.change(Flag::EnableHorseTrace, result);
    }

    bool str_ok = iter->tryGetParamStringByKey(&mRouteId, "RouteId");
    return success && str_ok;
}

}  // namespace ksys::map
