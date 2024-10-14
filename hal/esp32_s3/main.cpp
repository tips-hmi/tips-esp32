#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <lvgl.h>

// Always include `device_conf.h` first
#include "device_conf.h"

// Hardwares
#include "hardwares/cst816/cst816_lv_8.h"
#include "hardwares/st77916/st77916_lv_8.h"

// Drivers
#include "drivers/display/TouchDisplay.h"
#include "drivers/spiffs-driver/SPIFFSDriver.h"

// HAL Implementations
#include "wifi/WiFiConnection.h"

TouchDisplay display;
SPIFFSDriver spiffsDriver;

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
  display.setBrightness(66);
  display.setRotation(1);

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
  Serial.println("Welcome to Tips");

  initLogging();
  initHardwares();
  initDrivers();

  mx_setup();

  Serial.println("Tips is now started");
}

void loop() {
  mx_loop();
  display.update();

  keepSerialAlive();
}
