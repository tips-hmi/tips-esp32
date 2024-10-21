#pragma once

#define LOCAL_DEBUG 0

#include <STN_OBD_CAN.h>
#include <mx_ui.h>

#include "tesla/TeslaCANMessageProcessor.h"
#include "tesla/TeslaVehicle.h"
#include "tesla/definitions.h"
#include "ui/scenes/ChargingScene.h"
#include "ui/scenes/DoorScene.h"
#include "ui/scenes/MainScene.h"
#include "ui/scenes/WatchScene.h"

TeslaVehicle vehicle;

const char* DEVICE_ADDRESS = "c0:08:e1:98:fc:c8";
BLESerialConnection connection(1024);
TeslaCANMessageProcessor processor(vehicle);
CANMessageListener listener(TESLA_CAN_MESSAGES);
OBDConnector connector(connection, TESLA_CAN_MESSAGES);

ChargingScene* chargingScene;
DoorScene* doorScene;
MainScene* mainScene;
WatchScene* watchScene;

class TipApp : public Application {
 protected:
  void onInit() override {
    Application::onInit();

#if LOCAL_DEBUG
    struct tm tm;
    auto buildTime = String(__TIME__);
    tm.tm_year = 2025 - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_hour = buildTime.substring(0, 2).toInt() - 8;
    tm.tm_min = buildTime.substring(3, 5).toInt();
    tm.tm_sec = buildTime.substring(6, 8).toInt();
    time_t t = mktime(&tm) + 22;
    struct timeval now = {.tv_sec = t};
    settimeofday(&now, NULL);
    vehicle.setGear(Gear::P);
    vehicle.setSpeed(36);
    vehicle.setRange(371);
    vehicle.setStateOfCharge(90);
    vehicle.setTemperatureAmbient(23);
    // vehicle.setFrontLeftDoorClosed(false);
#else
    listener.setProcessor(&processor);
    connector.setListener(&listener);
    if (connector.connect(DEVICE_ADDRESS)) {
      connector.startMonitoring();
    } else {
      log_e("Failed to connect to device '%s'", DEVICE_ADDRESS);
      return;
    }
#endif
    mainScene = new MainScene(vehicle);
    watchScene = new WatchScene(vehicle);
    doorScene = new DoorScene(vehicle);
    // chargingScene = new ChargingScene(vehicle);
  }

  void onUpdate() override {
    connector.update();

    Scene* scene = mainScene;
    if (doorScene->isActive() && !doorScene->allDoorsClosed()) {
      scene = doorScene;
    } else {
      if (vehicle.hasDoorOpen()) {
        scene = doorScene;
      } else if (vehicle.getGear() == Gear::INVALID ||
                 vehicle.getGear() == Gear::SNA) {
        scene = watchScene;
      } else if (vehicle.getParkDuration() > 1 * 60 * 1000) {
        scene = watchScene;
      }

      if (scene && !scene->isActive()) {
        if (scene == mainScene) {
          // Back to main scene
          scene->show(LV_SCR_LOAD_ANIM_OVER_RIGHT);
        } else {
          // Jump out to other scene
          scene->show(LV_SCR_LOAD_ANIM_OVER_LEFT);
        }
      }
    }

    Display::main().setBrightness(vehicle.getDisplayBrightnessLevel());
    Application::onUpdate();
  }
};
