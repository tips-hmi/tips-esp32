#pragma once

#include <fonts.h>
#include <lvgl.h>

#include <vector>

class WatchFace {
 public:
  WatchFace(const char* backgroundImage,
            const lv_font_t* timeFont = &lv_font_montserrat_96,
            const lv_coord_t timeX = 0, const lv_coord_t timeY = 0,
            const uint32_t timeColor = 0xffffff)
      : backgroundImage(backgroundImage),
        timeFont(timeFont),
        timeX(timeX),
        timeY(timeY),
        timeColor(timeColor) {}

  const String backgroundImage;
  const lv_font_t* timeFont;
  const lv_coord_t timeX;
  const lv_coord_t timeY;
  const uint32_t timeColor;
};

std::vector<WatchFace*> watchFaces = {
    new WatchFace("001", &lv_font_montserrat_72),
    new WatchFace("002", &lv_font_montserrat_72),
    new WatchFace("003", &lv_font_montserrat_96),
};
