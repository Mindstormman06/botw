#include "KingSystem/Map/mapDebug.h"
#include "KingSystem/ActorSystem/actActor.h"
#include "KingSystem/Map/mapObject.h"
#include "KingSystem/Map/mapPlacementMapMgr.h"

namespace ksys::map {

void makeProjectMapMuuntPath(sead::BufferedSafeString* out_path, Object* object) {
    if (out_path == nullptr || object == nullptr)
        return;

    out_path->format("Map/Project/");

    sead::FormatFixedSafeString<32> map_type;
    PlacementMapMgr::instance()->getMapType(&map_type);
    out_path->append(map_type);

    const char* map_name = object->getUnitConfigNameFromByaml();
    if (map_name && map_name[0] != '\0') {
        out_path->append("/");
        out_path->append(map_name);
        out_path->append(".muunt");
    }
}

bool printDebugMsg(Object* object, const sead::SafeString& msg, const char* config_name) {
    if (object == nullptr)
        return false;

    sead::FixedSafeString<0x200> path;
    makeProjectMapMuuntPath(&path, object);
    return true;
}

bool printDebugMsg(ksys::act::Actor* actor, const sead::SafeString& msg, const char* config_name) {
    Object* o = actor->getMapObject();
    if (o == nullptr)
        return false;

    sead::FixedSafeString<0x200> path;
    makeProjectMapMuuntPath(&path, o);
    return true;
}

}  // namespace ksys::map
