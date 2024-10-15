#pragma once

#include <Arduino.h>

class TeslaCANMessageProcessor;

enum Gear {
  INVALID = 0,
  P = 1,
  R = 2,
  N = 3,
  D = 4,
  SNA =7
};

#define TESLA_VEHICLE_LOG_ENABLED 0

class TeslaVehicle {
  friend class TeslaCANMessageProcessor;

 public:
  TeslaVehicle() { reset(); }

  Gear getGear() const { return _gear; }
  void setGear(Gear gear) {
    if (_gear != gear) {
      _gear = gear;
      logState("Gear", (uint16_t)_gear);
    }
  }

  uint16_t getSpeed() const { return _speed; }
  void setSpeed(uint16_t speed) {
    if (_speed != speed) {
      _speed = speed;
      logState("Speed", _speed);
    }
  }

  float_t getRange() const { return _range; }
  void setRange(float_t range) {
    if (_range != range) {
      _range = range;
      logState("Range", _range);
    }
  }

  float_t getStateOfCharge() const { return _stateOfCharge; }
  void setStateOfCharge(float_t stateOfCharge) {
    if (_stateOfCharge != stateOfCharge) {
      _stateOfCharge = stateOfCharge;
      logState("State of charge", _stateOfCharge);
    }
  }

  float_t getTemperatureAmbient() const { return _temperatureAmbient; }
  void setTemperatureAmbient(float_t temperatureAmbient) {
    if (_temperatureAmbient != temperatureAmbient) {
      _temperatureAmbient = temperatureAmbient;
      logState("Temperature ambient", _temperatureAmbient);
    }
  }

  uint8_t getDisplayBrightnessLevel() const { return _displayBrightnessLevel; }
  void setDisplayBrightnessLevel(uint8_t displayBrightnessLevel) {
    if (_displayBrightnessLevel != displayBrightnessLevel) {
      _displayBrightnessLevel = displayBrightnessLevel;
      logState("Display brightness level", _displayBrightnessLevel);
    }
  }

  bool isLeftTurnSignalOn() const { return _leftTurnSignal; }
  void setLeftTurnSignal(bool leftTurnSignal) {
    if (_leftTurnSignal != leftTurnSignal) {
      _leftTurnSignal = leftTurnSignal;
      logState("Left turn signal", _leftTurnSignal);
    }
  }

  bool isRightTurnSignalOn() const { return _rightTurnSignal; }
  void setRightTurnSignal(bool rightTurnSignal) {
    if (_rightTurnSignal != rightTurnSignal) {
      _rightTurnSignal = rightTurnSignal;
      logState("Right turn signal", _rightTurnSignal);
    }
  }

  bool isHazardLightsOn() const {
    return isRightTurnSignalOn() && isLeftTurnSignalOn();
  }

  bool isBrakeLightOn() const { return _brakeLight; }
  void setBrakeLight(bool value) {
    if (_brakeLight != value) {
      _brakeLight = value;
      logState("Brake light", _brakeLight);
    }
  }

  bool isFrontLeftDoorClosed() const { return _frontLeftDoorClosed; }
  void setFrontLeftDoorClosed(bool value) {
    if (_frontLeftDoorClosed != value) {
      _frontLeftDoorClosed = value;
      logState("Front left door closed", _frontLeftDoorClosed);
    }
  }

  bool isFrontRightDoorClosed() const { return _frontRightDoorClosed; }
  void setFrontRightDoorClosed(bool value) {
    if (_frontRightDoorClosed != value) {
      _frontRightDoorClosed = value;
      logState("Front right door closed", _frontRightDoorClosed);
    }
  }

  bool isRearLeftDoorClosed() const { return _rearLeftDoorClosed; }
  void setRearLeftDoorClosed(bool value) {
    if (_rearLeftDoorClosed != value) {
      _rearLeftDoorClosed = value;
      logState("Rear left door closed", _rearLeftDoorClosed);
    }
  }

  bool isRearRightDoorClosed() const { return _rearRightDoorClosed; }
  void setRearRightDoorClosed(bool value) {
    if (_rearRightDoorClosed != value) {
      _rearRightDoorClosed = value;
      logState("Rear right door closed", _rearRightDoorClosed);
    }
  }

  int8_t getSwitchRightScrollTicks() const { return _switchRightScrollTicks; }
  void setSwitchRightScrollTicks(int8_t value) {
    if (_switchRightScrollTicks != value) {
      _switchRightScrollTicks = value;
      logState("Switch right scroll ticks", _switchRightScrollTicks);
    }
  }

  bool hasDoorOpen() {
    return !isFrontLeftDoorClosed() || !isFrontRightDoorClosed() ||
           !isRearLeftDoorClosed() || !isRearRightDoorClosed();
  }

  bool isMoving() const { return getSpeed() > 0; }

  void reset() {
    _gear = Gear::P;
    _speed = 0;
    _range = 0;
    _stateOfCharge = 0;
    _temperatureAmbient = 0;
    _displayBrightnessLevel = 50;
    _leftTurnSignal = false;
    _rightTurnSignal = false;
    _brakeLight = false;
    _frontLeftDoorClosed = true;
    _frontRightDoorClosed = true;
    _rearLeftDoorClosed = true;
    _rearRightDoorClosed = true;
    _switchRightScrollTicks = 21;
  }

  void logState(const char* name, int8_t value) {
#if TESLA_VEHICLE_LOG_ENABLED
    Serial.printf("%s: %d\n", name, value);
#endif
  }

  void logState(const char* name, uint8_t value) {
#if TESLA_VEHICLE_LOG_ENABLED
    Serial.printf("%s: %d\n", name, value);
#endif
  }

  void logState(const char* name, uint16_t value) {
#if TESLA_VEHICLE_LOG_ENABLED
    Serial.printf("%s: %d\n", name, value);
#endif
  }

  void logState(const char* name, float_t value) {
#if TESLA_VEHICLE_LOG_ENABLED
    Serial.printf("%s: %.2f\n", name, value);
#endif
  }

  void logState(const char* name, bool value) {
#if TESLA_VEHICLE_LOG_ENABLED
    Serial.printf("%s: %s\n", name, value ? "true" : "false");
#endif
  }

 private:
  Gear _gear;
  uint16_t _speed;
  float_t _range;
  float_t _stateOfCharge;

  float_t _temperatureAmbient;

  uint8_t _displayBrightnessLevel;

  bool _leftTurnSignal;
  bool _rightTurnSignal;
  bool _brakeLight;

  bool _frontLeftDoorClosed;
  bool _frontRightDoorClosed;
  bool _rearLeftDoorClosed;
  bool _rearRightDoorClosed;

  int8_t _switchRightScrollTicks;
};
