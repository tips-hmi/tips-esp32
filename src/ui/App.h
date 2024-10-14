#pragma once

#include <mx_ui.h>

#include "ui/scenes/MainScene.h"
#include "vehicle/TeslaVehicle.h"

Scene *scene;
TeslaVehicle vehicle;

class TipApp : public Application {
 protected:
  void onInit() override {
    Application::onInit();

    scene = new MainScene(vehicle);
    scene->show();
  }
};
