#pragma once

#define LOCAL_TEST 0

#include <STN_OBD_CAN.h>
#include <mx_ui.h>

#include "tesla/TeslaCANMessageProcessor.h"
#include "tesla/TeslaVehicle.h"
#include "tesla/definitions.h"
#include "ui/scenes/MainScene.h"

TeslaVehicle vehicle;

const char* DEVICE_ADDRESS = "c0:08:e1:98:fc:c8";
BLESerialConnection connection(1024);
TeslaCANMessageProcessor processor(vehicle);
CANMessageListener listener(TESLA_CAN_MESSAGES);
OBDConnector connector(connection, TESLA_CAN_MESSAGES);

MainScene* mainScene;

class TipApp : public Application {
 protected:
  void onInit() override {
    Application::onInit();

#if LOCAL_TEST
    struct tm tm;
    auto buildTime = String(__TIME__);
    tm.tm_year = 2025 - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_hour = buildTime.substring(0, 2).toInt() - 8;
    tm.tm_min = buildTime.substring(3, 5).toInt();
    tm.tm_sec = buildTime.substring(6, 8).toInt();
    time_t t = mktime(&tm);
    struct timeval now = {.tv_sec = t};
    settimeofday(&now, NULL);
    vehicle.setGear(Gear::D);
    vehicle.setSpeed(36);
    vehicle.setRange(371);
    vehicle.setStateOfCharge(90);
    vehicle.setTemperatureAmbient(23);
#else
    listener.setProcessor(&processor);
    connector.setListener(&listener);
    if (connector.connect(DEVICE_ADDRESS)) {
      connector.startMonitoring();
      mainScene = new MainScene(vehicle);
      mainScene->show(lv_scr_load_anim_t::LV_SCR_LOAD_ANIM_MOVE_RIGHT);
    } else {
      log_e("Failed to connect to device '%s'", DEVICE_ADDRESS);
    }
#endif
  }

  void onUpdate() override {
    connector.update();
    Display::main().setBrightness(vehicle.getDisplayBrightnessLevel());
    Application::onUpdate();
  }
};
