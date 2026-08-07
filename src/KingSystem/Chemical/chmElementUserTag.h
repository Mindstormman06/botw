#pragma once

#include "KingSystem/Physics/System/physUserTag.h"
#include "KingSystem/Utils/Types.h"

namespace ksys::chm {

class ElementUserTag : public phys::UserTag {
    SEAD_RTTI_OVERRIDE(ElementUserTag, phys::UserTag)
public:
    ElementUserTag() = default;
    ~ElementUserTag() override = default;
};
KSYS_CHECK_SIZE_NX150(ElementUserTag, 0x8);

}  // namespace ksys::chm
