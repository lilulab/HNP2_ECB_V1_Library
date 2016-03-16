//#include <TimerOne.h>

#include "NTREK.h"

NTREK NTREK(Board_ID_A101);

void setup() {
  // put your setup code here, to run once:
  NTREK.setup(SETUP_MODE_DEFAULT);
}

void loop() {
  // put your main code here, to run repeatedly:
  NTREK.io_toggle(LED_GREEN);
  NTREK.io_toggle(LED_RED);
  delay(500);
}
