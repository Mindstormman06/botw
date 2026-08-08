#include "KingSystem/Map/mapPlacementActors.h"
#include "KingSystem/Map/mapObject.h"
#include "KingSystem/Map/mapPlacementMgr.h"

namespace ksys::map {

u32 PlacementActors::getNumStaticObjs() const {
    return mGroups[0].size();
}

int PlacementActors::getNumObjs(int group_idx) const {
    int idx = group_idx;
    if (static_cast<u32>(group_idx) > 9)
        idx = 0;
    return mGroups[idx].size();
}

Object* PlacementActors::getStaticObj_0(int object_idx) {
    auto* objs = mGroups[0].getBufferPtr();
    if (static_cast<u32>(object_idx) >= mGroups[0].mSize)
        return objs;
    return &objs[object_idx];
}

Object* PlacementActors::getStaticObj_2(s32 object_idx) const {
    auto* objs = mGroups[0].getBufferPtr();
    if (static_cast<u32>(object_idx) >= mGroups[0].mSize)
        return objs;
    return &objs[object_idx];
}

void PlacementActors::x_9() {
    mMutex.lock();
    auto* mgr = PlacementMgr::instance();
    for (s32 i = 0; i < 128; ++i) {
        auto* obj = mQueuedObjs[i];
        if (obj) {
            if (!obj->shouldSkipSpawn()) {
                if (obj->getFlags0().isOn(Object::Flag0::_1))
                    mgr->disableObjStaticCompound_0(obj);
                mgr->auto4(obj, true);
            }
            mQueuedObjs[i] = nullptr;
        }
    }
    mMutex.unlock();
}

}  // namespace ksys::map
