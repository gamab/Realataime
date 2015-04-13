/*
Realataime - The real time Robot you will love
11 avril 2015
Gabriel   MABILLE
Frederic  RECOULES

Huge thanks to Mr Nicolas Larsen : http://forum.arduino.cc/index.php?action=dlattach;topic=63651.0;attach=3585
Thanks to http://letmeknow.fr/blog/tuto-ajouter-le-bluetooth-au-arduino/ too for the bluetooth tutorial.
*/

#include <avr/wdt.h>

//Sens
#define FORTH                     'F'
#define BACK                      'B'

//Motor
#define MOTOR1                     1
#define MOTOR2                     2
#define MOTOR_BOTH                 3

//Returns
#define ACK                       'Y'
#define NACK                      'N'

//Commandes
#define CMD_START_WATCHDOG        'W'
#define CMD_START_INSECURELY      'u'
#define CMD_RELOAD_WATCHDOG       'w'
#define CMD_GET_VBAT              'v'
#define CMD_MOVE                  'M'     //USAGE M motor sens velocity
//#define CMD_SET_MOTORS            'm'
//#define CMD_TURN                  'T'
//#define CMD_IS_BUSY               'b'
//#define CMD_PING                  'p'
//#define CMD_GET_ODO               'o'
//#define CMD_RESET                 'r'
//#define CMD_GET_VERSION           'V'
//#define CMD_GET_SENSOR            's'

//Indicate wether the robot has started or not
int robot_started;

/*****MOTORS*****/
//Here are the pins we are using for the motors
/** 
 * On the L298 :
 * IN1  |   IN2   |   Spin
 * 0    |   1     |   Clockwise
 * 1    |   0     |   Anticlockwise
 **/

//MOTOR1:
const int pwm1 = 3;
const int in1 = 2;
const int in2 = 4;

//MOTOR2:
const int pwm2 = 6;
const int in3 = 7;
const int in4 = 8;
/***** *****/
 

//une entrée analogique (A0) pour régler la vitesse manuellement
int speed1;


/**
 * Init function
 */
void setup() {  
  //Initialize the serial link
  Serial.begin(9600);
  
  //Robot is stopped at first
  robot_started = 0;
  
  //preparing motors
  pinMode(pwm1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);  
  pinMode(pwm2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
    
  //motors : stop forth
  analogWrite(pwm1,0);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(pwm2,0);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
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

  char br;
  char sens;
  byte velocity;
  byte motor;
  
  if (Serial.available()>0) {
    //Reads a byte from the serial interface
    br = Serial.read();
    
    if (!robot_started) {
      switch (br) {
        case CMD_START_WATCHDOG :
          //start watchdog
          watchdogSetup();
          robot_started = 1;
          Serial.println(ACK);      
          break;
        case CMD_START_INSECURELY :
          //nothing atm
          robot_started = 1;
          Serial.println(ACK);
          break;
        default :
          Serial.println(NACK);      
      }
    }
    else {
      switch (br) {
        //Maybe we should put the reload of the watchdog on a separate timer
        //to force update precisely every seconds as they were doing it
        case CMD_RELOAD_WATCHDOG :
          wdt_reset();
          Serial.println(ACK); 
          break;
        case CMD_GET_VBAT :
          //TO BE COMPLETED
          Serial.println(NACK); 
          break;
        case CMD_MOVE :
          //Only possible if we have all bytes we need
          if (Serial.available()>=3) {
            //Getting motor sens of the spin and velocity
            motor = Serial.read();
            sens = Serial.read();
            velocity = Serial.read();
            
            if (updateMotor(motor,sens,velocity)) {
              Serial.println(ACK); 
            }
            else {
              Serial.println(NACK); 
            }
            
          }
          else {
            Serial.println(NACK);
          }
          break;
        default :
          Serial.println(NACK); 
      }
    }
  }
}

/**
 * Function that updates the speed and the spin of a motor
 * or both if motor is MOTOR_BOTH
 * @PARAM :
 * motor    : the motor to update
 * sens     : the sens of the spin
 * velocity : the velocity of the motor
 * @RETURN
 * 0 if sens or motor where not expected
 * 1 if the update is done
 */
byte updateMotor(byte motor, char sens, byte velocity) {
  byte sensOk = 1;
  byte motorOk = 1;
  
  if (motor==MOTOR1) {
      if (sens==FORTH) {
        digitalWrite(in1,LOW);
        digitalWrite(in2,HIGH);
      }
      else if (sens==BACK) {
        digitalWrite(in1,HIGH);
        digitalWrite(in2,LOW);
      }
      else {
        sensOk=0;
      }
      
      //Changing the velocity
      if (sensOk)
        analogWrite(pwm1,velocity);
  }
  else if (motor==MOTOR2) {
      if (sens==FORTH) {
        digitalWrite(in3,LOW);
        digitalWrite(in4,HIGH);
      }
      else if (sens==BACK) {
        digitalWrite(in3,HIGH);
        digitalWrite(in4,LOW);
      }
      else {
        sensOk=0;
      }
      
      //Changing the velocity
      if (sensOk)
        analogWrite(pwm2,velocity);
  }
  else if (motor==MOTOR_BOTH) {
    if (sens==FORTH) {
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);
      digitalWrite(in3,LOW);
      digitalWrite(in4,HIGH);
    }
    else if (sens==BACK) {
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
    }
    else {
      sensOk=0;
    }
    
    //Changing the velocity
    if (sensOk)
  {
      analogWrite(pwm2,velocity);
      analogWrite(pwm2,velocity);
    } 
  }
  else {
    motorOk = 0;
  }
  
  return motorOk && sensOk;
}

/**
 * Watchdog timer interrupt.
 */
ISR(WDT_vect) 
{
// Include your code here - be careful not to use functions they may cause the interrupt to hang and
// prevent a reset.
}
