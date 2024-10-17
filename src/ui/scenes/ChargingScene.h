#pragma once

#include <mx_ui.h>

#include "fonts.h"
#include "tesla/TeslaVehicle.h"
#include "ui/Theme.h"

class ChargingScene : public Scene {
 public:
  ChargingScene(const TeslaVehicle &vehicle) : _vehicle(vehicle) {}

 protected:
  MXObject *label;

  void onInit() override {
    Scene::onInit();
    label = &mx(this)
                 ->label()
                 .text_color(theme.textColor)
                 .text_letter_space(4)
                 .center()
                 .font(&lv_font_orbitron_medium_64);
  }

  void onUpdate() override { Scene::onUpdate(); }

 private:
  const TeslaVehicle &_vehicle;
};
