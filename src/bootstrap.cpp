#include <lvgl.h>

#include "ui/App.h"

App app;

void bootstrap() {
  app.begin();
  app.start();
}
