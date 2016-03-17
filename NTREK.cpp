/*
  NTREK.cpp - Library for HNPv2 Project ECB NTREK Library.
  NTREK - Neuromuscular Training and Rehabilitation Embedded Kit.
  Created by Lu Li (lxl361@case), Sep 2015.
  Version 1.0
  Online Doc: https://sites.google.com/a/case.edu/n-trek-wiki
  Repo: https://github.com/lilulab/HNP2_ECB_V1_Library
*/

#include "Arduino.h"
#include "NTREK.h"

int NTREK::_board_id = 0;
int NTREK::_setup_mode = 0;
bool NTREK::_io_state[54];

NTREK::NTREK(int board_id) {
	_board_id = board_id;
}

int NTREK::setup(int setup_mode) {

	// 3.3V Power Reg for Modular Slots.
	pinMode(IO_3V_EN1, OUTPUT);
  pinMode(IO_3V_EN2, OUTPUT);
    
  // Turn it off for 1sec.
  // Just in case need to reflash the IC.
  // This is critical since, 
  // USB-UART shares the same port with Bluetooth.
  this->io_set(POW_EN_BT, LOW);
  this->io_set(POW_EN_IMU, LOW);
  // digitalWrite(IO_3V_EN1, LOW);
  // digitalWrite(IO_3V_EN2, LOW);

  // LED on board
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  // Trun LED on at start
  this->io_set(LED_GREEN, LED_ON);  
  this->io_set(LED_RED, LED_ON);  
  // digitalWrite(LED_GREEN,LED_ON);
  // digitalWrite(LED_RED,LED_ON);

  // delay 1s, turn off both LED in the middle of 1s
  delay(500);
  this->io_set(LED_GREEN, LED_OFF);  
  this->io_set(LED_RED, LED_OFF);
  delay(500);
  
  // Now turn 3.3v regs on to power the Modular Slots.

  // digitalWrite(IO_3V_EN1, HIGH);
  // digitalWrite(IO_3V_EN2, HIGH);

  // Digital IOs setup.
  pinMode(IO_D00, OUTPUT);
  pinMode(IO_D01, OUTPUT);
  pinMode(IO_D02, OUTPUT);
  pinMode(IO_D03, OUTPUT);
  pinMode(IO_D04, OUTPUT);
  pinMode(IO_D05, OUTPUT);

  pinMode(IO_D06, OUTPUT);
  pinMode(IO_D07, OUTPUT);
  pinMode(IO_D08, OUTPUT);
  pinMode(IO_D09, OUTPUT);
  
  pinMode(IO_D10, INPUT); //FS_STOP 
    digitalWrite(IO_D10, HIGH);// turn on pullup resistors

  pinMode(IO_D11, INPUT); //FS_GO  
    digitalWrite(IO_D11, HIGH);// turn on pullup resistors


  pinMode(IO_D12, OUTPUT);
  
  pinMode(IO_D13, INPUT);   //FS_STIM_OFF   
    digitalWrite(IO_D13, HIGH);// turn on pullup resistors
  
  // ADC ref source
  analogReference(EXTERNAL);// (DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, or EXTERNAL)
  
  // Turn on Green LED
  this->io_set(LED_GREEN, LED_ON);  

  // Serial UART ports
  Serial.begin(115200); // Bluetooth and USB

  Serial2.begin(115200); // IMU-X subsystem

  // Serial1.begin(9600);  // Stim Channel
  // Serial3.begin(9600);  // Stim Channel


  // Save setup mode
	_setup_mode = setup_mode;

	return 1;
}

void NTREK::io_set(int io_pin, int io_val) {
	digitalWrite(io_pin,io_val);
  // Save to io states array
  _io_state[io_pin] = io_val;
}

bool NTREK::io_toggle(int io_pin) {
  // Flip the io_val;
  _io_state[io_pin] = !_io_state[io_pin];
  digitalWrite(io_pin,_io_state[io_pin]);

  return _io_state[io_pin];
}

