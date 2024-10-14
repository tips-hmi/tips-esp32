#pragma once

#include "Display.h"

class TouchDisplay : public Display {
 public:
  /**
   * @brief 设置触摸屏的旋转
   * @param rotation 旋转角度。0 USB 在上方，1 USB 在左侧，2 USB 在下方，3 USB 在右侧
   */
  void setRotation(uint8_t rotation) override {
    Display::setRotation(rotation);

    if (rotation > 3) return;
    if (lcd == NULL || touch == NULL) return;

    switch (rotation) {
      case 1:  // 顺时针90度
        touch->swapXY(true);
        touch->mirrorX(true);
        touch->mirrorY(false);
        break;
      case 2:
        touch->swapXY(false);
        touch->mirrorX(true);
        touch->mirrorY(true);
        break;
      case 3:
        touch->swapXY(true);
        touch->mirrorX(false);
        touch->mirrorY(true);
        break;
      default:
        touch->swapXY(false);
        touch->mirrorX(false);
        touch->mirrorY(false);
        break;
    }
  }
};
