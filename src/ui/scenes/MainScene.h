#pragma once

#include <mx_ui.h>

#include "fonts.h"
#include "tesla/TeslaVehicle.h"
#include "ui/Theme.h"
#include "utils/time_util.h"

class MainScene : public Scene {
 public:
  MainScene(const TeslaVehicle &vehicle) : _vehicle(vehicle) {}

 protected:
  MXObject *timeLabel;
  MXObject *temperatureLabel;
  MXObject *speedLabel;
  MXObject *rangeLabel;
  MXObject *gearLabel;
  MXObject *socArc;
  MXObject *leftSignalArc;
  MXObject *rightSignalArc;
  MXObject *brakeLightLine;

  const TeslaVehicle &getVehicle() const { return _vehicle; }

  void onInit() override {
    Scene::onInit();
    socArc = &mx(this)
                  ->progress_arc()
                  .size_full()
                  .center()
                  .arc_rotation(270)
                  .arc_width(5);

    timeLabel = &mx(this)
                     ->label()
                     .x(70)
                     .y(62)
                     .text_color(theme.secondaryTextColor)
                     .text_letter_space(2)
                     .font(&lv_font_montserrat_28);
    temperatureLabel = &mx(this)
                            ->label()
                            .x(194)
                            .y(62)
                            .w(100)
                            .text_color(theme.secondaryTextColor)
                            .text_letter_space(2)
                            .text_align(LV_TEXT_ALIGN_RIGHT)
                            .font(&lv_font_montserrat_28);
    mx(this)->hr_line(256, 100).center_x().line_color(0x555555);
    speedLabel = &mx(this)
                      ->label()
                      .center_x()
                      .y(96)
                      .text_color(theme.textColor)
                      .text_letter_space(4)
                      .font(&lv_font_orbitron_medium_128);
    rangeLabel = &mx(this)
                      ->label()
                      .center_x()
                      .y(244)
                      .text_color(theme.secondaryTextColor)
                      .text_letter_space(4)
                      .font(&lv_font_orbitron_medium_24);
    gearLabel = &mx(this)
                     ->label()
                     .center_x()
                     .y(290)
                     .text_color(theme.textColor)
                     .font(&lv_font_orbitron_medium_42);

    leftSignalArc = &mx(this)
                         ->arc()
                         .size(Screen::main().getWidth() - 40)
                         .center()
                         .arc_value(10)
                         .arc_rotation(162)
                         .arc_color(theme.warningColor);

    rightSignalArc = &mx(this)
                          ->arc()
                          .size(Screen::main().getWidth() - 40)
                          .center()
                          .arc_value(10)
                          .arc_rotation(162 + 180)
                          .arc_color(theme.warningColor);

    brakeLightLine =
        &mx(this)->hr_line(128, 284).center_x().line_width(6).line_color(
            0x000000);
  }

  void onUpdate() override {
    Scene::onUpdate();

    timeLabel->text(formatTime());

    temperatureLabel->text(
        _formatTemperature(_vehicle.getTemperatureAmbient()));

    speedLabel->text(_formatSpeed(_vehicle.getSpeed()));

    float_t soc = _vehicle.getStateOfCharge();
    float_t range = _vehicle.getRange();
    socArc->arc_value(soc);
    rangeLabel->text(_formatRange(range));
    if (soc <= 20.0) {
      rangeLabel->text_color(theme.warningColor);
      socArc->arc_color(theme.warningColor);
    } else {
      rangeLabel->text_color(theme.secondaryTextColor);
      socArc->arc_color(theme.successColor);
    }

    Gear gear = _vehicle.getGear();
    gearLabel->text(_formatGear(gear));
    if (gear == Gear::R) {
      gearLabel->text_color(theme.warningColor);
    } else {
      gearLabel->text_color(theme.textColor);
    }

    if (_vehicle.isLeftTurnSignalOn()) {
      leftSignalArc->arc_color(theme.warningColor);
    } else {
      leftSignalArc->arc_color(0x000000);
    }

    if (_vehicle.isRightTurnSignalOn()) {
      rightSignalArc->arc_color(theme.warningColor);
    } else {
      rightSignalArc->arc_color(0x000000);
    }

    if (_vehicle.isBrakeLightOn()) {
      brakeLightLine->line_color(theme.errorColor);
    } else {
      brakeLightLine->line_color(0x000000);
    }
  }

 private:
  const TeslaVehicle &_vehicle;

  const String _formatSpeed(uint16_t value) const { return String(value); }

  const String _formatRange(float_t value) const {
    return String(uint16_t(floor(value))) + " km";
  }

  const String _formatTemperature(float_t value) const {
    return String(uint16_t(round(value))) + "°C";
  }

  const String _formatGear(Gear gear) const {
    switch (gear) {
      case Gear::P:
        return "P";
      case Gear::R:
        return "R";
      case Gear::N:
        return "N";
      case Gear::D:
        return "D";
      case Gear::SNA:
        return "";
      default:
        return "";
    }
  }
};
