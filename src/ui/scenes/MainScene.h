#pragma once

#include <mx_ui.h>

#include <string>

#include "../../tesla/TeslaVehicle.h"
#include "../Theme.h"
#include "fonts.h"

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
                  .arc_width(4);

    timeLabel = &mx(this)
                     ->label()
                     .text_color(theme.secondaryTextColor)
                     .text_letter_space(2)
                     .x(64)
                     .y(60)
                     .font(&lv_font_montserrat_24);
    temperatureLabel = &mx(this)
                            ->label()
                            .text_color(theme.secondaryTextColor)
                            .text_letter_space(2)
                            .x(194)
                            .y(60)
                            .w(100)
                            .text_align(LV_TEXT_ALIGN_RIGHT)
                            .font(&lv_font_montserrat_24);
    mx(this)->hr_line(256, 100).center_x().line_color(theme.secondaryTextColor);
    speedLabel = &mx(this)
                      ->label()
                      .text_color(theme.textColor)
                      .text_letter_space(4)
                      .center_x()
                      .y(96)
                      .font(&lv_font_orbitron_medium_128);
    rangeLabel = &mx(this)
                      ->label()
                      .text_color(theme.secondaryTextColor)
                      .text_letter_space(4)
                      .center_x()
                      .y(244)
                      .font(&lv_font_orbitron_medium_24);
    gearLabel = &mx(this)
                     ->label()
                     .text_color(theme.textColor)
                     .center_x()
                     .y(290)
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

    timeLabel->text(_formatTime().c_str());

    temperatureLabel->text(
        _formatTemperature(_vehicle.getTemperatureAmbient()).c_str());

    speedLabel->text(_formatSpeed(_vehicle.getSpeed()).c_str());

    float_t soc = _vehicle.getStateOfCharge();
    float_t range = _vehicle.getRange();
    socArc->arc_value(soc);
    rangeLabel->text(_formatRange(range).c_str());
    if (soc <= 20.0) {
      rangeLabel->text_color(theme.warningColor);
      socArc->arc_color(theme.warningColor);
    } else {
      rangeLabel->text_color(theme.secondaryTextColor);
      socArc->arc_color(theme.successColor);
    }

    Gear gear = _vehicle.getGear();
    gearLabel->text(_formatGear(gear).c_str());
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

  const String _formatTime() const {
    time_t now = time(nullptr);
    struct tm *timeInfo = localtime(&now);
    int hour = (timeInfo->tm_hour + 8) % 24;  // Adjust for UTC+8
    int min = timeInfo->tm_min;
    return (hour < 10 ? "0" : "") + String(hour) + ":" + (min < 10 ? "0" : "") +
           String(min);
  }

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
