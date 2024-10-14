#pragma once

#define BLE_ENABLED 0

#include <mx_ui.h>

#if BLE_ENABLED
#include <STN_OBD_CAN.h>

#include "../tesla/TeslaCANMessageProcessor.h"
#include "../tesla/definitions.h"
#endif

#include "../tesla/TeslaVehicle.h"
#include "scenes/MainScene.h"

TeslaVehicle vehicle;

#if BLE_ENABLED
const char* DEVICE_ADDRESS = "c0:08:e1:98:fc:c8";
TeslaCANMessageProcessor processor(vehicle);
CANMessageListener listener(TESLA_CAN_MESSAGES);
OBDConnector connector(TESLA_CAN_MESSAGES);
#endif

MainScene* mainScene;

class TipApp : public Application {
 protected:
  void onInit() override {
    Application::onInit();

#if BLE_ENABLED
    listener.setProcessor(&processor);
    connector.setListener(&listener);
    if (connector.connect(DEVICE_ADDRESS)) {
      connector.startMonitoring();
#else
    struct tm tm;
    tm.tm_year = 2024 - 1900;
    tm.tm_mon = 9;
    tm.tm_mday = 14;
    tm.tm_hour = 22 - 8;
    tm.tm_min = 33;
    tm.tm_sec = 0;
    time_t t = mktime(&tm);
    struct timeval now = {.tv_sec = t};
    settimeofday(&now, NULL);
#endif

      mainScene = new MainScene(vehicle);
      mainScene->show();

#if BLE_ENABLED
    } else {
      log_e("Failed to connect to device '%s'", DEVICE_ADDRESS);
    }
#endif
  }

  void onUpdate() override {
#if BLE_ENABLED
    connector.update();
#endif
    Application::onUpdate();
  }
};
