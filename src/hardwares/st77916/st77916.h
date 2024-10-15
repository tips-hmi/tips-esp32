#pragma once

#include <ESP_IOExpander.h>
#include <ESP_Panel_Library.h>

#include "../../flash/splash_01.h"
#include "../../flash/splash_02.h"
#include "device_conf.h"

static ESP_PanelLcd *lcd = NULL;
static ESP_PanelBacklight *backlight = NULL;

#define TFT_SPI_FREQ_HZ (50 * 1000 * 1000)

uint16_t rgb565(uint32_t color24bit);
bool lcd_clear(uint32_t color24bit = 0x000000);
bool lcd_draw_splash();

void st77916_init() {
  ledc_timer_config_t ledc_timer = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                    .duty_resolution = LEDC_TIMER_13_BIT,
                                    .timer_num = LEDC_TIMER_0,
                                    .freq_hz = 5000,
                                    .clk_cfg = LEDC_AUTO_CLK};
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  ledc_channel_config_t ledc_channel = {.gpio_num = (TFT_BLK),
                                        .speed_mode = LEDC_LOW_SPEED_MODE,
                                        .channel = LEDC_CHANNEL_0,
                                        .intr_type = LEDC_INTR_DISABLE,
                                        .timer_sel = LEDC_TIMER_0,
                                        .duty = 0,
                                        .hpoint = 0};

  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

  backlight = new ESP_PanelBacklight(ledc_timer, ledc_channel);
  backlight->begin();
  backlight->off();

  ESP_PanelBus_QSPI *panel_bus = new ESP_PanelBus_QSPI(
      TFT_CS, TFT_SCK, TFT_SDA0, TFT_SDA1, TFT_SDA2, TFT_SDA3);
  panel_bus->configQspiFreqHz(TFT_SPI_FREQ_HZ);
  panel_bus->begin();

  lcd = new ESP_PanelLcd_ST77916(panel_bus, 16, TFT_RST);

  lcd->init();
  lcd->reset();
  lcd->begin();

  lcd->invertColor(true);
  lcd->displayOn();

  // lcd_draw_splash();
  lcd->colorBarTest(SCREEN_RES_HOR, SCREEN_RES_VER);
}

bool lcd_draw_splash() {
  auto randomIndex = random(1, 3);
  auto map = randomIndex == 1 ? splash_01_map : splash_02_map;
  return lcd->drawBitmapWaitUntilFinish(0, 0, SCREEN_RES_HOR, SCREEN_RES_VER,
                                        map);
}

bool lcd_clear(uint32_t color24bit) {
  uint16_t color16bit = rgb565(color24bit);
  int bits_per_pixel = 16;  // 16 bits per pixel
  int bytes_per_pixel = bits_per_pixel / 8;
  uint8_t *color_buf = nullptr;

  try {
    // Allocate memory for one line
    color_buf = new uint8_t[SCREEN_RES_HOR * bytes_per_pixel];
  } catch (std::bad_alloc &e) {
    return false;
  }

  // Fill the buffer with the specified color
  for (int i = 0; i < SCREEN_RES_HOR; i++) {
    color_buf[i * 2] = (color16bit >> 8) & 0xFF;  // High byte
    color_buf[i * 2 + 1] = color16bit & 0xFF;     // Low byte
  }

  // Draw the color across the entire screen
  bool ret = true;
  for (int j = 0; j < SCREEN_RES_VER; j++) {
    ret = lcd->drawBitmapWaitUntilFinish(0, j, SCREEN_RES_HOR, 1, color_buf);
    if (!ret) {
      break;
    }
  }

  delete[] color_buf;
  return ret;
}

uint16_t rgb565(uint32_t color24bit) {
  uint8_t r = (color24bit >> 16) & 0xFF;
  uint8_t g = (color24bit >> 8) & 0xFF;
  uint8_t b = color24bit & 0xFF;

  uint16_t color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  return color565;
}
