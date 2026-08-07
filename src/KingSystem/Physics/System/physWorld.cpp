#include "KingSystem/Physics/System/physWorld.h"
#include <nn/os.h>

namespace ksys::phys {

void World::lockCS(const char* description, int line, bool only_lock_if_needed) {
    if (only_lock_if_needed) {
        // Condition check for lock
    }
    mCS.lock();
}

void World::unlockCS(const char* description, int line, bool only_lock_if_needed) {
    if (only_lock_if_needed) {
        // Condition check for unlock
    }
    mCS.unlock();
}

}  // namespace ksys::phys
