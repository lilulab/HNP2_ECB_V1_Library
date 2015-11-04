/*
  HNP2_ECB_V1_Demo_VCK5.ino - Example code for HNPv2 Project ECB NTREK Library.
  NTREK - Neuromuscular Training and Rehabilitation Embedded Kit.
  Created by Lu Li (lxl361@case), Sep 2015.
  Version 1.0
  Online Doc: https://sites.google.com/a/case.edu/n-trek-wiki
  Repo: https://github.com/lilulab/HNP2_ECB_V1_Library
*/

#include <NTREK.h>
#include <CwruStim.h>
#include "StimPattern_VCK5.h"
#include <TimerOne.h>


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

// Value for VCK5's walking pattern
#define NUM_CHANNELS 12
#define NUM_BOARDS 2
#define GAIT_CYCLE_STEP_MAX 8

#define STEP_DURATION 1000

uint8_t VCK5_pulse_width_zeros[NUM_BOARDS][NUM_CHANNELS] = 
    
  {
    {0,0,0,0, 0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0}
  };

uint8_t VCK5_amplitude[NUM_BOARDS][NUM_CHANNELS] = 
  {
    {38,38,38,38,  38,38,38,38,  0,0,0,0},
    {38,38,38,38,  38,38,38,38,  0,0,0,0}
  };

uint8_t VCK5_channel_delays[NUM_CHANNELS] = 
    {2,4,6,8,  10,12,14,16,  18,20,22,24};

// Value for VCK5's walking pattern
#define NUM_CHANNELS 12
#define NUM_BOARDS 2
#define SAMPLE_PER_CYCLE 6

// Finger switch states machine
#define FSSM_STATE_START  1 
#define FSSM_STATE_STAND  2 
#define FSSM_STATE_LSETP  3 
#define FSSM_STATE_RSETP  4 
#define FSSM_STATE_SIT    5 

#define FSSM_EVENT_NONE       0 
#define FSSM_EVENT_PRESS_GO   1 
#define FSSM_EVENT_PRESS_STOP 2 

#define FSSM_RESULT_NULL            -1
#define FSSM_RESULT_NONE            0
#define FSSM_RESULT_NO_STIM         1 
#define FSSM_RESULT_EXE_STAND  2
#define FSSM_RESULT_EXE_LSTEP  3
#define FSSM_RESULT_EXE_RSTEP  4
#define FSSM_RESULT_EXE_SIT         5

// Finger switch state machine vars.
int8_t finger_switch_state = FSSM_STATE_START;
int8_t finger_switch_event = FSSM_EVENT_NONE;
int8_t finger_switch_output = FSSM_RESULT_NONE;

// Timer one
static const int16_t TIMER_SEC_MAX = 60;
static const int16_t TIMER_MS_MAX = 1000;
int16_t timer1_sec_counter = 0;
int16_t timer1_ms_counter = 0;


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

  Serial.begin(115200);

  Timer1.initialize(1000); // set a timer of length 1000 microseconds (or 0.001 sec - or 1kHz)
  Timer1.attachInterrupt( timerOneIsr ); // attach the service routine here

}

void loop() {
  //accumulate main loop counter
  // Add one per loop()
  counter_main_loop++;

  // Toggle LED to show system is running
  //ECB.io_toggle(LED_GREEN);
  //ECB.io_toggle(LED_RED);

  // Change Event Params on the fly.
  //Stim_Perc.cmd_set_evnt(event_id, pulse_width, amplitude, zone);

  // // Comment out between Start-End to disable this code block.
  // // Start - Sweep 4 channels.
  // DemoRunSweep4CH(50);  //Sweep 4 channels.
  // // End - Sweep 4 channels.

  ECB.io_set(LED_GREEN, LED_OFF);
  ECB.io_set(LED_RED, LED_OFF);

  while(1){
    if (digitalRead(FS_GO) == LOW) {
      delay(10);
      if (digitalRead(FS_GO) == LOW) {
        finger_switch_output = FSSM_run_once(FSSM_EVENT_PRESS_GO);
        RunPercStimOnce(finger_switch_output, STEP_DURATION/GAIT_CYCLE_STEP_MAX);
      }
    } else if (digitalRead(FS_STOP) == LOW) {
      delay(10);
      if (digitalRead(FS_STOP) == LOW) {
        finger_switch_output = FSSM_run_once(FSSM_EVENT_PRESS_STOP);
      }
    }
    FSSM_print();
    
  }



}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerOneIsr()
{
    timer1_ms_counter++; // increment millisec counter
    if (timer1_ms_counter >= TIMER_MS_MAX) {
      timer1_ms_counter = 0; // reset

      timer1_sec_counter ++; // increment second counter
      if (timer1_sec_counter >= TIMER_SEC_MAX) {
        timer1_sec_counter = 0; // reset
        // Increment miniutes counter 
      }

    }
    // Toggle LED
    ECB.io_toggle(LED_GREEN);
}

void timerOneClear (void) {
  Timer1.stop();
  timer1_ms_counter = 0;
  timer1_sec_counter = 0;
}

