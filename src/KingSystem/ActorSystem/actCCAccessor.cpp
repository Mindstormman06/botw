#include "KingSystem/ActorSystem/actCCAccessor.h"
#include "KingSystem/ActorSystem/actActor.h"
#include "KingSystem/Physics/System/physInstanceSet.h"

namespace ksys::act {

CCAccessor::CCAccessor() = default;
CCAccessor::~CCAccessor() = default;

void CCAccessor::resetRigidBodyMotion(Actor* actor) {
    if (actor->getInstanceSet()) {
        actor->getInstanceSet()->resetRigidBodyMotion();
    }
}

}  // namespace ksys::act
