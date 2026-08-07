#include "KingSystem/ActorSystem/actLifeRecoveryInfo.h"
#include <algorithm>

namespace ksys::act {

bool LifeRecoverInfo::onApplyDamage(s32& damage) {
    if (mExtraHp1 != 0) {
        s32 remaining = std::max(0, static_cast<s32>(mExtraHp1 - damage));
        damage = (damage - mExtraHp1) + remaining;
        f32 extra_hp = static_cast<f32>(mExtraHp2);
        if (static_cast<f32>(remaining) <= extra_hp) {
            extra_hp = static_cast<f32>(remaining);
        }
        mExtraHp1 = static_cast<u32>(extra_hp);
        return remaining != static_cast<s32>(mExtraHp1);
    }
    return damage > 0;
}

}  // namespace ksys::act
