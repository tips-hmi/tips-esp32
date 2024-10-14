#pragma once

#include <lvgl.h>

#include "cst816.h"

IRAM_ATTR bool __lv_on_touch_interrupt_callback(void *user_data) {
  return false;
}

static void __lv_touchpad_read_cb(lv_indev_drv_t *indev_drv,
                                  lv_indev_data_t *data) {
  ESP_PanelTouch *tp = (ESP_PanelTouch *)indev_drv->user_data;
  ESP_PanelTouchPoint point;

  int read_touch_result = tp->readPoints(&point, 1);
  if (read_touch_result > 0) {
    data->point.x = point.x;
    data->point.y = point.y;
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

static lv_indev_t *__lv_init_indev(ESP_PanelTouch *tp) {
  ESP_PANEL_CHECK_FALSE_RET(tp != nullptr, nullptr, "Invalid touch device");
  ESP_PANEL_CHECK_FALSE_RET(tp->getHandle() != nullptr, nullptr,
                            "Touch device is not initialized");
  static lv_indev_drv_t indev_drv_tp;
  lv_indev_drv_init(&indev_drv_tp);
  indev_drv_tp.type = LV_INDEV_TYPE_POINTER;
  indev_drv_tp.read_cb = __lv_touchpad_read_cb;
  indev_drv_tp.user_data = (void *)tp;
  return lv_indev_drv_register(&indev_drv_tp);
}

void lv_cst816_init() {
  cst816_init();

  touch->attachInterruptCallback(__lv_on_touch_interrupt_callback, NULL);
  __lv_init_indev(touch);
}
