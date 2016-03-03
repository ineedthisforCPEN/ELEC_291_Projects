#include <Servo.h>

//--------------------------------------
// PIN DEFINITIONS
//--------------------------------------

// Define Arduino pins used for the motor
#define M1_DIR_PIN    4   // Direction pin for left motor
#define M1_SPEED_PIN  5   // Speed pin for the left motor
#define M2_DIR_PIN    7   // Direction pin for the right motor
#define M2_SPEED_PIN  6   // Speed pin for the right motor

// Define Arduino pins used for the ultrasonic sensor
#define ECHO        12    // Arduino pin to which the ultrasonic sensor's echo pin is connected
#define TRIGGER     11    // Arduino pin to which the ultrasonic sensor's trigger pin is connected
#define TEMPERATURE A0    // Arduino pin to which the LM35 output is connected
#define SERVO       10    // Arduino pin to which the servo output is connected

// Define Arduino pins used for the hall effect sensors
#define LEFT_WHEEL  A5    // Hall effect sensor on left side
#define RIGHT_WHEEL A6    // Hall effect sensor on right side

//--------------------------------------
// CONSTANT DEFINITIONS
//--------------------------------------

// Constants for motor direction (i.e. enumerating directions)
#define FORWARD   0
#define LEFT      1
#define RIGHT     2
#define BACKWARDS 3

// Constants for motor control
#define MAX_SPEED 255     // Maximum speed
#define TURN_SPEED 175    // Speed at which to turn
#define MIN_SPEED 0       // Not moving
#define SPEED_DEC 5       // Decrement value for speed

// Other consants
#define STOP_THRESHOLD 45.00    // If distance measured is less than this, start slowing down
#define SCAN_THRESHOLD 10.00    // If distance measured is less that this, definitely don't turn in this direction
#define SLOW_DOWN_TIME 1000     // How many milliseconds it will take to slow down
#define TURN_TIME 300           // How many milliseconds the robot will turn for
#define HALL_THRESHOLD 50       // Anything below this value means a magnet is detected
#define ROTATION_THRESHOLD 20   // How many milliseconds of difference between wheel periods is acceptable

/* Function prototupes */
int scanEnvironment(void);
float calculatePeriod(void);
float distanceFromSensor(void);

/* Global variables */
Servo scanningServo;
int currentServoDegrees = 90;   // Servo is initially facing forwards
int current_speed = 0;
int current_left_speed = 0;
int current_right_speed = 0;

void setup() {
  // Initialize serial port - useful for debugging
  Serial.begin(9600);

  // Initialize pins and servo motor
  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M1_SPEED_PIN, OUTPUT);
  pinMode(M2_DIR_PIN, OUTPUT);
  pinMode(M2_SPEED_PIN, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(TEMPERATURE, INPUT);
  scanningServo.attach(SERVO);

  scanningServo.write(90);
}

