//#include <TimerOne.h>

#include "NTREK.h"

NTREK Ntrek(Board_ID_A101);

void setup() {
  // put your setup code here, to run once:
  Ntrek.setup(SETUP_MODE_DEFAULT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Ntrek.io_toggle(LED_GREEN);
  delay(500);
}
