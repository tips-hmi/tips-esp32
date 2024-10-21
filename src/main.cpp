#include <Arduino.h>
#include <SPIFFS.h>

#define TOUCH_ENABLED 0

// LVGL
#include <lvgl.h>

// Always include `device_conf.h` first
#include "device_conf.h"

// Hardwares
#include "hardwares/st77916/st77916_lv_8.h"
#if TOUCH_ENABLED
#include "hardwares/cst816/cst816_lv_8.h"
#endif

// Drivers
#if TOUCH_ENABLED
#include "drivers/display/TouchDisplay.h"
#else
#include "drivers/display/Display.h"
#endif
#include "drivers/spiffs-driver/SPIFFSDriver.h"

// App
#include "App.h"

#if TOUCH_ENABLED
TouchDisplay display;
#else
Display display;
#endif
SPIFFSDriver spiffsDriver;
TipApp* app = nullptr;

void mx_setup();
void mx_loop();

void initLogging() {
  lv_log_register_print_cb([](const char* buf) { Serial.print(buf); });
}

void initHardwares() {
  lv_st77916_init();
#if TOUCH_ENABLED
  lv_cst816_init();
#endif
}

void initDrivers() {
  display.begin();
  display.setBrightness(50);
  display.setRotation(2);

  spiffsDriver.begin('A');
}

uint64_t __lastSerialUpdateTime = 0;
void keepSerialAlive() {
  auto now = millis();
  if (now - __lastSerialUpdateTime > 5 * 1000) {
    __lastSerialUpdateTime = now;
    Serial.print("Free heap: ");
    Serial.println(esp_get_free_heap_size());
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Welcome to Tips!");
  Serial.print("Build at ");
  Serial.print(__TIME__);
  Serial.print(" ");
  Serial.println(__DATE__);  // Compile date
  Serial.println();

  initLogging();
  initHardwares();
  initDrivers();

  app = new TipApp();
  app->start();

  Serial.println("Tips is now started");
}

void loop() {
  app->update();
  display.update();

  keepSerialAlive();
}
