#include "Scene.h"

#include <lvgl.h>

#include "Component.h"
#include "ui/app/Application.h"
#include "ui/obj/MXObject.h"

void Scene::activate() {
  auto activeScene = Application::current()->getActiveScene();
  if (activeScene != nullptr) {
    activeScene->deactivate();
    if (activeScene->shouldDestroy()) {
      Application::current()->destroyScene(activeScene);
    }
    activeScene = nullptr;
  }

  begin();

  Application::current()->setActiveScene(this);

  active = true;
  onActivate();
}

void Scene::deactivate() {
  onDeactivate();
  active = false;
}

void Scene::show(lv_scr_load_anim_t animation, uint32_t duration) {
  if (!active) {
    activate();
  }
  lv_scr_load_anim(root->lv_obj(), animation, duration, 0, false);
}

void Scene::onInit() {
  Component::onInit();
  root = &mx()->size_full().bg_color(0x000000);
  width = lv_obj_get_width(root->lv_obj());
  height = lv_obj_get_height(root->lv_obj());
#ifdef SDL2
  root->clip_corner().rounded_full();
#endif
}

void Scene::willDestroy() {
  Scene::willDestroy();
  delete root;
  root = nullptr;
}

void Scene::update() {
  if (root) {
    Component::update();
  }
}
