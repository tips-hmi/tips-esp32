#pragma once

#include <mx_ui.h>

#include "fonts.h"
#include "tesla/TeslaVehicle.h"
#include "ui/Theme.h"

class DoorScene : public Scene {
 public:
  DoorScene(const TeslaVehicle &vehicle) : _vehicle(vehicle) {}

  MXObject *vehicleContainer;
  MXObject *vehicleImage;
  MXObject *frontLeftDoorImage;
  MXObject *frontRightDoorImage;
  MXObject *rearLeftDoorImage;
  MXObject *rearRightDoorImage;

  bool allDoorsClosed() const {
    return !_vehicle.hasDoorOpen() && _lastAllDoorsClosedTime > 0 &&
           millis() - _lastAllDoorsClosedTime > 500;
  }

 protected:
  void onInit() override {
    Scene::onInit();
    vehicleContainer = &mx(this)->container().size(220, 268).center();
    vehicleImage =
        &mx(vehicleContainer)->image().center().src("A:/vehicle-top-view.png");
    frontLeftDoorImage = &mx(vehicleContainer)
                              ->image()
                              .hide()
                              .x(0)
                              .y(74)
                              .src("A:/vehicle-top-view-dfl.png");
    frontRightDoorImage = &mx(vehicleContainer)
                               ->image()
                               .hide()
                               .x(156)
                               .y(74)
                               .src("A:/vehicle-top-view-dfr.png");
    rearLeftDoorImage = &mx(vehicleContainer)
                             ->image()
                             .hide()
                             .x(0)
                             .y(134)
                             .src("A:/vehicle-top-view-drl.png");
    rearRightDoorImage = &mx(vehicleContainer)
                              ->image()
                              .hide()
                              .x(146)
                              .y(134)
                              .src("A:/vehicle-top-view-drr.png");
  }

  void onActivate() override {
    Scene::onActivate();
    _lastAllDoorsClosedTime = 0;
  }

  void onUpdate() override {
    Scene::onUpdate();

    frontLeftDoorImage->toggle(!_vehicle.isFrontLeftDoorClosed());
    frontRightDoorImage->toggle(!_vehicle.isFrontRightDoorClosed());
    rearLeftDoorImage->toggle(!_vehicle.isRearLeftDoorClosed());
    rearRightDoorImage->toggle(!_vehicle.isRearRightDoorClosed());

    if (_vehicle.hasDoorOpen()) {
      _lastAllDoorsClosedTime = 0;
    } else {
      if (_lastAllDoorsClosedTime == 0) {
        _lastAllDoorsClosedTime = millis();
      }
    }
  }

 private:
  const TeslaVehicle &_vehicle;
  long _lastAllDoorsClosedTime;
};
