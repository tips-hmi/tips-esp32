#pragma once

#include <STN_OBD_CAN.h>
#include <mx_ui.h>

#include "../tesla/TeslaCANMessageProcessor.h"
#include "../tesla/TeslaVehicle.h"
#include "../tesla/definitions.h"
#include "scenes/MainScene.h"

const char* DEVICE_ADDRESS = "c0:08:e1:98:fc:c8";

TeslaVehicle vehicle;
TeslaCANMessageProcessor processor(vehicle);
CANMessageListener listener(TESLA_CAN_MESSAGES);
OBDConnector connector(TESLA_CAN_MESSAGES);

MainScene* mainScene;

class TipApp : public Application {
 protected:
  void onInit() override {
    Application::onInit();

    listener.setProcessor(&processor);
    connector.setListener(&listener);
    if (connector.connect(DEVICE_ADDRESS)) {
      connector.startMonitoring();
      mainScene = new MainScene(vehicle);
      mainScene->show();
    } else {
      log_e("Failed to connect to device '%s'", DEVICE_ADDRESS);
    }
  }

  void onUpdate() override {
    connector.update();
    Application::onUpdate();
  }
};
