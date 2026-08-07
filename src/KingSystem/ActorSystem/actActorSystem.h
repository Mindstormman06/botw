#pragma once

#include <heap/seadDisposer.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace ksys::act {

class ActorConstDataAccess;

// TODO: incomplete
class ActorSystem {
    SEAD_SINGLETON_DISPOSER(ActorSystem)
    ActorSystem();

public:
    enum class State : u8 {
        _0 = 0,
        _1 = 1,
        _2 = 2,
    };

    void onBaseProcMgrCalc();

    bool getPlayer(ActorConstDataAccess* accessor);

    bool getAutoPlacementActorPos(const sead::SafeString& name, sead::Vector3f* pos) const;

    sead::Heap* getEmergencyHeap() const { return mEmergencyHeap; }
    const sead::Vector3f& getPlayerPos() const { return mPlayerPos; }
    State getState() const { return mState; }

private:
    u8 temp_0x20[0x28 - 0x20];
    State mState{State::_0};
    u8 temp_0x29[0xd0 - 0x29];
    sead::Heap* mEmergencyHeap;
    sead::Vector3f mPlayerPos;
};

}  // namespace ksys::act
