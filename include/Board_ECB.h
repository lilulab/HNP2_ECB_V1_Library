#ifndef BOARD_ECB_H
#define BOARD_ECB_H

static const int Board_ID_A1xx = 100;
static const int Board_ID_A101 = 101;
static const int Board_ID_A102 = 102;
static const int Board_ID_A103 = 103;
static const int Board_ID_A104 = 104;
static const int Board_ID_A105 = 105;

static const int SETUP_MODE_DEFAULT = 0;

// -----------------------------------------------------------------------------
// N-Trek IO Define
// -----------------------------------------------------------------------------
static const int LED_GREEN 	= 40;
static const int LED_RED = 41;

static const int IO_RXD2 = 16;
static const int IO_TXD2 = 17;

static const int IO_SCK = 52;
static const int IO_MOSI = 51;
static const int IO_MISO = 50;
static const int IO_IMU_INT = 53;
static const int IO_BUZZER = 44;

static const int IO_3V_EN1 = 42;
static const int IO_3V_EN2 = 43;

#define POW_EN_BT IO_3V_EN1
#define POW_EN_IMU IO_3V_EN2

static const int IO_TXD1 = 18;
static const int IO_RXD1 = 19;

static const int IO_SDA = 20;
static const int IO_SCL = 21;

static const int IO_D00 = 46;
static const int IO_D01 = 45;
static const int IO_D02 = 12;
static const int IO_D03 = 13;

static const int IO_D04 = 10;
static const int IO_D05 = 11;
static const int IO_D06 = 8;
static const int IO_D07 = 9;

static const int IO_D08 = 6;
static const int IO_D09 = 7;
static const int IO_D10 = 2;
static const int IO_D11 = 3;

static const int IO_D12 = 4;
static const int IO_D13 = 5;

static const int IO_TXD3 = 14;
static const int IO_RXD3 = 16;

static const int IO_ADC00 = 0;
static const int IO_ADC01 = 1;
static const int IO_ADC02 = 2;
static const int IO_ADC03 = 3;
static const int IO_ADC04 = 4;
static const int IO_ADC05 = 5;
static const int IO_ADC06 = 6;
static const int IO_ADC07 = 7;
static const int IO_ADC08 = 8;
static const int IO_ADC09 = 9;
static const int IO_ADC10 = 10;
static const int IO_ADC11 = 11;
static const int IO_ADC12 = 12;
static const int IO_ADC13 = 13;
static const int IO_ADC14 = 14;
static const int IO_ADC15 = 15;

#endif