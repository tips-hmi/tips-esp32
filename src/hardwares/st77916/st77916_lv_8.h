#pragma once

#include <lvgl.h>

#include "st77916.h"

static lv_color_t *disp_draw_buf;
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  ESP_PanelLcd *lcd = (ESP_PanelLcd *)disp->user_data;
  const int offsetX1 = area->x1;
  const int offsetX2 = area->x2;
  const int offsetY1 = area->y1;
  const int offsetY2 = area->y2;
  lcd->drawBitmap(offsetX1, offsetY1, offsetX2 - offsetX1 + 1, offsetY2 - offsetY1 + 1, (const uint8_t *)color_p);
}

IRAM_ATTR bool onRefreshFinishCallback(void *user_data) {
  lv_disp_drv_t *drv = (lv_disp_drv_t *)user_data;
  lv_disp_flush_ready(drv);
  return false;
}

void lv_st77916_init() {
  st77916_init();

  size_t lv_cache_rows = 72;

  disp_draw_buf = (lv_color_t *)heap_caps_malloc(lv_cache_rows * SCREEN_RES_HOR * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, SCREEN_RES_HOR * lv_cache_rows);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_RES_HOR;
  disp_drv.ver_res = SCREEN_RES_VER;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.user_data = (void *)lcd;
  lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

  if (lcd->getBus()->getType() != ESP_PANEL_BUS_TYPE_RGB) {
    lcd->attachRefreshFinishCallback(onRefreshFinishCallback, (void *)disp->driver);
  }
}
