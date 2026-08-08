#pragma once

#include <prim/seadSafeString.h>

namespace ksys::act {
class Actor;
}  // namespace ksys::act

namespace ksys::map {

class Object;

void makeProjectMapMuuntPath(sead::BufferedSafeString* out_path, Object* object);
bool printDebugMsg(Object* object, const sead::SafeString& msg, const char* config_name);
bool printDebugMsg(ksys::act::Actor* actor, const sead::SafeString& msg, const char* config_name);

}  // namespace ksys::map
