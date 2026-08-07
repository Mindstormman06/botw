#include "KingSystem/Chemical/chmChemical.h"
#include <cmath>
#include "KingSystem/Chemical/chmSystemConfig.h"

namespace ksys {

SEAD_SINGLETON_DISPOSER_IMPL(Chemical)

Chemical::Chemical() = default;

Chemical::~Chemical() {
    mObjects.freeBuffer();
    chm::SystemConfig::deleteInstance();
}

void Chemical::createChmresAndHeap(sead::Heap* heap) {
    chm::SystemConfig::createInstance(heap);
    chm::SystemConfig::instance()->init(heap);
    mObjects.allocBuffer(8, heap, 8);
    sead::SafeString name = "chm::System";
    mHeap = sead::ExpHeap::create(0x300000, name, heap, 8, sead::Heap::cHeapDirection_Forward, true);
}

void combineForces(sead::Vector3f* out, const sead::Vector3f& other) {
    for (int i = 0; i < 3; ++i) {
        float a = out->e[i];
        float b = other.e[i];
        float sign_a = (a < 0.0f) ? -1.0f : 1.0f;
        float sign_b = (b < 0.0f) ? -1.0f : 1.0f;
        if (sign_a == sign_b) {
            float abs_a = (a <= 0.0f) ? -a : a;
            float abs_b = (b <= 0.0f) ? -b : b;
            if (abs_a <= abs_b)
                abs_a = abs_b;
            out->e[i] = sign_a * abs_a;
        } else {
            out->e[i] = a + b;
        }
    }
}

float normalizeScientificExponent(float val) {
    float y = std::log10(val);
    float offset = 24.0f;
    if (y <= 24.0f) {
        if (y <= 21.0f) {
            if (y <= 18.0f) {
                if (y <= 15.0f) {
                    if (y <= 12.0f) {
                        if (y <= 9.0f) {
                            if (y <= 6.0f) {
                                if (y <= 3.0f) {
                                    if (0.0f < y)
                                        goto calc;
                                    if (-3.0f < y) {
                                        y += 3.0f;
                                        goto calc;
                                    }
                                    if (-6.0f < y) {
                                        y += 6.0f;
                                        goto calc;
                                    }
                                    if (-9.0f < y) {
                                        y += 9.0f;
                                        goto calc;
                                    }
                                    if (-12.0f < y) {
                                        y += 12.0f;
                                        goto calc;
                                    }
                                    if (-15.0f < y) {
                                        y += 15.0f;
                                        goto calc;
                                    }
                                    if (-18.0f < y) {
                                        y += 18.0f;
                                        goto calc;
                                    }
                                    if (-21.0f < y) {
                                        y += 21.0f;
                                        goto calc;
                                    }
                                    if (y <= -24.0f)
                                        goto calc;
                                } else {
                                    offset = -3.0f;
                                }
                            } else {
                                offset = -6.0f;
                            }
                        } else {
                            offset = -9.0f;
                        }
                    } else {
                        offset = -12.0f;
                    }
                } else {
                    offset = -15.0f;
                }
            } else {
                offset = -18.0f;
            }
        } else {
            offset = -21.0f;
        }
    } else {
        offset = -24.0f;
    }
    y += offset;
calc:
    return std::pow(10.0f, y);
}

}  // namespace ksys
