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

#define DEBUG_ON 1 //Comment this out if want to disenable all debug serial printing.
#define DEBUG_GAIT 1  //Comment this out if want to disenable gait info debug.
// #define DEBUG_TIMER 1 //Comment this out if want to disenable timer debug.
#define DEBUG_FSSM 1  //Comment this out if want to disenable finger switch debug.
// #define DEBUG_SYS 1


// Create object ECB
NTREK ECB(Board_ID_A101);

// Stim board 1 @ UART1
Stim stimBrd1(STIM_CHANNEL_UART1);
// Stim stimBrd1(STIM_CHANNEL_UART0); //only use for USB-UART debugging.

// Stim board 2 @ UART3
Stim stimBrd2(STIM_CHANNEL_UART3);

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
#define NUM_CHANNELS 8
#define NUM_BOARDS 2
#define GAIT_CYCLE_STEP_MAX 7

float STEP_DURATION_DEFAULT = 2.0;
float STEP_DURATION_L = 1.33;
float STEP_DURATION_R = 1.56;
// TODO: extract step duration from excel file.

uint8_t VCK5_pulse_width_zeros[NUM_BOARDS][NUM_CHANNELS] = 
    
  {
    {0,0,0,0, 0,0,0,0},
    {0,0,0,0, 0,0,0,0}
  }; 
 // {
 //    {0,0,0,0, 0,0,0,0, 0,0,0,0},
 //    {0,0,0,0, 0,0,0,0, 0,0,0,0}
 //  };

uint8_t VCK5_amplitude[NUM_BOARDS][NUM_CHANNELS] = 
  {
    {38,38,38,38,  38,38,38,38},
    {38,38,38,38,  38,38,38,38}
  };
  // {
  //   {38,38,38,38,  38,38,38,38,  0,0,0,0},
  //   {38,38,38,38,  38,38,38,38,  0,0,0,0}
  // };

uint8_t VCK5_channel_delays[NUM_CHANNELS] = 
    {2,4,6,8,  10,12,14,16};
    // {2,4,6,8,  10,12,14,16,  18,20,22,24};

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

boolean finger_switch_button_go_current = HIGH;
boolean finger_switch_button_stop_current = HIGH;

boolean finger_switch_button_go_old = HIGH;
boolean finger_switch_button_stop_old = HIGH;

// Timer one
// static const uint16_t TIMER_MIN_MAX = 60;
// static const uint16_t TIMER_SEC_MAX = 60;
static const uint16_t TIMER_SEC_MAX = 43200; //=60*60*12 = number of seconds in 12 hours.
static const uint16_t TIMER_MS_MAX = 1000;
// uint16_t timer1_min_counter = 0;
uint16_t timer1_sec_counter = 0;
uint16_t timer1_ms_counter = 0;
uint16_t system_100hz_counter = 0;
# define OS_RATE_HZ 100




void setup() {
  // Setup the board hardware
  ECB.setup(SETUP_MODE_DEFAULT);

  // Turn off 3.3v Power for module slots.
  ECB.io_set(IO_3V_EN1, LOW);
  ECB.io_set(IO_3V_EN2, LOW);

  // Start Serial0 for debug and BT comm.
  Serial.begin(115200);

  // Setup CwruStim library
  // Board 1 setup
  stimBrd1.init(STIM_MODE_PERC_8CH_MULTI_SCHEDULE); // Initialize the Stim board and delete old schedule
  stimBrd1.config(STIM_MODE_PERC_8CH_MULTI_SCHEDULE); // Setup channels, schedule, and events
  stimBrd1.start(UECU_SYNC_MSG);

  // Board 2 setup
  stimBrd2.init(STIM_MODE_PERC_8CH_MULTI_SCHEDULE); // Initialize the Stim board and delete old schedule
  stimBrd2.config(STIM_MODE_PERC_8CH_MULTI_SCHEDULE); // Setup channels, schedule, and events
  stimBrd2.start(UECU_SYNC_MSG);


  // Timer1.initialize(1000); // set a timer of length 1000 microseconds (or 0.001 sec - or 1kHz)
  Timer1.initialize(10000); // set a timer of length 10 miliseconds (or 0.01 sec - or 1kHz)
  Timer1.attachInterrupt( timerOneIsr ); // attach the service routine here
  Timer1.stop();
}

void loop() {
  //accumulate main loop counter
  // Add one per loop()
  counter_main_loop++;

  // Toggle LED to show system is running
  //ECB.io_toggle(LED_GREEN);
  //ECB.io_toggle(LED_RED);

  // Change Event Params on the fly.
  //stimBrd1.cmd_set_evnt(event_id, pulse_width, amplitude, zone);

  // // Comment out between Start-End to disable this code block.
  // // Start - Sweep 4 channels.
  // DemoRunSweep4CH(50);  //Sweep 4 channels.
  // // End - Sweep 4 channels.

  ECB.io_set(LED_GREEN, LED_OFF);
  ECB.io_set(LED_RED, LED_ON);

  delay(1000);
  timerOneRestart();

    ECB.io_set(LED_RED, LED_OFF);

  uint16_t system_100hz_counter_current = 0;
  uint16_t system_100hz_counter_old = 0;

  while(1) {

    system_100hz_counter_old = system_100hz_counter_current;
    system_100hz_counter_current = system_100hz_counter;

    #if defined(DEBUG_SYS) && defined(DEBUG_ON)
      Serial.print("system_100hz_counter_old: ");
      Serial.print(system_100hz_counter_old);
      Serial.print("\t");
      Serial.print("system_100hz_counter_current: ");
      Serial.println(system_100hz_counter_current);
    #endif

    // check if system update
    if (system_100hz_counter_old != system_100hz_counter_current) {
      // 1Hz tasks
      if (system_100hz_counter_current % (OS_RATE_HZ/1) == 0) {
      } //end 1Hz tasks

      // 10Hz tasks
      if (system_100hz_counter_current % (OS_RATE_HZ/10) == 0) {
        ECB.io_toggle(LED_RED);
        task_finger_switch();
      } //end 10Hz tasks

      // 30ms tasks
      if (system_100hz_counter_current % 3 == 0) {
        ECB.io_toggle(LED_GREEN);
        task_perc_stim();
      } //end 30Hz tasks

      // 100Hz tasks
      // Just put code in here.
      // ECB.io_toggle(LED_RED);

    } //end check if system update
  } // end while(1)
} //end main loop


