#include "KingSystem/Map/mapAutoPlacementMgr.h"
#include "KingSystem/ActorSystem/actActor.h"

namespace ksys::map {

SEAD_SINGLETON_DISPOSER_IMPL(AutoPlacementMgr)

AutoPlacementMgr::AutoPlacementMgr() {
    mDelegate.bind(this, &AutoPlacementMgr::threadFn);
}

AutoPlacementMgr::~AutoPlacementMgr() = default;

bool AutoPlacementMgr::auto9() {
    return _171e48 > 0;
}

bool AutoPlacementMgr::sub_7100659E40(act::Actor* actor, const sead::SafeString& actor_name,
                                      int count, bool is_box) {
    if (static_cast<u32>(count - 1) >= 0x7fff)
        return false;

    mCS_5b0d8.lock();
    Entry* free_entry = nullptr;
    for (s32 i = 0; i < 32; ++i) {
        if (mEntries[i].actor == actor) {
            mCS_5b0d8.unlock();
            return false;
        }
        if (mEntries[i].actor == nullptr && (!mEntries[i]._8b62 || free_entry == nullptr)) {
            free_entry = &mEntries[i];
        }
    }

    if (free_entry == nullptr) {
        mCS_5b0d8.unlock();
        return false;
    }

    free_entry->actor = actor;
    free_entry->vec1 = {actor->getMtx()(0, 0), actor->getMtx()(0, 1), actor->getMtx()(0, 2)};
    free_entry->vec2 = {actor->getMtx()(1, 0), actor->getMtx()(1, 1), actor->getMtx()(1, 2)};
    free_entry->vec3 = {actor->getMtx()(2, 0), actor->getMtx()(2, 1), actor->getMtx()(2, 2)};
    free_entry->vec4 = actor->getScale();
    free_entry->name = actor_name;
    free_entry->is_box = is_box;
    free_entry->count = static_cast<s16>(count);
    free_entry->_8b62 = true;
    _171e46 = true;

    mCS_5b0d8.unlock();
    return true;
}

void AutoPlacementMgr::sub_7100659F94(act::Actor* actor) {
    mCS_5b0d8.lock();
    for (s32 i = 0; i < 32; ++i) {
        if (mEntries[i].actor == actor) {
            mEntries[i].actor = nullptr;
            _171e46 = true;
            break;
        }
    }
    mCS_5b0d8.unlock();
}

}  // namespace ksys::map
