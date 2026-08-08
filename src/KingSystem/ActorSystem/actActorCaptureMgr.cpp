#include "KingSystem/ActorSystem/actActorCaptureMgr.h"
#include "KingSystem/ActorSystem/actActor.h"
#include "KingSystem/ActorSystem/actActorParam.h"
#include "KingSystem/ActorSystem/actBaseProcMgr.h"
#include "KingSystem/ActorSystem/actInfoData.h"
#include "KingSystem/Resource/Actor/resResourceModelList.h"
#include "KingSystem/Resource/resEntryFactory.h"
#include "KingSystem/Resource/resLoadRequest.h"
#include "KingSystem/Resource/resResourceMgrTask.h"
#include "KingSystem/Resource/resSystem.h"
#include "KingSystem/Utils/SafeDelete.h"

namespace ksys::act {

ActorCaptureMgr::ActorCaptureMgr() {
    mRes.reset();
    mTimer.reset(30.0);
}

ActorCaptureMgr::~ActorCaptureMgr() {
    mActor = nullptr;
    mCamera = nullptr;
    res::unregisterEntryFactory(mFactory);
    util::safeDelete(mFactory);
}

void ActorCaptureMgr::init(const InitArg& arg) {
    mFactory = new (arg.heap) res::EntryFactory<res::ActorCapture>(1.0, 0x1000);
    res::registerEntryFactory(mFactory, "bactcapt");

    res::LoadRequest req;
    req.mRequester = "ActorCaptureMgr";
    req._26 = false;
    mDummyFile.load("Actor/ActorCapture/Dummy.bactcapt", &req);
}

void ActorCaptureMgr::setCamera(Camera* camera) {
    if (camera)
        mCamera = camera;
}

void ActorCaptureMgr::loadCaptureParam() {
    if (!mActor || !mActor->getParam())
        return;

    const char* name = getCapturedActorName();
    if (!name)
        return;

    res::ActorCapture* param = nullptr;

    {
        res::LoadRequest req;
        req.mRequester = "ActorCaptureMgr";
        sead::FormatFixedSafeString<128> path("Actor/ActorCapture/%s.bactcapt", name);
        res::ResourceMgrTask::instance()->controlField9c0d88(false);
        auto* file = sead::DynamicCast<res::ActorCapture>(mCaptureParamFile.load(path, &req));
        res::ResourceMgrTask::instance()->controlField9c0d88(true);
        if (file)
            param = file;
        if (!param)
            param = sead::DynamicCast<res::ActorCapture>(mDummyFile.getResource());
    }

    mRes.mCameraInfoObj.copy(param->mCameraInfoObj);
    mRes.mActorInfoObj.copy(param->mActorInfoObj);
    mRes.mLightInfoObj.copy(param->mLightInfoObj);
}

const char* ActorCaptureMgr::getCapturedActorName() const {
    Actor* actor = mActor;
    if (InfoData::instance()->hasTag(mActor->getName().cstr(), 0xf2bc0087)) {
        if (auto* mgr = BaseProcMgr::instance()) {
            BaseProcMgr::ProcFilters filters;
            filters.set(BaseProcMgr::ProcFilter::Initializing);
            filters.set(BaseProcMgr::ProcFilter::SkipAccessCheck);
            auto* proc = mgr->getProc(mStr2, filters);
            if (auto* target = sead::DynamicCast<Actor>(proc))
                actor = target;
        }
    }
    if (!actor || !actor->getParam())
        return nullptr;
    auto* model_list = actor->getParam()->getRes().mModelList;
    if (!model_list)
        return nullptr;
    res::ModelList::ModelDataInfo info;
    model_list->getModelDataInfo(&info);
    return info.folder_name[0];
}

}  // namespace ksys::act
