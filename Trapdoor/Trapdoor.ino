/******************************************
 * Description: operate a stepper motor remotely with push buttons to remotely focus a telescope.
 * Board: Arudino Uno
 * 
 * Motor: 28BYJ-48 + Driver
 * Source: http://www.4tronix.co.uk/arduino/Stepper-Motors.php 
 * 
 * Wiring:
LED as Sensor:
  Anode: A0
  Cathode: GND

LED Source:
  Anode: PIN6
  Cathod: RES->GND

Motor Driver:
  RED: 5V
  BROWN: GND
  BLUE: PIN11
  GREEN: PIN10
  YELLOW: PIN9
  ORANGE: PIN8

 ******************************************/
#include <AccelStepper.h>

const int MOTION_PIN = 2; // Pin connected to motion detector
const int LED_PIN = 13; // LED pin - active-high
const int stepsPerRevolution = 32;
 
//declare variables for the motor pins
int motorPin1 = 8;		// Blue   - 28BYJ48 pin 1
int motorPin2 = 9;		// Pink   - 28BYJ48 pin 2
int motorPin3 = 10;		// Yellow - 28BYJ48 pin 3
int motorPin4 = 11;		// Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 (VCC)

const int led = 6;

int sens = 0;
int total = 0;
int total1 = 0;
int x = 0;

// This will probably need adjusted
const int dark_threshold = 75;

#define MotorInterfaceType 8
#define maxSpeed 1200 
//#define maxSpeed 600

// initialize the stepper library on pins 8 through 11:
// AccelStepper forwardStepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin2, motorPin3, motorPin4); // This one won't go in reverse with negative speeds.  Doesn't work.
AccelStepper myStepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);
int stepCount = 0;  // number of steps the motor has taken

void setup() {
  
	// Setup Serial port for debugging purposes
	Serial.begin(9600);
	// The PIR sensor's output signal is an open-collector, 
	// so a pull-up resistor is required:
	pinMode(MOTION_PIN, INPUT_PULLUP);
	pinMode(LED_PIN, OUTPUT);
  
  //declare the motor pins as outputs
    // I don't think this is needed; pretty sure AccelStepper does this? Need to verify
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  Serial.begin(9600);

  // Set switch pins as inputs and pull-up
//  pinMode(cwPin, INPUT_PULLUP);
//  pinMode(ccwPin, INPUT_PULLUP);

  myStepper.setMaxSpeed(maxSpeed);
  // May need to make negative
  myStepper.setSpeed(maxSpeed);

  analogWrite(led,255);

}
//////////////////////////////////////////////////////////////////////////////
void loop(){

  for(x =0; x < 10; x++) {
    sens = analogRead(0);
    total = total1 + sens; total1 = total;
  }
  sens = total/x;
  Serial.println(sens);
    
  total = 0;
  total1 = 0;

  if (sens < dark_threshold) {
    uint32_t i = 0;
    while (i < 2500000) {
      myStepper.runSpeed();
      i++;
    }

    // Enter Infinite blink loop
    while(1) {
      analogWrite(led, 0);
      delay(1000);
      analogWrite(led,255);
      delay(1000);
    }
  }

  delay(500);

}