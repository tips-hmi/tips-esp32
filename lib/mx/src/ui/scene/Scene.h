#pragma once

#include "Component.h"
#include "ui/obj/MXObject.h"
#include "ui/screen/Screen.h"

class Scene : public Component {
 public:
  Scene() {}

  MXObject* getRoot() { return root; }

  bool isActive() { return active; }

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

  virtual bool shouldDestroy() { return false; }

  virtual void onInit() override;
  virtual void willDestroy() override;

  virtual void onActivate() {};
  virtual void onDeactivate() {};
};
