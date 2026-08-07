#include "KingSystem/ActorSystem/actActorSystem.h"
#include "KingSystem/ActorSystem/actActorConstDataAccess.h"

namespace ksys::act {

SEAD_SINGLETON_DISPOSER_IMPL(ActorSystem)

bool ActorSystem::getPlayer(ActorConstDataAccess* accessor) {
    return accessor->acquire(nullptr);
}

bool ActorSystem::getAutoPlacementActorPos(const sead::SafeString& name, sead::Vector3f* pos) const {
    return false;
}

}  // namespace ksys::act
