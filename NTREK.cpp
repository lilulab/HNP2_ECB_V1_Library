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
  this->io_set(IO_3V_EN1, LOW);
  this->io_set(IO_3V_EN2, LOW);
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