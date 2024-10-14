#include "TeslaCANMessageProcessor.h"

#include "TeslaVehicle.h"
#include "definitions.h"

bool __time_has_been_set = false;

void TeslaCANMessageProcessor::process(uint16_t messageId,
                                       const uint8_t* frame) {
  switch (messageId) {
    case ID102VCLEFT_doorStatus: {
      auto value = VCLEFT_frontLatchStatus;
      _vehicle.setFrontLeftDoorClosed(value == 2);
      value = VCLEFT_rearLatchStatus;
      _vehicle.setRearLeftDoorClosed(value == 2);
      break;
    }
    case ID103VCRIGHT_doorStatus: {
      auto value = VCRIGHT_frontLatchStatus;
      _vehicle.setFrontRightDoorClosed(value == 2);
      value = VCRIGHT_rearLatchStatus;
      _vehicle.setRearRightDoorClosed(value == 2);
      break;
    }
    case ID118DriveSystemStatus: {
      auto value = DI_gear;
      if (value == 0 || value == 7) {
        _vehicle.setGear(Gear::UNKNOWN);
      } else {
        _vehicle.setGear(static_cast<Gear>(value));
      }
      break;
    }
    case ID257DIspeed: {
      auto value = DI_uiSpeed;
      _vehicle.setSpeed(value);
      break;
    }
    case ID273UI_vehicleControl: {
      auto value = UI_displayBrightnessLevel;
      _vehicle.setDisplayBrightnessLevel(value);
      break;
    }
    case ID292BMS_SOC: {
      auto value = SOCmin292;
      _vehicle.setStateOfCharge(value);
      break;
    }
    case ID318SystemTimeUTC: {
      if (!__time_has_been_set) {
        time_t t = CANMessageEncoder::extract_time(frame);
        struct timeval now = {.tv_sec = t};
        settimeofday(&now, NULL);
        __time_has_been_set = true;
        log_i("Time has been set to %d", t);
      }
      break;
    }
    case ID321VCFRONT_sensors: {
      auto value = VCFRONT_tempAmbientFiltered;
      _vehicle.setTemperatureAmbient(value);
      break;
    }
    case ID33AUI_rangeSOC: {
      auto value = UI_Range;
      auto valueInKm = convertMilesToKilometers(value);
      _vehicle.setRange(valueInKm);
      break;
    }
    case ID3C2VCLEFT_switchStatus: {
      auto value = VCLEFT_swcRightScrollTicks;
      if (value) {
        _vehicle.setSwitchRightScrollTicks(value);
      }
      break;
    }
    case ID3E2VCLEFT_lightStatus: {
      auto value = VCLEFT_turnSignalStatus;
      _vehicle.setLeftTurnSignal(value == 1);
      value = VCLEFT_brakeLightStatus;
      _vehicle.setBrakeLight(value == 1);
      break;
    }
    case ID3E3VCRIGHT_lightStatus: {
      auto value = VCRIGHT_turnSignalStatus;
      _vehicle.setRightTurnSignal(value == 1);
      break;
    }
    default:
      break;
  }
}
