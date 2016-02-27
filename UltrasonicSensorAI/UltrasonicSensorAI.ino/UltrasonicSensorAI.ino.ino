#include <Servo.h>

#define ECHO 9          // Arduino pin to which the ultrasonic sensor's echo pin is connected
#define TRIGGER 10      // Arduino pin to which the ultrasonic sensor's trigger pin is connected
#define TEMPERATURE A0  // Arduino pin to which the LM35 output is connected
#define SERVO 11        // Arduino pin to which the servo output is connected

#define LEFT 0
#define RIGHT 1
#define BACKWARDS 2
#define STOP_THRESHOLD 50.00
#define SCAN_THRESHOLD 10.00
#define SLOW_DOWN_TIME 2000

/* Function prototupes */
int scanEnvironment(void);
float calculatePeriod(void);
float distanceFromSensor(void);

/* Global variables */
Servo scanningServo;
int currentServoDegrees = 90;   // Servo is initially facing forwards

void setup() {
  // Initialize serial port - useful for debugging
  Serial.begin(9600);

  // Initialize pins and servo motor
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(TEMPERATURE, INPUT);
  scanningServo.attach(SERVO);
}

void loop() {
  int obstacleDistance;

  obstacleDistance = distanceFromSensor();
  if (obstacleDistance <= STOP_THRESHOLD) {
    int turnDirection;
    
    // Slow down if an object is detected and come to a stop
    //slow_down(SLOW_DOWN_TIME);
    Serial.println("STOPPED");
    delay(500);
    
    // Scan the surroundings and take appropriate action
    turnDirection = scanEnvironment();
    switch(turnDirection) {
      case LEFT:
        //turnLeftFunction();
        Serial.println("TURNING LEFT");
        break;
      case RIGHT:
        //turnRightFunction();
        Serial.println("TURNING RIGHT");
        break;
      case BACKWARDS:
        // Default case is to go backwards
      default:
        //goBackwardsFunction();
        Serial.println("GOING BACKWARDS");
        break;
    }
  }
}

//------------------------------------------------
// FUNCTION FOR SCANNING THE ENVORONMENT
//------------------------------------------------

/**
 * This function scans the environment first by rotating
 * the ultrasonic sensor to face left of the robot's
 * previous path (90 degrees left), then rotate the sensor
 * to the right (180 degrees right), then bring the sensor
 * back to face forwards (90 degrees left). The function
 * then determines which direction to rotate and returns
 * the direction's respected integer representation
 * (i.e. 0 for left, 1 for right). If the robot is stuck
 * (i.e. not viable to turn left or right), the function
 * will return BACKWARDS (2).
 * 
 * Return: LEFT (0) if robot should turn left
 *         RIGHT (1) if robot should turn right
 *         BACKWARDS (2) if robot should go backwards
 */
int scanEnvironment(void) {
  int leftDistance;
  int rightDistance;
  
  // Smoothly turn the sensor to face the left
  while (currentServoDegrees > 0) {
    scanningServo.write(--currentServoDegrees);
    delay(4);
  }

  // Measure the distance to the nearest object
  rightDistance = distanceFromSensor();
  Serial.print("Right:\t");
  Serial.println(rightDistance);

  // Smoothly turn the sensor to face the right
  while (currentServoDegrees < 180) {
    scanningServo.write(++currentServoDegrees);
    delay(4);
  }

  // Measure the distance to the nearest object
  leftDistance = distanceFromSensor();
  Serial.print("Left:\t");
  Serial.println(leftDistance);

  // Smoothly return the sensor to face forwards
  while (currentServoDegrees > 90) {
    scanningServo.write(--currentServoDegrees);
  }

  if (leftDistance < SCAN_THRESHOLD && rightDistance < SCAN_THRESHOLD) {
    // If stuck, move backwards
    return BACKWARDS;
  } else if (leftDistance >= rightDistance) {
    // Turn left if there's more space on the left
    return LEFT;
  } else {
    // Turn right if there's more space on the right
    return RIGHT;
  }
  
  // By default, go backwards
  return BACKWARDS;
}

//------------------------------------------------
// FUNCTIONS FOR MEASURING DISTANCE
//------------------------------------------------

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
    totalMilliVolts += analogRead(TEMPERATURE);           // Add to total millivolts measures
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
