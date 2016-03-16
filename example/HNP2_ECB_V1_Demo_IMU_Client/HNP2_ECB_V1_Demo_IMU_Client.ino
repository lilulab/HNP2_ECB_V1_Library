#include "NTREK.h"

NTREK NTREK(Board_ID_A1xx);

void setup() {
  // put your setup code here, to run once:
  NTREK.setup(SETUP_MODE_DEFAULT);

  // Kick start the IMU-X subsystem
  NTREK.imu_init(IMU_MODE_FLTR_KF_BIN);

}

void loop() {
  // put your main code here, to run repeatedly:
  NTREK.io_toggle(LED_GREEN);
  //NTREK.io_toggle(LED_RED);

  // read imu subsystem data
  NTREK.imu_update(IMU_MODE_FLTR_KF_BIN);

  if(Serial.available()) {
    // if send command string 'C' via Serial Monitor
    if (Serial.read() == 'C') {
      // Run Kalman Filter calibration.
      NTREK.imu_calibration(IMU_MODE_FLTR_KF_BIN); 
    }
  }

  // print out the data via Serial Monitor
  debug_print();

  //delay(10);
}

void debug_print(void) {
  // print out to debug port
  //readable strings.
  Serial.print("IMU: "); 

  // print raw data buffer
  // for (int i=0; i<IMU_KF_DATA_BUFFER_LENGTH; i++) {
  //   Serial.print(NTREK.imu_data_kf[i], DEC);
  //   Serial.print(",\t");
  // }

  // roll angle (in degree*100)
  Serial.print(" kf_roll = ");
  Serial.print(NTREK.imu_kf_roll);
  Serial.print("(int) / ");
  Serial.print((float)NTREK.imu_kf_roll/100);
  Serial.print("(deg);\t");

  // pitch angle (in degree*100)
  Serial.print(" kf_pitch = ");
  Serial.print(NTREK.imu_kf_pitch);
  Serial.print("(int) / ");
  Serial.print((float)NTREK.imu_kf_pitch/100);
  Serial.print("(deg);\t");


  // // roll offset (in degree*100)
  // Serial.print(" roll_offset = ");
  // Serial.print(NTREK.imu_kf_roll_offset, DEC);
  // Serial.print("(int);\t");

  // // pitch offset (in degree*100)
  // Serial.print(" pitch_offset = ");
  // Serial.print(NTREK.imu_kf_pitch_offset, DEC);
  // Serial.print("(int);\t");

  Serial.println("");
}
