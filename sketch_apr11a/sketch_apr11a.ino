/*
Realataime - The real time Robot you will love
11 avril 2015
Gabriel   MABILLE
Frederic  RECOULES

Huge thanks to Mr Nicolas Larsen : http://forum.arduino.cc/index.php?action=dlattach;topic=63651.0;attach=3585
Thanks to http://letmeknow.fr/blog/tuto-ajouter-le-bluetooth-au-arduino/ to...
*/

#include <avr/wdt.h>

//Commands
#define C_START ((const char)'s')
#define C_START_INSEC ((const char)'z')


//Returns
#define R_START_OK               "START OK"
#define R_START_INSEC_OK         "START INSEC OK"
#define R_ERREUR_UNKNOWN_COMMAND "ERREUR UNKNOWN COMMAND"


//Movements
#define MVT_FORTH  "FORTH"
#define MVT_BACK   "BACK"
#define MVT_RIGHT  "RIGH"
#define MVT_LEFT   "LEFT"

//Motors speed
#define SPEED_HIGH "HIGH"
#define SPEED_LOW  "LOW"

int robot_started;

/**
 * Init function
 */
void setup() {  
  //Initialize the serial link
  Serial.begin(9600);
  
  //Robot is stopped at first
  robot_started = 0;
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
  
  char read;
  
  if (Serial.available()>0) {
    //Reads a byte from the serial interface
    read = Serial.read();
    
    if (!robot_started) {
      switch (read) {
        case C_START :
          //start watchdog
          watchdogSetup();
          robot_started = 1;
          Serial.println(R_START_OK);      
          break;
        case C_START_INSEC :
          //nothing atm
          robot_started = 1;
          Serial.println(R_START_INSEC_OK);
          break;
        default :
          Serial.println(R_ERREUR_UNKNOWN_COMMAND);      
      }
    }
  }
}


/**
 * Watchdog timer interrupt.
 */
ISR(WDT_vect) 
{
// Include your code here - be careful not to use functions they may cause the interrupt to hang and
// prevent a reset.
}
