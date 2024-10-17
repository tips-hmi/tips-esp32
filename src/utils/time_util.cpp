#include "time_util.h"

const String formatTime() {
  time_t now = time(nullptr);
  struct tm *timeInfo = localtime(&now);
  int hour = (timeInfo->tm_hour + 8) % 24;  // Adjust for UTC+8
  int min = timeInfo->tm_min;
  return (hour < 10 ? "0" : "") + String(hour) + ":" + (min < 10 ? "0" : "") +
         String(min);
}
