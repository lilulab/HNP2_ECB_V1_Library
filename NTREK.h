/*
  NTREK.h - Library for HNPv2 Project ECB NTREK Library.
  NTREK - Neuromuscular Training and Rehabilitation Embedded Kit.
  Created by Lu Li (lxl361@case), Sep 2015.
  Version 1.0
  Online Doc: https://sites.google.com/a/case.edu/n-trek-wiki
  Repo: https://github.com/lilulab/HNP2_ECB_V1_Library
*/

#ifndef NTREK_H
#define NTREK_H

#include "Arduino.h"
//#include <TimerOne.h>
#include "include/Board_ECB.h"
#include "include/Board_SCB.h"
#include "include/IMU_Subsystem.h"

#define NOP __asm__ __volatile__ ("nop\n\t")
#define LED_ON LOW
#define LED_OFF HIGH

// -----------------------------------------------------------------------------
// Arduino Library
// -----------------------------------------------------------------------------
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
	
class NTREK
{
  public:
  	NTREK(int board_id);
  	int setup(int setup_mode);
  	void io_set(int io_pin, int io_val);
    bool io_toggle(int io_pin);
    int imu_init(int mode);
    int imu_turn_off(void);
    int imu_update(int mode);
    int imu_calibration(int mode);
    uint8_t checksum(uint8_t vals[], int length);
    
    uint8_t imu_data_kf[IMU_KF_DATA_BUFFER_LENGTH];
    int16_t imu_kf_roll;
    int16_t imu_kf_pitch;
    int16_t imu_kf_yaw;

    int16_t imu_kf_roll_offset;
    int16_t imu_kf_pitch_offset;
    int16_t imu_kf_yaw_offset;

    uint8_t imu_quaternion_msg[IMU_MODE_9AXIS_FUSION_DATA_LENGTH];
    int16_t imu_quaternion[4];

  private:
    static int _board_id;
    static int _setup_mode;
    static bool _io_state[54];

};

#endif

