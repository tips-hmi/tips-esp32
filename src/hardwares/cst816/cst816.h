#pragma once

#include <ESP_IOExpander.h>
#include <ESP_Panel_Library.h>

#include "device_conf.h"

static ESP_PanelTouch *touch = NULL;

void cst816_init() {
  ESP_PanelBus_I2C *touchBus =
      new ESP_PanelBus_I2C(TOUCH_PIN_NUM_I2C_SCL, TOUCH_PIN_NUM_I2C_SDA,
                           ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG());
  touchBus->configI2cFreqHz(400000);
  touchBus->begin();

  touch = new ESP_PanelTouch_CST816S(touchBus, SCREEN_RES_HOR, SCREEN_RES_VER,
                                     TOUCH_PIN_NUM_RST, TOUCH_PIN_NUM_INT);
  touch->init();
  touch->begin();
}
