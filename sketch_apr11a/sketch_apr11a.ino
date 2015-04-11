/*
Realataime - The real time Robot you will love
11 avril 2015
Gabriel   MABILLE
Frederic  RECOULES

Huge thanks to Mr Nicolas Larsen : http://forum.arduino.cc/index.php?action=dlattach;topic=63651.0;attach=3585
*/

#include <avr/wdt.h>

int loop_count=0;

/**
 * Init function
 */
void setud() {
  watchdogSetup();
}

/**
 * Function setup the watchdog to expire in 8 seconds
 */
void watchdogSetup(void) {
  // Disable all interrupt
  cli();
  
  // Reset du timer du watchdog
  wdt_reset();
  
  /*
  WDTCSR configuration:
  WDIE = 1 :Interrupt Enable
  WDE  = 1 :Reset Enable
  WDP3 = 1 :For 8000ms Time-out
  WDP2 = 0 :For 8000ms Time-out
  WDP1 = 0 :For 8000ms Time-out
  WDP0 = 1 :For 8000ms Time-out
  */
  // Enter Watchdog Configuration mode:
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // Set Watchdog settings:
  WDTCSR = (1<<WDIE) | (1<<WDE) | (1<<WDP3) | (0<<WDP2) | (0<<WDP1) | (1<<WDP0);
  
  // Enable all interrupt
  sei();
}

/**
 * Function loop
 */
void loop() {
}


/**
 * Watchdog timer interrupt.
 */
ISR(WDT_vect) 
{
// Include your code here - be careful not to use functions they may cause the interrupt to hang and
// prevent a reset.
}
