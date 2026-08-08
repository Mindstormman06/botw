#include "KingSystem/ActorSystem/Profiles/actPlayerBase.h"
#include "KingSystem/ActorSystem/actInfoData.h"

namespace ksys::act {

void PlayerBase::switchEquipment(const sead::SafeString& slot, int frames, int x,
                                 const uintptr_t& y) {
    auto lock = sead::makeScopedLock(mCS);

    if (slot == "Armor_Default_Upper") {
        mArmorUpper = slot;
        _1168[4] = -1;
    } else if (slot == "Armor_Default_Lower") {
        mArmorLower = slot;
        _1168[5] = -1;
    } else if (slot == "Weapon_Default_Left") {
        mWeaponLeft = slot;
        _1168[1] = 0;
        _1184[1] = 1;
    } else if (slot == "Weapon_Default_Bow") {
        mWeaponBow = slot;
        _1168[2] = 0;
        _1184[2] = 1;
    } else if (slot == "Weapon_Default_Right") {
        mWeaponRight = slot;
        _1168[0] = 0;
        _1184[0] = 1;
    } else {
        const char* profile = nullptr;
        if (InfoData::instance()->getActorProfile(&profile, slot.cstr())) {
            sead::SafeString prof(profile);
            if (prof == "ArmorHead") {
                mArrow = slot;
                _1168[3] = x;
            } else if (prof == "ArmorUpper") {
                mArmorUpper = slot;
                _1168[4] = x;
            } else if (prof == "ArmorLower") {
                mArmorLower = slot;
                _1168[5] = x;
            } else if (prof == "WeaponShield") {
                mWeaponLeft = slot;
                _1168[1] = frames;
                _1184[1] = 1;
                _119c[1] = y;
            } else if (prof == "WeaponBow") {
                mWeaponBow = slot;
                _1168[2] = frames;
                _1184[2] = 1;
                _119c[2] = y;
            } else {
                mWeaponRight = slot;
                _1168[0] = frames;
                _1184[0] = 1;
                _119c[0] = y;
            }
        }
    }
}

}  // namespace ksys::act
