#include "Screen.h"

Screen &Screen::main() {
  static Screen main(lv_disp_get_default());
  return main;
}

Screen::Screen(lv_disp_t *display) { _display = display; }
