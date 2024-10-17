#pragma once

#include <mx_ui.h>

#include "fonts.h"
#include "tesla/TeslaVehicle.h"
#include "ui/Theme.h"
#include "ui/watch/WatchFace.h"
#include "utils/time_util.h"

void handleClick(lv_event_t *e);

class WatchScene : public Scene {
 public:
  WatchScene(const TeslaVehicle &vehicle) : _vehicle(vehicle) {}

  void nextWatchFace() {
    _watchFaceIndex++;
    if (_watchFaceIndex >= watchFaces.size()) {
      _watchFaceIndex = 0;
    }
    _watchFace = watchFaces.at(_watchFaceIndex);
    _applyWatchFace(_watchFace);
  }

  void previousWatchFace() {
    if (_watchFaceIndex == 0) {
      _watchFaceIndex = watchFaces.size() - 1;
    } else {
      _watchFaceIndex--;
    }
    _watchFace = watchFaces.at(_watchFaceIndex);
    _applyWatchFace(_watchFace);
  }

 protected:
  MXObject *backgroundImage;
  MXObject *socArc;
  MXObject *timeLabel;

  void onInit() override {
    Scene::onInit();

    socArc = &mx(this)
                  ->progress_arc()
                  .size_full()
                  .center()
                  .arc_rotation(270)
                  .arc_width(5);

    backgroundImage = &mx(this)->image().center();
    timeLabel = &mx(this)->label().center().text_letter_space(4);

    _applyWatchFace(_watchFace);

    mx(this)->onGesture([](lv_event_t *e) {
      lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());

      WatchScene *scene = static_cast<WatchScene *>(e->user_data);
      if (dir == LV_DIR_RIGHT) {
        scene->previousWatchFace();
      } else if (dir == LV_DIR_LEFT) {
        scene->nextWatchFace();
      }
    });
  }

  void onUpdate() override {
    Scene::onUpdate();

    float_t soc = _vehicle.getStateOfCharge();
    socArc->arc_value(soc);
    if (soc <= 20.0) {
      socArc->arc_color(theme.warningColor);
    } else {
      socArc->arc_color(theme.successColor);
    }
    timeLabel->text(formatTime());
  }

 private:
  const TeslaVehicle &_vehicle;
  uint16_t _watchFaceIndex = 0;
  WatchFace *_watchFace = watchFaces[_watchFaceIndex];

  void _applyWatchFace(WatchFace *watchFace) {
    backgroundImage->src(String("A:/watch-face-") + watchFace->backgroundImage +
                         ".png");
    timeLabel->text_color(watchFace->timeColor);
    timeLabel->font(watchFace->timeFont);
    if (watchFace->timeX != 0) {
      timeLabel->x(watchFace->timeX);
    }
    if (watchFace->timeY != 0) {
      timeLabel->y(watchFace->timeY);
    }
    if (watchFace->timeX == 0 && watchFace->timeY == 0) {
      timeLabel->center();
    } else {
      if (watchFace->timeX == 0) {
        timeLabel->center_x();
      }
      if (watchFace->timeY == 0) {
        timeLabel->center_y();
      }
    }
  }
};
