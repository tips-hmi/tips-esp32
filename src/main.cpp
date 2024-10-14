#include <Arduino.h>
#include <SPIFFS.h>

// LVGL
#include <lvgl.h>

// Always include `device_conf.h` first
#include "device_conf.h"

// Hardwares
#include "hardwares/cst816/cst816_lv_8.h"
#include "hardwares/st77916/st77916_lv_8.h"

// Drivers
#include "drivers/display/TouchDisplay.h"
#include "drivers/spiffs-driver/SPIFFSDriver.h"

// App
#include "ui/App.h"

TouchDisplay display;
SPIFFSDriver spiffsDriver;
TipApp* app = nullptr;

void mx_setup();
void mx_loop();

void initLogging() {
  lv_log_register_print_cb([](const char* buf) { Serial.print(buf); });
}

void initHardwares() {
  lv_st77916_init();
  lv_cst816_init();
}

void initDrivers() {
  display.begin();
  display.setBrightness(50);
  display.setRotation(3);

  spiffsDriver.begin('A');
}

uint64_t __lastSerialUpdateTime = 0;
void keepSerialAlive() {
  auto now = millis();
  if (now - __lastSerialUpdateTime > 5 * 1000) {
    __lastSerialUpdateTime = now;
    Serial.println(esp_get_free_heap_size());
  }
}

void setup() {
  delay(200);

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
