#ifndef IMU_SUBSYSTEM_H
#define IMU_SUBSYSTEM_H

// For IMU-X subsystem


static const int  IMU_MODE_9AXIS_FUSION_DATA_LENGTH  =  10;
static const int  IMU_MODE_9AXIS_FUSION  =  0xA0;

static const int  IMU_MODE_RAW_STR       =  0xA1;
static const int  IMU_MODE_RAW_DER_STR   =  0xA2;

//raw pitch and roll
static const int  IMU_MODE_RPY_BIN       =  0xA3;
static const int  IMU_MODE_RPY_STR       =  0xA4;

//raw binary
static const int  IMU_MODE_RAW_BIN       =  0xA5;
static const int  IMU_MODE_RAW_DER_BIN   =  0xA6;

//Kalman Filtered
static const int  IMU_KF_DATA_BUFFER_LENGTH = 9;
static const int  IMU_MODE_FLTR_KF_BIN   =  0xA7;
static const int  IMU_MODE_FLTR_KF_STR   =  0xA8;

//Complementary  Filtered
static const int  IMU_MODE_FLTR_CPLM_BIN   =  0xA9;
static const int  IMU_MODE_FLTR_CPLM_STR   =  0xAA;

#endif