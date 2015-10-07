#include <NTREK.h>
#include <CwruStim.h>
//#include <TimerOne.h>

NTREK Ntrek(Board_ID_A101);

Stim Stim_Perc_J115(STIM_CHANNEL_UART1);

int32_t counter_main_loop = 0;

void setup() {
  // Setup the board hardware
  Ntrek.setup(SETUP_MODE_DEFAULT);

  // Setup CwruStim library
  setupCwruStim();
}

void loop() {
  //accumulate main loop counter
  counter_main_loop++;

  // Toggle LED to show system states
  Ntrek.io_toggle(LED_GREEN);
  Ntrek.io_toggle(LED_RED);

  // have a rest
  delay(500);
}

void setupCwruStim(void) {
  Stim_Perc_J115.init(STIM_MODE_PERC); // Initialize the Stim board and delete old schedule
  Stim_Perc_J115.config(STIM_SETTING_DEFAULT); // Setup channels, schedule, and events
  Stim_Perc_J115.start(UECU_SYNC_MSG); // Send start command (Sync message)

  // Stim Event update
  // Stim_Perc_J115.update(STIM_COMMAND_ZERO_ALL); // Set pulse width and amplitude to 0 for all four channels. 
}
