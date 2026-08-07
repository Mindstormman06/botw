#pragma once

#include <container/seadPtrArray.h>
#include <heap/seadDisposer.h>
#include <heap/seadExpHeap.h>
#include <math/seadVector.h>
#include <thread/seadCriticalSection.h>
#include "KingSystem/Utils/Types.h"

namespace agl {
class DrawContext;
}

namespace ksys {

namespace chm {
class Object;
}

class Chemical {
    SEAD_SINGLETON_DISPOSER(Chemical)
    Chemical();
    virtual ~Chemical();

public:
    void createChmresAndHeap(sead::Heap* heap);
    void drawScene(agl::DrawContext* context);

private:
    sead::CriticalSection mCS1;
    sead::PtrArray<chm::Object> mObjects;
    sead::ExpHeap* mHeap = nullptr;
    int mCount = 1;
    u8 mFlag1 = 0;
    u8 mFlag2 = 0;
    sead::CriticalSection mCS2;
};
KSYS_CHECK_SIZE_NX150(Chemical, 0xc8);

void combineForces(sead::Vector3f* out, const sead::Vector3f& other);
float normalizeScientificExponent(float val);

}  // namespace ksys
