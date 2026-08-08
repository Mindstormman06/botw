#pragma once

#include <thread/seadCriticalSection.h>
#include "KingSystem/ActorSystem/actActor.h"

namespace ksys::act {

class PlayerBase : public Actor {
public:
    void switchEquipment(const sead::SafeString& slot, int frames, int x = -1,
                         const uintptr_t& y = {});

    void setExtraLife(s32 extra_life, f32 x);

private:
    u8 _840[0xeb8 - 0x840];
    sead::CriticalSection mCS;
    u8 _ef8[0xf00 - 0xef8];
    sead::FixedSafeString<64> mWeaponRight;  // 0xf00
    sead::FixedSafeString<64> mWeaponLeft;   // 0xf58
    sead::FixedSafeString<64> mWeaponBow;    // 0xfb0
    sead::FixedSafeString<64> mArrow;        // 0x1008
    sead::FixedSafeString<64> mArmorUpper;   // 0x1060
    sead::FixedSafeString<64> mArmorLower;   // 0x10b8
    sead::FixedSafeString<64> mArmorHead;    // 0x1110
    u32 _1168[6];
    u32 _1180;
    u8 _1184[12];
    u8 _1190[12];
    u64 _119c[3];
};

}  // namespace ksys::act
