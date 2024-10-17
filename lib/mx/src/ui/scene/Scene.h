#pragma once

#include <lvgl.h>

#include "Component.h"

class Component;
class MXObject;

class Scene : public Component {
 public:
  Scene() {}

  MXObject* getRoot() const { return root; }

  bool isActive() const { return active; }

  void activate();

  void deactivate();

  void show(lv_scr_load_anim_t animation = LV_SCR_LOAD_ANIM_FADE_ON,
            uint32_t duration = 200);

  void update() override;

 protected:
  MXObject* root;
  lv_coord_t width;
  lv_coord_t height;
  bool active = false;

  virtual bool shouldDestroy() const { return false; }

  virtual void onInit() override;
  virtual void willDestroy() override;

  virtual void onActivate() {};
  virtual void onDeactivate() {};
};
