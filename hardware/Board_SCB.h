#ifndef BOARD_SCB_H
#define BOARD_SCB_H

// Signal Conditioning Board Remap
#define SB2_TXD IO_TXD3;
#define FSR_HEL_L IO_ADC00;
#define FSR_HEL_L IO_ADC02;
#define JNT_HIP_L IO_ADC04;
#define JNT_KNE_L IO_ADC06;
#define PRS_CH1 IO_ADC08;
#define PRS_CH3 IO_ADC10;
#define PRS_CH5 IO_ADC12;
#define PRS_CH7 IO_ADC14;

#define SB2_RXD IO_RXD3;
#define FSR_FFT_L IO_ADC01;
#define FSR_FFT_R IO_ADC03;
#define JNT_HIP_R IO_ADC05;
#define JNT_KNE_R IO_ADC07;
#define PRS_CH2 IO_ADC09;
#define PRS_CH4 IO_ADC11;
#define PRS_CH6 IO_ADC13;
#define PRS_CH8 IO_ADC15;

#define BA1_VTE IO_ADC10;
#define BA2_VTE IO_ADC11;

#endif