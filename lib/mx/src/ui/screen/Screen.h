#pragma once

#include <lvgl.h>

class Screen {
 public:
  ~Screen() {}

  static Screen &main();

  int getWidth() const { return lv_disp_get_hor_res(_display); }

  int getHeight() const { return lv_disp_get_ver_res(_display); }

 protected:
  Screen(lv_disp_t *display);

 private:
  lv_disp_t *_display;
};
