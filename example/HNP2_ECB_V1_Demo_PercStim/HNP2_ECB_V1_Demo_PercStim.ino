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

// Value for AA's walking pattern
#define NUM_CHANNELS 8
#define SAMPLE_PER_CYCLE 6
uint8_t AA_pulse_width[NUM_CHANNELS] = {0,0,0,0,0,0,0,0};
uint8_t AA_amplitude[NUM_CHANNELS] = {20,20,20,20,20,20,20,0};

uint8_t AA_Step_L[NUM_CHANNELS][SAMPLE_PER_CYCLE] = 
  {
    {0,   0,   0,   0,   0,   0}, //CH0
    {250, 250, 0,   0,   0,   0}, //CH1
    {0,   0,   0,   0,   0,   0}, //CH2
    {0,   0,   0,   0,   0,   0}, //CH3
    {250, 250, 250, 0,   0,   0}, //CH4
    {0,   250, 250, 0,   0,   0}, //CH5
    {0,   0,   250, 250, 0,   0}, //CH6
    {0,   0,   0,   0,   0,   0}, //CH7
  };

  uint8_t AA_Step_R[NUM_CHANNELS][SAMPLE_PER_CYCLE] = 
  {
    {0,   0,   0,   0,   0,   0}, //CH0
    {0,   250, 250, 0,   0,   0}, //CH1
    {0,   250, 250, 0,   0,   0}, //CH2
    {0,   0,   0,   0,   0,   0}, //CH3
    {200, 200, 200, 0,   0,   0}, //CH4
    {0,   0,   0,   0,   0,   0}, //CH5
    {250, 250, 0,   0,   0,   0}, //CH6
    {0,   0,   0,   0,   0,   0}, //CH7
  };

void setup() {
  // Setup the board hardware
  ECB.setup(SETUP_MODE_DEFAULT);

  // Turn off 3.3v Power for module slots.
  ECB.io_set(IO_3V_EN1, LOW);
  ECB.io_set(IO_3V_EN2, LOW);

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

  // // Comment out between Start-End to disable this code block.
  // // Start - Sweep 4 channels.
  // DemoRunSweep4CH(50);  //Sweep 4 channels.
  // // End - Sweep 4 channels.

  // AA's Left Step Pattern Generator
  DemoRunAAStepL(2000);

}

// Generate periodic sweep function
int8_t SweepValUpdate(int8_t val, int8_t dir, int8_t min, int8_t max) {
  // Check if out of the min max boundary, if so reverse direction
  if((val + dir) > max | (val + dir) < min) { dir = -dir; } 
  // Add or Sub based on the dir.
  val+=dir; 

  return val;
}

int8_t DemoRunSweep4CH(unsigned long delay_ms) {
  for (uint8_t i=0; i<4; i++) {
    pulse_width[i] = (uint8_t)SweepValUpdate((uint8_t)pulse_width[i], pw_dir[i], pw_min, pw_max); // update the Pulse width value
    Stim_Perc.cmd_set_evnt(i, pulse_width[i], amplitude[i], 0); // Change Event 4 for port_chn_id 3 in sched_id 1  
    delay(delay_ms); // delay ms
  }
  return 1;
}

int8_t DemoRunAAStepL(unsigned long step_duration_ms) {
  // Calculate delay time between channels.
  unsigned long delay_between_channels = (step_duration_ms/NUM_CHANNELS)/SAMPLE_PER_CYCLE;
  
  for (uint8_t j=0; j<SAMPLE_PER_CYCLE; j++) {
    for (uint8_t i=0; i<NUM_CHANNELS; i++) {
      AA_pulse_width[i] = AA_Step_L[i][j];// update the Pulse width value
      Stim_Perc.cmd_set_evnt(i, AA_pulse_width[i], AA_amplitude[i], 0); // Change Event 4 for port_chn_id 3 in sched_id 1  
      
      delay(delay_between_channels); // delay ms
    }
  }
  return 1;
}



// void setupCwruStim(void) {
//   Stim_Perc.init(STIM_MODE_PERC); // Initialize the Stim board and delete old schedule
//   Stim_Perc.config(STIM_SETTING_DEFAULT); // Setup channels, schedule, and events
//   Stim_Perc.start(UECU_SYNC_MSG); // Send start command (Sync message)
// }