void loop() {
  //int obstacleDistance;
  int i;

  move_forward(MAX_SPEED);

  while (distanceFromSensor() > STOP_THRESHOLD) {
    Serial.print(distanceFromSensor());
    Serial.print("\t");
    Serial.println(analogRead(A0));
    //adjustCourse(0);
  }

  if (distanceFromSensor() <= STOP_THRESHOLD) {
    
  int turnDirection;
  
  // Slow down if an object is detected and come to a stop
  slow_down(SLOW_DOWN_TIME /* ((int) distanceFromSensor() / STOP_THRESHOLD)*/);
  
  // Scan the surroundings and take appropriate action
  turnDirection = scanEnvironment();
  switch(turnDirection) {
    case LEFT:
      for (i = 0; i < TURN_TIME; i++) {
        turn_robot(LEFT, TURN_SPEED);
      }
      Serial.println("TURNING LEFT");
      break;
    case RIGHT:
      for (i = 0; i < TURN_TIME; i++) {
        turn_robot(RIGHT, TURN_SPEED);
      }
      Serial.println("TURNING RIGHT");
      break;
    case BACKWARDS:
      // Default case is to go backwards
    default:
      set_motors(BACKWARDS);
      analogWrite(M1_SPEED_PIN, MAX_SPEED);
      analogWrite(M2_SPEED_PIN, MAX_SPEED);
      delay(1000);
      stop_motors();
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
  delay(100);
  rightDistance = distanceFromSensor();
  delay(100);
  Serial.print("Right:\t");
  Serial.println(rightDistance);

  // Smoothly turn the sensor to face the right
  while (currentServoDegrees < 180) {
    scanningServo.write(++currentServoDegrees);
    delay(4);
  }

  // Measure the distance to the nearest object
  delay(100);
  leftDistance = distanceFromSensor();
  delay(100);
  Serial.print("Left:\t");
  Serial.println(leftDistance);

  // Smoothly return the sensor to face forwards
  while (currentServoDegrees > 90) {
    scanningServo.write(--currentServoDegrees);
  }
  delay(200);

  if (leftDistance < SCAN_THRESHOLD && rightDistance < SCAN_THRESHOLD) {
    // If stuck, move backwards
    return BACKWARDS;
  } else if (leftDistance > rightDistance) {
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
// FUNCTION FOR ADJUSTING COURSE
//------------------------------------------------

void adjustCourse(int currentSpeedVoltage) {
  // Make sure the parameter is within a valid range (i.e. between 0 and 255 inclusive)
  if (currentSpeedVoltage < 0) currentSpeedVoltage = 0;
  else if (currentSpeedVoltage > 255) currentSpeedVoltage = 255;
  
  long start_left = 0;  // Used to measure period of left wheel
  long start_right = 0; // Used to measure period of right wheel
  long period_left = 0;
  long period_right = 0;

  int left_measure;
  int right_measure;

  while (true) {
    left_measure = analogRead(LEFT_WHEEL);
    right_measure = analogRead(RIGHT_WHEEL);

    //Serial.print(left_measure);
    //Serial.print("\t");
    //Serial.println(right_measure);

    if (start_left   == 0 && left_measure  < HALL_THRESHOLD) start_left   = millis();
    if (start_right  == 0 && right_measure < HALL_THRESHOLD) start_right  = millis();
    
    // NOTE: 200 is used below because the fastest possible period is 450 milliseconds (200 is less than 450),
    // yet the wheel can spin enough in 200 milliseconds to pass the hall effect sensor wihtout being read
    // twice before completing a full revolution
    if (period_left  == 0 && left_measure  < HALL_THRESHOLD && millis() - start_left  > 200)
      period_left  = millis() - start_left;
    if (period_right == 0 && right_measure < HALL_THRESHOLD && millis() - start_right > 200)
      period_right = millis() - start_right;

    // If both periods have been measured, leave this while loop
    if (period_left != 0 && period_right != 0){
      //Serial.print("start_left:\t");
      //Serial.println(start_left);
      //Serial.print("start_right:\t");
      //Serial.println(start_right);
      if(abs(period_left-period_right) <=ROTATION_THRESHOLD){
        return;
      }
      while(period_left> period_right){
        period_left -= 10;
        Serial.println("Slowing right");
        slow_right(22);
      }
      while(period_right >period_left){
        period_right -= 10;
      Serial.println("Slowing left");
        slow_left(22);
      }
      if (current_left_speed < 200 || current_right_speed <200){
        move_forward(MAX_SPEED);
      }
      break;
    }
  }
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
  float period = 58.0; //calculatePeriod();

  // Return the distance (in centimeters)
  float distance = (float) echoTime / period;

  if (distance > 1000) {
    return 200.0;
  }
  return distance;
}

//------------------------------------------------
// FUNCTIONS FOR ADJUSTING THE MOTORS
//------------------------------------------------

// Move robot forward at a given speed

void move_forward(int speed) {
  set_motors(FORWARD);
  current_left_speed = speed;
  current_right_speed = speed;
  analogWrite(M1_SPEED_PIN, speed);
  analogWrite(M2_SPEED_PIN, speed);
}

// Turn robot from forward direction towards input direction for 1 ms

void turn_robot(int direction, int turn_speed) {
  set_motors(direction);
  analogWrite(M1_SPEED_PIN, turn_speed);
  analogWrite(M2_SPEED_PIN, turn_speed);
  delay(1);
  // once turning is finished, stop motors
  stop_motors();
}

// Slow down robot from MAX_SPEED to MIN_SPEED
// time (ms) specifies duration over which robot slows down

void slow_down(int time) {
  for (int speed = MAX_SPEED; speed > MIN_SPEED; speed -= SPEED_DEC) {
    current_left_speed = speed;
    current_right_speed = speed;
    analogWrite(M1_SPEED_PIN, speed);
    analogWrite(M2_SPEED_PIN, speed);
    delay(time / (MAX_SPEED / SPEED_DEC));
  }
}

void speedup_motor(int motor, int amount) {

  int desired_speed;
  if (motor == RIGHT) {
    desired_speed = current_right_speed + amount;
    if (desired_speed > MAX_SPEED) {
      analogWrite(M2_SPEED_PIN, MAX_SPEED);
      current_right_speed = MAX_SPEED;
    }
    else {
      analogWrite(M2_SPEED_PIN, desired_speed);
      current_right_speed = desired_speed;
    }
  }
  
  else if (motor == LEFT) {
    desired_speed = current_left_speed + amount;
    if (desired_speed > MAX_SPEED) {
      analogWrite(M1_SPEED_PIN, MAX_SPEED);
      current_left_speed = MAX_SPEED;
    }
    else {
      analogWrite(M1_SPEED_PIN, desired_speed);
      current_left_speed = desired_speed;
    }
  }
   
}

// Slows down the motor (RIGHT or LEFT) by a given amount.

void slowdown_motor(int motor, int amount) {

  int desired_speed;
  if (motor == RIGHT) {
    desired_speed = current_right_speed - amount;
    if (desired_speed < MIN_SPEED) {
      analogWrite(M2_SPEED_PIN, MIN_SPEED);
      current_right_speed = MIN_SPEED;
    }
    else {
      analogWrite(M2_SPEED_PIN, desired_speed);
      current_right_speed = desired_speed;
    }
  }
  
  else if (motor == LEFT) {
    desired_speed = current_left_speed - amount;
    if (desired_speed < MIN_SPEED) {
      analogWrite(M1_SPEED_PIN, MIN_SPEED);
      current_left_speed = MIN_SPEED;
    }
    else {
      analogWrite(M1_SPEED_PIN, desired_speed);
      current_left_speed = desired_speed;
    }
  }
}

// Stops the motors from turning

void stop_motors() {
  analogWrite(M1_SPEED_PIN, MIN_SPEED);
  analogWrite(M2_SPEED_PIN, MIN_SPEED);
}

// Set the motors for a desired direction for the robot

void set_motors(int direction) {
  if (direction == FORWARD) {
    digitalWrite(M1_DIR_PIN, HIGH);
    digitalWrite(M2_DIR_PIN, HIGH);
  }
  else if (direction == RIGHT) {
    digitalWrite(M1_DIR_PIN, HIGH );
    digitalWrite(M2_DIR_PIN, LOW);
  }
  else if (direction == LEFT) {
    digitalWrite(M1_DIR_PIN, LOW);
    digitalWrite(M2_DIR_PIN, HIGH);
  }
  else {
    digitalWrite(M1_DIR_PIN, LOW);
    digitalWrite(M2_DIR_PIN, LOW);
  }
}

void slow_right(int slow_amount) {
  analogWrite(M2_SPEED_PIN, current_right_speed - slow_amount);
  current_right_speed -= slow_amount;
}

void slow_left(int slow_amount) {
  analogWrite(M1_SPEED_PIN, current_left_speed - slow_amount);
  current_left_speed -= slow_amount;
}