int NTREK::imu_init(int mode) {
  // turn on power to IMU-X subsystem
  this->io_set(POW_EN_IMU, HIGH);

  // calibrate sensor if needed
  imu_kf_roll_offset = 0;
  imu_kf_pitch_offset = 0;
  imu_kf_yaw_offset = 0;

  for (int i=0; i<4; i++) {
    imu_quaternion[i] = -1;
  } // end for i

  // this->imu_calibration(mode);

  return 1;
}

int NTREK::imu_turn_off(void) {
  // turn off power to IMU-X subsystem
  this->io_set(POW_EN_IMU, HIGH);

  return 1;
}

int NTREK::imu_update(int mode) {

  switch (mode) {
    case  IMU_MODE_FLTR_KF_BIN:
      //Kalman Filtered
      Serial2.write((int) IMU_MODE_FLTR_KF_BIN);

      // check incoming
      if(Serial2.available()) {
        // read data from UART2
        Serial2.readBytes((char*)&imu_data_kf, IMU_KF_DATA_BUFFER_LENGTH);

        // process the date
        imu_kf_roll = (int16_t)imu_data_kf[3]*255 + (int16_t)imu_data_kf[4] - imu_kf_roll_offset;
        imu_kf_pitch = (int16_t)imu_data_kf[5]*255 + (int16_t)imu_data_kf[6] - imu_kf_pitch_offset;
        //imu_kf_yaw;
      }

      break;

    // For AHRS 9-Axis fusion firmware.
    case IMU_MODE_9AXIS_FUSION:
      //AHRS 9-Axis fusion

      if (Serial2.availableForWrite()>=60) {
        Serial2.write((int) IMU_MODE_9AXIS_FUSION);
      }

      // check incoming
      if(Serial2.available()) {

        // read data from UART2
        Serial2.readBytes((char*)&imu_quaternion_msg, IMU_MODE_9AXIS_FUSION_DATA_LENGTH);

        // calculate checksum byte
        uint8_t checkSumByte = this->checksum(imu_quaternion_msg, IMU_MODE_9AXIS_FUSION_DATA_LENGTH-1);

        // only if checksum is correct
        if (checkSumByte == imu_quaternion_msg[IMU_MODE_9AXIS_FUSION_DATA_LENGTH-1]) {
          // unpack the quaternion data from message
          for (int i=0; i<4; i++) {
            imu_quaternion[i] = ( ( (int16_t)imu_quaternion_msg[i*2+1] << 8 ) & 0xFF00 ) | ((int16_t)imu_quaternion_msg[i*2+2] & 0x00FF );
          } // end for i
        } else { // check sum not meet
          // TODO : error handle
          for (int i=0; i<4; i++) {
            imu_quaternion[i] = -1;
          } // end for i
        } // end if checksum


      } // end if Serial 2

      break;

    default:
      // TODO
      // Error handling
      return -1;
      break;
  }

  return 1;
}

int NTREK::imu_calibration(int mode) {

  imu_kf_roll_offset = 0;
  imu_kf_pitch_offset = 0;
  //get data update
  this->imu_update(mode);

  // TODO
  // multi sample to filtering the offset

  switch (mode) {
    case  IMU_MODE_FLTR_KF_BIN:
      // use current value as offset.
      imu_kf_roll_offset = imu_kf_roll;
      imu_kf_pitch_offset = imu_kf_pitch;
      break;

    default:
      // TODO
      // Error handling
      return -1;
      break;
  }

  return 1;
}

uint8_t NTREK::checksum(uint8_t vals[], int length){
  uint16_t csum = 0;
  for(int i=0; i<length-1; i++) {
    csum += (uint16_t)vals[i];
  }
  csum = ((0x00FF & csum) + (csum >> 8))^0xFF;
  return (uint8_t)csum;
} 


