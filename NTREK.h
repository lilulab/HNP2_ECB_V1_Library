/*
  NTREK.h - Library for HNPv2 Project ECB NTREK Library.
  NTREK - Neuromuscular Training and Rehabilitation Embedded Kit.
  Created by Lu Li (lxl361@case), Sep 2015.
  Version 2.1
  Online Doc: https://sites.google.com/a/case.edu/n-trek-wiki
  Repo: https://github.com/lilulab
*/

#ifndef NTREK_H
#define NTREK_H

#include "Arduino.h"
//#include <TimerOne.h>
#include "NTREK_Board.h"

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
  private:
    static int _board_id;
    static int _setup_mode;
    static bool _io_state[54];
};

#endif

