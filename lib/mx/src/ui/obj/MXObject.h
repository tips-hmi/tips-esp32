#pragma once

#include <lvgl.h>

#include <algorithm>

class Scene;

class MXObject {
 public:
  MXObject(lv_obj_t* obj) : _internalObj(obj) {}
  ~MXObject() {
    if (_internalObj != nullptr) {
      lv_obj_del(_internalObj);
    }
    _internalObj = nullptr;
  }

  MXObject* ptr() { return this; }
  lv_obj_t* lv_obj() { return _internalObj; }
  lv_obj_t* parent() { return lv_obj_get_parent(_internalObj); }

  /* Creation */
  MXObject& label(const char* label = "") {
    _internalObj = lv_label_create(_internalObj);
    text(label);
    return *this;
  }

  MXObject& image(const char* imageSrc = "") {
    _internalObj = lv_img_create(_internalObj);
    src(imageSrc);
    return *this;
  }

  MXObject& line(lv_point_t p1, lv_point_t p2) {
    _internalObj = lv_line_create(_internalObj);
    lv_point_t* line_points = new lv_point_t[2]{p1, p2};
    lv_line_set_points(_internalObj, line_points, 2);
    return *this;
  }

  MXObject& hr_line(lv_coord_t width, lv_coord_t y) {
    line(lv_point_t{0, y}, lv_point_t{width, y});
    return *this;
  }

  MXObject& arc() {
    _internalObj = lv_arc_create(_internalObj);
    lv_arc_set_bg_angles(_internalObj, 0, 360);
    remove_style(NULL, LV_PART_KNOB);
    remove_style(NULL, LV_PART_MAIN);
    non_clickable();
    return *this;
  }

  MXObject& progress_arc() {
    _internalObj = lv_arc_create(_internalObj);
    lv_arc_set_bg_angles(_internalObj, 0, 360);
    remove_style(NULL, LV_PART_KNOB);
    non_clickable();
    return *this;
  }

  MXObject& spinner(const lv_coord_t diameter = lv_pct(100),
                    const uint16_t period = 2000, const uint16_t arc = 60) {
    _internalObj = lv_spinner_create(_internalObj, period, arc);
    auto d = diameter;
    lv_obj_set_size(_internalObj, d, d);
    return *this;
  }

  /* Positioning */
  lv_coord_t x() { return lv_obj_get_x(_internalObj); }
  MXObject& x(lv_coord_t x) {
    lv_obj_set_x(_internalObj, x);
    return *this;
  }
  lv_coord_t y() { return lv_obj_get_y(_internalObj); }
  MXObject& y(lv_coord_t y) {
    lv_obj_set_y(_internalObj, y);
    return *this;
  }
  MXObject& pos(lv_coord_t x, lv_coord_t y) {
    lv_obj_set_pos(_internalObj, x, y);
    return *this;
  }

  MXObject& center() {
    lv_obj_center(_internalObj);
    return *this;
  }

  MXObject& center_x() {
    lv_obj_align(_internalObj, LV_ALIGN_TOP_MID, 0, 0);
    return *this;
  }

  MXObject& center_y() {
    lv_obj_align(_internalObj, LV_ALIGN_LEFT_MID, 0, 0);
    return *this;
  }

  MXObject& text_align(lv_text_align_t align) {
    lv_obj_set_style_text_align(_internalObj, align, LV_PART_MAIN);
    return *this;
  }

  /* Size */
  MXObject& size(lv_coord_t width, lv_coord_t height) {
    lv_obj_set_size(_internalObj, width, height);
    return *this;
  }

  MXObject& size(lv_coord_t value) {
    lv_obj_set_size(_internalObj, value, value);
    return *this;
  }

  lv_coord_t w() { return lv_obj_get_width(_internalObj); }
  MXObject& w(lv_coord_t width) {
    lv_obj_set_width(_internalObj, width);
    return *this;
  }

  lv_coord_t h() { return lv_obj_get_width(_internalObj); }
  MXObject& h(lv_coord_t height) {
    lv_obj_set_height(_internalObj, height);
    return *this;
  }

  MXObject& w_full() {
    lv_obj_set_width(_internalObj, lv_pct(100));
    return *this;
  }

  MXObject& h_full() {
    lv_obj_set_height(_internalObj, lv_pct(100));
    return *this;
  }

  MXObject& size_full() {
    lv_obj_set_size(_internalObj, lv_pct(100), lv_pct(100));
    return *this;
  }

