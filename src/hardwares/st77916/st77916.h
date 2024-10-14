#pragma once

#include <ESP_IOExpander.h>
#include <ESP_Panel_Library.h>

#include "device_conf.h"

static ESP_PanelLcd *lcd = NULL;
static ESP_PanelBacklight *backlight = NULL;

#define TFT_SPI_FREQ_HZ (50 * 1000 * 1000)

void st77916_init() {
  ledc_timer_config_t ledc_timer = {.speed_mode = LEDC_LOW_SPEED_MODE, .duty_resolution = LEDC_TIMER_13_BIT, .timer_num = LEDC_TIMER_0, .freq_hz = 5000, .clk_cfg = LEDC_AUTO_CLK};
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  ledc_channel_config_t ledc_channel = {.gpio_num = (TFT_BLK), .speed_mode = LEDC_LOW_SPEED_MODE, .channel = LEDC_CHANNEL_0, .intr_type = LEDC_INTR_DISABLE, .timer_sel = LEDC_TIMER_0, .duty = 0, .hpoint = 0};

  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

  backlight = new ESP_PanelBacklight(ledc_timer, ledc_channel);
  backlight->begin();
  backlight->off();

  ESP_PanelBus_QSPI *panel_bus = new ESP_PanelBus_QSPI(TFT_CS, TFT_SCK, TFT_SDA0, TFT_SDA1, TFT_SDA2, TFT_SDA3);
  panel_bus->configQspiFreqHz(TFT_SPI_FREQ_HZ);
  panel_bus->begin();

  lcd = new ESP_PanelLcd_ST77916(panel_bus, 16, TFT_RST);

  lcd->init();
  lcd->reset();
  lcd->begin();

  lcd->invertColor(true);
  lcd->displayOn();

  lcd->colorBarTest(SCREEN_RES_HOR, SCREEN_RES_VER);
}