void timerOneRestart (void) {
  timerOneClear();
  Timer1.resume();
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

int8_t RunPercStimOnce(int8_t gait_type, unsigned long delay_ms) {

  uint8_t pulse_width_i = 0;
  uint8_t amplitude_i = 0;

  int8_t gait_cycle_step = 0;

  // loop all gait cycle steps
  for (gait_cycle_step = 0; gait_cycle_step < GAIT_CYCLE_STEP_MAX; gait_cycle_step++ ) {

    // loop all channels
    for (uint8_t i=0; i<NUM_CHANNELS; i++) {
      switch (gait_type) {
        case FSSM_RESULT_NO_STIM:
          pulse_width_i = VCK5_pulse_width_zeros[1][i];
        break;

        case FSSM_RESULT_EXE_STAND:
          pulse_width_i = VCK5_pulse_width_zeros[1][i];
        break;

        case FSSM_RESULT_EXE_LSTEP:
          pulse_width_i = gait_walk_L_B1_PW[i][gait_cycle_step];
          amplitude_i = VCK5_amplitude[1][i];

        break;

        case FSSM_RESULT_EXE_RSTEP:
          pulse_width_i = gait_walk_R_B1_PW[i][gait_cycle_step];
          amplitude_i = VCK5_amplitude[1][i];

        break;

        case FSSM_RESULT_EXE_SIT:
          pulse_width_i = VCK5_pulse_width_zeros[1][i];
        break; 
      }

        Serial.print("PercStim_state: ");
        Serial.print(gait_type);
        Serial.print(", GaitCycle =");
        Serial.print(gait_cycle_step);
        Serial.print(", CH");
        Serial.print(i);
        Serial.print(", PW=");
        Serial.print(pulse_width_i);
        Serial.print(", AMP=");
        Serial.print(amplitude_i);
        Serial.println(";");
        
      Stim_Perc.cmd_set_evnt(i, pulse_width_i, amplitude_i, 0); // Change Event 4 for port_chn_id 3 in sched_id 1  
      //delay(10);
    }// end channels loop
    delay(delay_ms); // delay ms
  } // end cycle loop
  
  return 1;  
}

int8_t FingerSwitchStatesMachine(int8_t * current_state, int8_t trigger_event) {
  // states machine
  switch (*current_state) {
    case FSSM_STATE_START:
      if (trigger_event == FSSM_EVENT_PRESS_GO) {
        *current_state = FSSM_STATE_STAND; // if press go, then exe stand.
        return FSSM_RESULT_EXE_STAND;
      } else if(trigger_event == FSSM_EVENT_NONE) {
        return FSSM_RESULT_NONE; // do noting
      } else {
        return FSSM_RESULT_NULL; // return null
      }
    break;

    case FSSM_STATE_STAND:
      if (trigger_event == FSSM_EVENT_PRESS_GO) {
        *current_state = FSSM_STATE_LSETP; // if press go, then exe Lstep.
        return FSSM_RESULT_EXE_LSTEP;
      } else if(trigger_event == FSSM_EVENT_PRESS_STOP) {
        *current_state = FSSM_STATE_SIT;  // if press stop, then exe sit.
        return FSSM_RESULT_EXE_SIT;
      } else if(trigger_event == FSSM_EVENT_NONE) {
        return FSSM_RESULT_NONE; // do noting
      } else {
        return FSSM_RESULT_NULL; // return null
      }
    break;

    case FSSM_STATE_LSETP:
      if (trigger_event == FSSM_EVENT_PRESS_GO) {
        *current_state = FSSM_STATE_RSETP; // if press go, then exe Rstep.
        return FSSM_RESULT_EXE_RSTEP;
      } else if(trigger_event == FSSM_EVENT_PRESS_STOP) {
        *current_state = FSSM_STATE_SIT;  // if press stop, then exe sit.
        return FSSM_RESULT_EXE_SIT;
      } else if(trigger_event == FSSM_EVENT_NONE) {
        return FSSM_RESULT_NONE; // do noting
      } else {
        return FSSM_RESULT_NULL; // return null
      }
    break;

    case FSSM_STATE_RSETP:
      if (trigger_event == FSSM_EVENT_PRESS_GO) {
        *current_state = FSSM_STATE_LSETP; // if press go, then exe Lstep.
        return FSSM_RESULT_EXE_LSTEP;
      } else if(trigger_event == FSSM_EVENT_PRESS_STOP) {
        *current_state = FSSM_STATE_SIT;  // if press stop, then exe sit.
        return FSSM_RESULT_EXE_SIT;
      } else if(trigger_event == FSSM_EVENT_NONE) {
        return FSSM_RESULT_NONE; // do noting
      } else {
        return FSSM_RESULT_NULL; // return null
      }
    break;

    case FSSM_STATE_SIT:
      *current_state = FSSM_STATE_START;
      //return FSSM_RESULT_EXE_SIT; // don't need to exe sit since previous states did already.
    break;


  } 
}

void FSSM_led_tester(int8_t get_result) {
  switch (get_result) {
    case FSSM_RESULT_EXE_STAND:
      ECB.io_set(LED_GREEN, LED_ON);
      ECB.io_set(LED_RED, LED_ON);
      break;
    case FSSM_RESULT_EXE_LSTEP:
      ECB.io_set(LED_GREEN, LED_ON);
      ECB.io_set(LED_RED, LED_OFF);
      break;
    case FSSM_RESULT_EXE_RSTEP:
      ECB.io_set(LED_GREEN, LED_OFF);
      ECB.io_set(LED_RED, LED_ON);
      break;
    case FSSM_RESULT_EXE_SIT:
      ECB.io_set(LED_GREEN, LED_OFF);
      ECB.io_set(LED_RED, LED_OFF);
      break;
  }
}

int8_t FSSM_run_once(int8_t test_event) {
  int8_t result;

  result = FingerSwitchStatesMachine(&finger_switch_state, test_event);
  FSSM_led_tester(result);

  //delay(500);
  return result;
}

void FSSM_print(void) {
  Serial.print("FSSM_state= ");
  Serial.print(finger_switch_state);
  Serial.print(", FSSM_event= ");
  Serial.print(finger_switch_event);
  Serial.print(", FSSM_result= ");
  Serial.print(finger_switch_output);
  Serial.println(".");
}