  /* Padding */
  MXObject& p(lv_coord_t top, lv_coord_t right, lv_coord_t bottom,
              lv_coord_t left) {
    lv_obj_set_style_pad_top(_internalObj, top, LV_PART_MAIN);
    lv_obj_set_style_pad_right(_internalObj, right, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(_internalObj, bottom, LV_PART_MAIN);
    lv_obj_set_style_pad_left(_internalObj, left, LV_PART_MAIN);
    return *this;
  }

  MXObject& p(lv_coord_t x, lv_coord_t y) {
    p(y, x, y, x);
    return *this;
  }

  MXObject& p(lv_coord_t padding) {
    p(padding, padding, padding, padding);
    return *this;
  }

  MXObject& p_x(lv_coord_t padding) {
    lv_obj_set_style_pad_hor(_internalObj, padding, LV_PART_MAIN);
    return *this;
  }

  MXObject& p_y(lv_coord_t padding) {
    lv_obj_set_style_pad_ver(_internalObj, padding, LV_PART_MAIN);
    return *this;
  }

  /* Background */
  MXObject& bg_color(uint32_t color) {
    lv_obj_set_style_bg_color(_internalObj, lv_color_hex(color), LV_PART_MAIN);
    return *this;
  }

  /* Rounded */
  MXObject& clip_corner() {
    lv_obj_set_style_clip_corner(_internalObj, true, LV_PART_MAIN);
    return *this;
  }

  MXObject& rounded(lv_coord_t radius) {
    lv_obj_set_style_radius(_internalObj, radius, LV_PART_MAIN);
    return *this;
  }

  MXObject& rounded_full() {
    rounded(LV_RADIUS_CIRCLE);
    return *this;
  }

  MXObject& text_color(uint32_t color) {
    lv_obj_set_style_text_color(_internalObj, lv_color_hex(color),
                                LV_PART_MAIN);
    return *this;
  }

  MXObject& font(const lv_font_t* font) {
    lv_obj_set_style_text_font(_internalObj, font, LV_PART_MAIN);
    return *this;
  }

  MXObject& show() {
    lv_obj_clear_flag(_internalObj, LV_OBJ_FLAG_HIDDEN);
    return *this;
  }

  MXObject& hide() {
    lv_obj_add_flag(_internalObj, LV_OBJ_FLAG_HIDDEN);
    return *this;
  }

  MXObject& add_style(lv_style_t* style, lv_part_t part = LV_PART_MAIN) {
    lv_obj_add_style(_internalObj, style, part);
    return *this;
  }

  MXObject& remove_style(lv_style_t* style, lv_part_t part = LV_PART_MAIN) {
    lv_obj_remove_style(_internalObj, style, part);
    return *this;
  }

  MXObject& clickable() {
    lv_obj_add_flag(_internalObj, LV_OBJ_FLAG_CLICKABLE);
    return *this;
  }

  MXObject& non_clickable() {
    lv_obj_clear_flag(_internalObj, LV_OBJ_FLAG_CLICKABLE);
    return *this;
  }

  // Label specific
  const char* text() { return lv_label_get_text(_internalObj); }
  MXObject& text(const char* text) {
    lv_label_set_text(_internalObj, text);
    return *this;
  }

  MXObject& text_fmt(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    lv_label_set_text_fmt(_internalObj, fmt, args);
    va_end(args);
    return *this;
  }

  // Image specific
  const void* src() { return lv_img_get_src(_internalObj); }
  MXObject& src(const char* src) {
    lv_img_set_src(_internalObj, src);
    return *this;
  }

  // Line specific
  MXObject& line_color(uint32_t color) {
    lv_obj_set_style_line_color(_internalObj, lv_color_hex(color),
                                LV_PART_MAIN);
    return *this;
  }

  MXObject& line_width(lv_coord_t width) {
    lv_obj_set_style_line_width(_internalObj, width, LV_PART_MAIN);
    return *this;
  }

  // Arc specific
  MXObject& arc_color(uint32_t color) {
    lv_obj_set_style_arc_color(_internalObj, lv_color_hex(color),
                               LV_PART_INDICATOR);
    return *this;
  }

  MXObject& arc_bg_color(uint32_t color) {
    lv_obj_set_style_arc_color(_internalObj, lv_color_hex(color), LV_PART_MAIN);
    return *this;
  }

  MXObject& arc_width(lv_coord_t width) {
    lv_obj_set_style_arc_width(_internalObj, width, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(_internalObj, width, LV_PART_MAIN);
    return *this;
  }

  MXObject& arc_rotation(lv_coord_t rotation) {
    lv_arc_set_rotation(_internalObj, rotation);
    return *this;
  }

  // Arc value
  MXObject& arc_value(lv_coord_t value) {
    lv_arc_set_value(_internalObj, value);
    return *this;
  }

 private:
  lv_obj_t* _internalObj;
};

MXObject* mx(lv_obj_t* obj);
MXObject* mx(MXObject* obj);
MXObject* mx(Scene* scene);
MXObject* mx();
