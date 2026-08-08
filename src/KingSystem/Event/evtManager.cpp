#include "KingSystem/Event/evtManager.h"

namespace ksys::evt {

SEAD_SINGLETON_DISPOSER_IMPL(Manager)

bool Manager::hasActiveEvent() const {
    return mActiveEvent != nullptr;
}

Event* Manager::getActiveEvent() const {
    if (mActiveEvent == nullptr)
        return nullptr;

    u32 idx = *reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(mActiveEvent) + 0x1f0);
    if (idx > 7)
        idx = 0;

    s8 entry_idx = *reinterpret_cast<s8*>(reinterpret_cast<uintptr_t>(mActiveEvent) + idx + 0x1e8);
    u32 count = *reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(mActiveEvent) + 8);
    if (static_cast<u32>(entry_idx) < count) {
        auto* array = *reinterpret_cast<Event***>(reinterpret_cast<uintptr_t>(mActiveEvent) + 0x10);
        return array[entry_idx];
    }
    return nullptr;
}

}  // namespace ksys::evt
