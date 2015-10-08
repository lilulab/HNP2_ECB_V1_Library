/*
  HNP2_ECB_V1_Demo_PercStim.ino - Example code for HNPv2 Project ECB NTREK Library.
  NTREK - Neuromuscular Training and Rehabilitation Embedded Kit.
  Created by Lu Li (lxl361@case), Sep 2015.
  Version 1.0
  Online Doc: https://sites.google.com/a/case.edu/n-trek-wiki
  Repo: https://github.com/lilulab/HNP2_ECB_V1_Library
*/

#include <NTREK.h>
#include <CwruStim.h>
//#include <TimerOne.h>

// Create object ECB
NTREK ECB(Board_ID_A101);

// Create object Stim_Perc
Stim Stim_Perc(STIM_CHANNEL_UART1);
//Stim Stim_Perc(STIM_CHANNEL_UART3);

// main loop counter
int32_t counter_main_loop = 0;

// Pulse width and Amplitude value
uint8_t pulse_width[4] = {0,0,0,0};
uint8_t amplitude[4] = {0x10, 0x15, 0x20, 0x26};

// Value for sweep pulse width;
int8_t pw_dir[4] = {1,2,3,4}; 
int8_t pw_min = 0x00;
int8_t pw_max = 0xFA;

void setup() {
  // Setup the board hardware
  ECB.setup(SETUP_MODE_DEFAULT);

  // Setup CwruStim library
  // setupCwruStim(); // One function setup everyting to replace three lines below.
  Stim_Perc.init(STIM_MODE_PERC); // Initialize the Stim board and delete old schedule
  Stim_Perc.config(STIM_SETTING_DEFAULT); // Setup channels, schedule, and events
  Stim_Perc.start(UECU_SYNC_MSG); // Send start command (Sync message)

}

void loop() {
  //accumulate main loop counter
  // Add one per loop()
  counter_main_loop++;

  // Toggle LED to show system is running
  ECB.io_toggle(LED_GREEN);
  //ECB.io_toggle(LED_RED);

  // Change Event Params on the fly.
  //Stim_Perc.cmd_set_evnt(event_id, pulse_width, amplitude, zone);

  // Comment out between Start-End to disable this code block.
  // Start - Sweep all 4 channels.
  // /*
  for (uint8_t i=0; i<4; i++) {
    pulse_width[i] = (uint8_t)SweepValUpdate((uint8_t)pulse_width[i], pw_dir[i], pw_min, pw_max); // update the Pulse width value
    Stim_Perc.cmd_set_evnt(i, pulse_width[i], amplitude[i], 0); // Change Event 4 for port_chn_id 3 in sched_id 1  
    delay(50); // delay 50ms
  }

  // */
  // Start - Sweep all 12 channels.

  // Comment out between Start-End to disable this code block.
  // Start - Reproduce Jeremy's Processing example code behaviers.
  /*
  pulse_width[0] = (uint8_t)SweepValUpdate((uint8_t)pulse_width[0], pw_dir[0], pw_min, pw_max); // update the Pulse width value
  Stim_Perc.cmd_set_evnt(1, pulse_width[0], amplitude[0], 0); // Change Event 1 for port_chn_id 0 in sched_id 1
  delay(50); // delay 50ms  

  pulse_width[1] = (uint8_t)SweepValUpdate((uint8_t)pulse_width[1], pw_dir[1], pw_min, pw_max); // update the Pulse width value
  Stim_Perc.cmd_set_evnt(2, pulse_width[1], amplitude[1], 0); // Change Event 2 for port_chn_id 1 in sched_id 1
  delay(50); // delay 50ms  

  pulse_width[2] = (uint8_t)SweepValUpdate((uint8_t)pulse_width[2], pw_dir[2], pw_min, pw_max); // update the Pulse width value
  Stim_Perc.cmd_set_evnt(3, pulse_width[2], amplitude[2], 0); // Change Event 3 for port_chn_id 2 in sched_id 1
  delay(50); // delay 50ms  

  pulse_width[3] = (uint8_t)SweepValUpdate((uint8_t)pulse_width[3], pw_dir[3], pw_min, pw_max); // update the Pulse width value
  Stim_Perc.cmd_set_evnt(4, pulse_width[3], amplitude[3], 0); // Change Event 4 for port_chn_id 3 in sched_id 1
  delay(50); // delay 50ms  
  */
  // End - Reproduce Jeremy's Processing example code behaviers.

}

// Generate periodic sweep function
int8_t SweepValUpdate(int8_t val, int8_t dir, int8_t min, int8_t max) {
  // Check if out of the min max boundary, if so reverse direction
  if((val + dir) > max | (val + dir) < min) { dir = -dir; } 
  // Add or Sub based on the dir.
  val+=dir; 

  return val;
}

// void setupCwruStim(void) {
//   Stim_Perc.init(STIM_MODE_PERC); // Initialize the Stim board and delete old schedule
//   Stim_Perc.config(STIM_SETTING_DEFAULT); // Setup channels, schedule, and events
//   Stim_Perc.start(UECU_SYNC_MSG); // Send start command (Sync message)
// }
