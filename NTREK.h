#ifndef NTREK_H
#define NTREK_H

// -----------------------------------------------------------------------------
// Arduino Library
// -----------------------------------------------------------------------------

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
 
#endif
	
// -----------------------------------------------------------------------------
// N-Trek IO Define
// -----------------------------------------------------------------------------
#define LED_GREEN 40
#define LED_RED 41

#define IO_RXD2 16
#define IO_TXD2 17

#define IO_SCK 52
#define IO_MOSI 51
#define IO_MISO 50
#define IO_IMU_INT 53
#define IO_BUZZER 44

#define IO_3V_EN1 42
#define IO_3V_EN2 43

#define IO_TXD1 18
#define IO_RXD1 19

#define IO_SDA 20
#define IO_SCL 21

#define IO_D00 46
#define IO_D01 45
#define IO_D02 12
#define IO_D03 13

#define IO_D04 10
#define IO_D05 11
#define IO_D06 8
#define IO_D07 9

#define IO_D08 6
#define IO_D09 7
#define IO_D10 2
#define IO_D11 3

#define IO_D12 4
#define IO_D13 5

#define IO_TXD3 14
#define IO_RXD3 16

#define IO_ADC00	0
#define	IO_ADC01	1
#define	IO_ADC02	2
#define	IO_ADC03	3
#define	IO_ADC04	4
#define	IO_ADC05	5
#define	IO_ADC06	6
#define	IO_ADC07	7
#define	IO_ADC08	8
#define	IO_ADC09	9
#define	IO_ADC10	10
#define	IO_ADC11	11
#define	IO_ADC12	12
#define	IO_ADC13	13
#define	IO_ADC14	14
#define	IO_ADC15	15

// Signal Conditioning Board Remap

#define SB2_TXD  IO_TXD3
#define FSR_HEL_L  IO_ADC00
#define FSR_HEL_L  IO_ADC02
#define JNT_HIP_L  IO_ADC04
#define JNT_KNE_L  IO_ADC06
#define PRS_CH1  IO_ADC08
#define PRS_CH3  IO_ADC10
#define PRS_CH5  IO_ADC12
#define PRS_CH7  IO_ADC14

#define SB2_RXD  IO_RXD3
#define FSR_FFT_L  IO_ADC01
#define FSR_FFT_R  IO_ADC03
#define JNT_HIP_R  IO_ADC05
#define JNT_KNE_R  IO_ADC07
#define PRS_CH2  IO_ADC09
#define PRS_CH4  IO_ADC11
#define PRS_CH6  IO_ADC13
#define PRS_CH8  IO_ADC15

#define BA1_VTE  IO_ADC10
#define BA2_VTE  IO_ADC11

#endif