// Task finger switch
void task_finger_switch(void) {

  // save last
  finger_switch_button_go_old = finger_switch_button_go_current;
  finger_switch_button_stop_old = finger_switch_button_stop_current;

  // get new
  finger_switch_button_go_current = digitalRead(FS_GO);
  finger_switch_button_stop_current = digitalRead(FS_STOP);

    if (finger_switch_button_go_current == LOW) {
      if (finger_switch_button_go_old == HIGH) {
        finger_switch_output = FSSM_run_once(FSSM_EVENT_PRESS_GO);
        //RunPercStimOnce(finger_switch_output, STEP_DURATION_DEFAULT);
      }
    } else if (finger_switch_button_stop_current == LOW) {
      if (finger_switch_button_stop_old == HIGH) {
        finger_switch_output = FSSM_run_once(FSSM_EVENT_PRESS_STOP);
      }
    }
    FSSM_print();
}

// Task Perc Stim
void task_perc_stim(void) {

  int8_t gait_type = finger_switch_output;

  uint8_t pulse_width_i = 0;
  uint8_t amplitude_i = 0;

  size_t gait_cycle_step[NUM_CHANNELS];
  size_t gait_cycle_next_step[NUM_CHANNELS];


  uint8_t gait_finished_count = 0;

  float current_gait_time = 0.0;
  float next_event_time = 0.0;

  switch (gait_type) {
    case FSSM_RESULT_NO_STIM:
      #if defined(DEBUG_GAIT) && defined(DEBUG_ON)
         Serial.print("[GAIT]"); Serial.println("NO_STIM"); 
      #endif

    break;

    case FSSM_RESULT_EXE_STAND:
      #if defined(DEBUG_GAIT) && defined(DEBUG_ON)
         Serial.print("[GAIT]"); Serial.println("EXE_STAND"); 
      #endif

    break;

    case FSSM_RESULT_EXE_LSTEP:
      #if defined(DEBUG_GAIT) && defined(DEBUG_ON)
         Serial.print("[GAIT]"); Serial.println("EXE_LSTEP"); 
      #endif

    break;

    case FSSM_RESULT_EXE_RSTEP:
      #if defined(DEBUG_GAIT) && defined(DEBUG_ON)
         Serial.print("[GAIT]"); Serial.println("EXE_RSTEP"); 
      #endif

    break;

    case FSSM_RESULT_EXE_SIT:
      #if defined(DEBUG_GAIT) && defined(DEBUG_ON)
         Serial.print("[GAIT]"); Serial.println("EXE_SIT"); 
      #endif

    break; 
  }

  //delay(30);
}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerOneIsr()
{
  system_100hz_counter++; // SYSTEM counter +1
  timer1_ms_counter+=10; // increment millisec counter

  if (timer1_ms_counter >= TIMER_MS_MAX) {
    timer1_ms_counter = 0; // reset

    timer1_sec_counter ++; // increment second counter
    if (timer1_sec_counter >= TIMER_SEC_MAX) {
      timer1_sec_counter = 0; // reset
      // Increment miniutes counter 
    }

  }

  #if defined(DEBUG_TIMER) && defined(DEBUG_ON)
    // Toggle LED
    //ECB.io_toggle(LED_GREEN);
    Serial.print("Timer ISR: system_100hz_counter[");
    Serial.print(system_100hz_counter);
    Serial.print("]\t");
    
    Serial.print(timer1_sec_counter);
    Serial.print("s\t");

    Serial.print(timer1_ms_counter);
    Serial.println("ms\t");
  #endif
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
    stimBrd1.cmd_set_evnt(i+1, pulse_width[i], amplitude[i], 0); // Change Event 4 for port_chn_id 3 in sched_id 1  
    delay(delay_ms); // delay ms
  }
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
        return FSSM_RESULT_NONE; // do nothing
        //return FSSM_RESULT_NULL; // return null
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
        *current_state = FSSM_STATE_STAND;  // if press stop, then exe sit.
        return FSSM_RESULT_EXE_STAND;
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
        *current_state = FSSM_STATE_STAND;  // if press stop, then exe sit.
        return FSSM_RESULT_EXE_STAND;
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
  //FSSM_led_tester(result);

  //delay(500);
  return result;
}

void FSSM_print(void) {
  #if defined(DEBUG_FSSM) && defined(DEBUG_ON)

    Serial.print("[FSSM]");
    Serial.print("FSSM_state= ");
    Serial.print(finger_switch_state);
    Serial.print(", FSSM_event= ");
    Serial.print(finger_switch_event);
    Serial.print(", FSSM_result= ");
    Serial.print(finger_switch_output);
    Serial.println(".");
  #endif
}