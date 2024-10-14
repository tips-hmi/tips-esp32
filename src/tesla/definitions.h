
#pragma once

#include <STN_OBD_CAN.h>

#define SG(type, startBit, bitLength, ...) \
  CANMessageEncoder::extract_##type(frame, startBit, bitLength, ##__VA_ARGS__)

#define ID102VCLEFT_doorStatus 0x102
#define VCLEFT_frontLatchStatus SG(uint8, 0, 4)
#define VCLEFT_rearLatchStatus SG(uint8, 4, 4)

#define ID103VCRIGHT_doorStatus 0x103
#define VCRIGHT_frontLatchStatus SG(uint8, 0, 4)
#define VCRIGHT_rearLatchStatus SG(uint8, 4, 4)

#define ID118DriveSystemStatus 0x118
#define DI_gear SG(uint8, 21, 3)

#define ID257DIspeed 0x257
#define DI_uiSpeed SG(uint16, 24, 9)

#define ID273UI_vehicleControl 0x273
#define UI_displayBrightnessLevel SG(uint8, 32, 8, 0.5)

#define ID292BMS_SOC 0x292
#define SOCmin292 SG(float, 0, 10, 0.1)

#define ID318SystemTimeUTC 0x318

#define ID321VCFRONT_sensors 0x321
#define VCFRONT_tempAmbientFiltered SG(float, 40, 8, 0.5, -40)

#define ID33AUI_rangeSOC 0x33A
#define UI_Range SG(uint16, 0, 10)

#define ID3C2VCLEFT_switchStatus 0x3C2
#define VCLEFT_swcRightScrollTicks SG(int8, 24, 6)

#define ID3E2VCLEFT_lightStatus 0x3E2
#define VCLEFT_turnSignalStatus SG(uint8, 4, 2)
#define VCLEFT_brakeLightStatus SG(uint8, 0, 2)

#define ID3E3VCRIGHT_lightStatus 0x3E3
#define VCRIGHT_turnSignalStatus SG(uint8, 4, 2)

const CANMessageDefinitionCollection TESLA_CAN_MESSAGES({
    // Left Door Status
    CANMessageDefinition(ID102VCLEFT_doorStatus),

    // Right Door Status
    CANMessageDefinition(ID103VCRIGHT_doorStatus),

    // Drive System Status
    CANMessageDefinition(ID118DriveSystemStatus),

    // Speed
    CANMessageDefinition(ID257DIspeed),

    // Ambient Brightness
    CANMessageDefinition(ID273UI_vehicleControl),

    // State of Charge
    CANMessageDefinition(ID292BMS_SOC),

    // System Time UTC
    CANMessageDefinition(ID318SystemTimeUTC),

    // Temperature
    CANMessageDefinition(ID321VCFRONT_sensors),

    // Range
    CANMessageDefinition(ID33AUI_rangeSOC),

    // Buttons
    CANMessageDefinition(ID3C2VCLEFT_switchStatus),

    // Left turn signal and brake light
    CANMessageDefinition(ID3E2VCLEFT_lightStatus, 7),

    // Left turn signal and brake light
    CANMessageDefinition(ID3E3VCRIGHT_lightStatus, 4),
});
