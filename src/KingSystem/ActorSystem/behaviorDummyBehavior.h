#pragma once

#include "KingSystem/ActorSystem/actAiBehavior.h"

namespace ksys::act::ai {

class DummyBehavior : public Behavior {
    SEAD_RTTI_OVERRIDE(DummyBehavior, Behavior)
public:
    explicit DummyBehavior(const InitArg& arg);
    bool init_(sead::Heap*) override { return true; }
};

}  // namespace ksys::act::ai
