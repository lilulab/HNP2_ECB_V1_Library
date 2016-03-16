//#include <TimerOne.h>

#include "NTREK.h"

NTREK NTREK(Board_ID_A1xx);

void setup() {
  // put your setup code here, to run once:
  NTREK.setup(SETUP_MODE_DEFAULT);

  // NTREK.imu_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  NTREK.io_toggle(LED_GREEN);
  //NTREK.io_toggle(LED_RED);

  // read imu subsystem data
  NTREK.imu_update(IMU_MODE_FLTR_KF_BIN);

  // print out to debug port
  //readable strings.
  Serial.print("IMU: ");  
  for (int i=0; i<IMU_KF_DATA_BUFFER_LENGTH; i++) {
    Serial.print(NTREK.imu_data_kf[i], DEC);
    Serial.print(",\t");
  }

  Serial.println(".");

  //delay(10);
}
