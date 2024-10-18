#include "MXObject.h"

#include "ui/scene/Scene.h"

MXObject::MXObject(Scene* scene)
    : _internalObj(scene->getRoot()->lv_obj()), _scene(scene) {}

MXObject::MXObject(MXObject* obj)
    : _internalObj(obj->lv_obj()), _scene(obj->_scene) {}

MXObject* mx(lv_obj_t* obj) { return new MXObject(obj); }
MXObject* mx(MXObject* obj) { return new MXObject(obj); }
MXObject* mx(Scene* scene) { return new MXObject(scene); }
MXObject* mx() { return mx(lv_obj_create(nullptr)); }
