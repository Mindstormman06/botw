#pragma once

#include <heap/seadDisposer.h>
#include <math/seadVector.h>
#include <prim/seadDelegate.h>
#include <prim/seadSafeString.h>
#include <thread/seadCriticalSection.h>

#include "KingSystem/Utils/Types.h"

namespace ksys::act {
class Actor;
}  // namespace ksys::act

namespace ksys::map {

class AutoPlacementMgr {
    SEAD_SINGLETON_DISPOSER(AutoPlacementMgr)
public:
    AutoPlacementMgr();
    virtual ~AutoPlacementMgr();

    bool sub_7100659E40(act::Actor* actor, const sead::SafeString& actor_name, int count,
                        bool is_box);
    void sub_7100659F94(act::Actor* actor);

    // 0x0000007100654e44
    bool threadFn();
    // 0x0000007100656030
    bool auto9();
    // 0x0000007100656d24
    bool auto2(const sead::SafeString& name, const sead::Vector3f& pos);
    // 0x0000007100659188
    bool auto0(const sead::Vector3f& pos, u32 placement_type);
    // 0x0000007100659350
    bool auto11(const sead::Vector3f& pos);

    struct Entry {
        act::Actor* actor{};
        sead::FixedSafeString<64> name;
        sead::Vector3f vec1{};
        sead::Vector3f vec2{};
        sead::Vector3f vec3{};
        sead::Vector3f vec4{};
        bool is_box{};
        u8 _91[0x8b60 - 0x91]{};
        s16 count{};
        bool _8b62{};
        u8 _8b63[5]{};
    };
    KSYS_CHECK_SIZE_NX150(Entry, 0x8b68);

    sead::DelegateR<AutoPlacementMgr, bool> mDelegate;
    u8 _48[0x5b0d8 - 0x48];
    sead::CriticalSection mCS_5b0d8;
    Entry mEntries[32];
    u8 _171e18[0x171e46 - 0x171e18];
    bool _171e46{};
    u8 _171e47{};
    s32 _171e48{};
};
// KSYS_CHECK_SIZE_NX150(AutoPlacementMgr, 0x189E38);

}  // namespace ksys::map
