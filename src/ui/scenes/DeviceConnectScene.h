#pragma once

#include <mx_ui.h>

#include "fonts.h"

class ConnectingScene : public Scene {
 protected:
  MXObject *label;

  void onInit() override {
    Scene::onInit();

    label = &mx(this)
                 ->label()
                 .text_color(0x520404)
                 .center_x()
                 .y(44)
                 .font(&lv_font_montserrat_24)
                 .text("Connecting...");
  }
};
