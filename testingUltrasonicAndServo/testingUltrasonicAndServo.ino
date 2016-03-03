#include <Servo.h>

#define ECHO 13
#define TRIGGER 12
#define TEMPERATURE A0
#define SERVO 4

#define LEFT 0
#define RIGHT 1

/* Function prototypes */
int checkSurroundings(void);
float calculatePeriod(void);
float distanceFromSensor(void);

/* Global variables */
Servo myServo;
int currentServoDegrees = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(TEMPERATURE, INPUT);
  myServo.attach(SERVO);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(checkSurroundings());
}

int checkSurroundings(void) {
  float leftDistance;
  float rightDistance;
  
  myServo.write(0);
  delay(200);
  leftDistance = distanceFromSensor();
  myServo.write(180);
  delay(200);
  rightDistance = distanceFromSensor();
  myServo.write(90);

  if (leftDistance - rightDistance > -0.05) {
    return LEFT;
  }
  return RIGHT;
}

/**
 * This function is meant to calculate the sound's period based on the reading from the
 * LM35 temperature sensor and lecture slides. It reads the temperature sensor value, then
 * returns the calculated period.
 * 
 * This function takes into account that the sound wave must travel twice the distance
 * (hence, 20000.0 is used instead of 10000.0)
 */
float calculatePeriod(void) {
  long startTime = millis();                              // Get current uptime of Arduino board
  long totalMilliVolts = 0;                               // Long for storing total millivolts (long to prevent overflow)
  long numMeasures = 0;                                   // Long for storing number of measurements (long to prevent overflow)

  while (millis() - startTime < 50) {                     // Measure for 50 milliseconds
    totalMilliVolts += analogRead(TEMPERATURE);        // Add to total millivolts measures
    numMeasures++;                                        // Increment measure counter
  }

  // Calculate the period, then return it
  // Calculation gotten from lecture slides
  // The value 0.415282392 is used instead of 500.0/1024.0 - we believe this will reduce the number of computations needed
  // Calculation is (1000000 / 100) * (1 / (331.5 + 0.6 * Temperature))
  // Our reduced calculation is seen below
  return 20000.0 / (331.5 + 0.6*(((float) totalMilliVolts)/((float) numMeasures) * (0.415282392)));
                                /*--------------------- Temperature in C -----------------------*/
}

/**
 * This function determines how far away an object is from the sensor. The calculations
 * are based on the formula given in the lecture slides
 * 
 * Distance in cm = Echo pulse width (in us) / period
 */
float distanceFromSensor(void) {
  // Send a start signal to the trigger pin
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  // Measure how long it takes the echo signals to return
  unsigned long echoTime = pulseIn(ECHO, HIGH);

  // Build in delay with calculatePeriod(), so we do not need to delay an extra 50ms after calling this function
  float period = calculatePeriod();

  // Return the distance (in centimeters)
  return ((float) echoTime / period);
}

