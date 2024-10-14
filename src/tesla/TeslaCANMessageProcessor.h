#pragma once

#include <Arduino.h>
#include <STN_OBD_CAN.h>

class TeslaVehicle;

class TeslaCANMessageProcessor : public CANMessageProcessor {
 public:
  TeslaCANMessageProcessor(TeslaVehicle& vehicle) : _vehicle(vehicle) {}

  void process(uint16_t messageId, const uint8_t* frame) override;

 private:
  TeslaVehicle& _vehicle;
};
