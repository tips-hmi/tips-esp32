#pragma once

#include <Arduino.h>

class Theme {
 public:
  const uint32_t textColor = 0xffffff;
  const uint32_t secondaryTextColor = 0x8d8d8d;
  const uint32_t successColor = 0x80C96A;
  const uint32_t warningColor = 0xdfa73f;
  const uint32_t errorColor = 0xbf5b4a;
};

static const Theme theme;
