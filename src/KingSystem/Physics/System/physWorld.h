#pragma once

#include <basis/seadTypes.h>
#include <thread/seadCriticalSection.h>
#include "KingSystem/Utils/Types.h"

class hkpWorld;

namespace ksys::phys {

class World {
public:
    hkpWorld* getHavokWorld() const { return mHavokWorld; }
    void lockCS(const char* description, int line, bool only_lock_if_needed);
    void unlockCS(const char* description, int line, bool only_lock_if_needed);

private:
    u8 _0[0x8];
    hkpWorld* mHavokWorld = nullptr;
    u8 _10[0x30 - 0x10];
    sead::CriticalSection mCS;
};
KSYS_CHECK_SIZE_NX150(World, 0x70);

}  // namespace ksys::phys
