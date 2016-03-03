#include <Servo.h>

//--------------------------------------
// PIN DEFINITIONS
//--------------------------------------

// Arduino digital pins for controlling DC motors
#define M1_DIR_PIN    4   // Direction pin for left motor
#define M1_SPEED_PIN  5   // Speed pin for left motor
#define M2_DIR_PIN    7   // Direction pin for right motor
#define M2_SPEED_PIN  6   // Speed pin for right motor

// Arduino pins used for the ultrasonic sensor
#define ECHO          12  // Arduino pin to which the ultrasonic sensor's echo pin is connected
#define TRIGGER       11  // Arduino pin to which the ultrasonic sensor's trigger pin is connected
#define TEMPERATURE   A0  // Arduino pin to which the LM35 output is connected
#define SERVO         10  // Arduino pin to which the servo output is connected

// Arduino analog pins used for the hall effect sensors
#define LEFT_WHEEL    A4  // Hall effect sensor on left side
#define RIGHT_WHEEL   A5  // Hall effect sensor on right side

// Arduino analog pins used for the optical sensors
#define SENSOR_F A3
#define SENSOR_L A2
#define SENSOR_R A1

// Arduino digital pin corresponding to the robot's functionalities
#define SWITCH_FUNC_PIN 9


//--------------------------------------
// CONSTANT DEFINITIONS
//--------------------------------------

// Speed constants
#define MAX_SPEED   255   // Maximum speed (maximum voltage that can be applied to the motors)
#define MIN_SPEED   0     // Not moving
#define SPEED_DEC   5     // Decrement value for speed (when slowing down)
#define TURN_SPEED  175   // The speed of the robot when turning

// Threshold values
#define BLACK_THRESHOLD     200   // Threshold for optical sensor - below this value means sensor is reading black (F2)
#define STOP_THRESHOLD      40.00 // Below this value and the robot starts slowing down (F1)
#define SCAN_THRESHOLD      10.00 // Below this value and the robot will definitely not turn in this direction (F1)
#define SLOW_DOWN_TIME      1000  // How many milliseconds it will take for the robot to slow down (F1)
#define F1_TURN_TIME        300   // How many milliseconds the robot will turn (F1)
#define HALL_THRESHOLD      50    // Below this value means a magnet is detected (F1)
#define ROTATION_THRESHOLD  20    // How many milliseconds of difference between wheel periods is acceptable (F1)

// Robot directions (i.e. enumerating directions)
#define FORWARD 0
#define RIGHT 1
#define LEFT 2
#define BACKWARDS 3

// Other constants
#define ON 1
#define OFF 0

//--------------------------------------
// FUNCTION PROTOTYPES
//--------------------------------------

//--------------------------------------
// GLOBAL VARIABLES
//--------------------------------------

Servo scanningServo;
bool leave = false;
int sensorf_value, sensorl_value, sensorr_value = 0;
int currentServoDegrees = 90;   // Servo is initially facing forwards
int current_left_speed = 0;
int current_right_speed = 0;

int SIZE_IN = 5;
int SIZE_OUT = 4;
int inputPins[]  = {ECHO, SENSOR_F, SENSOR_L, SENSOR_R, SWITCH_FUNC_PIN};
int outputPins[] = {M1_DIR_PIN, M2_DIR_PIN, TRIGGER, TEMPERATURE};

void setup() {
  int i;  // Counter variable used for initializing pins later in the setup() function
  Serial.begin(9600);

  // Initialize input pins
  for (i = 0; i < SIZE_IN; i++) {
    pinMode(inputPins[i], INPUT);
  }

  // Initialize output pins
  for (i = 0; i < SIZE_OUT; i++) {
    pinMode(outputPins[i], OUTPUT);
  }

  // Initialize pin for servo motor
  scanningServo.attach(SERVO);

  // Ensure servo motor is facing straight at the beginning
  scanningServo.write(90);
}

void loop() {

  if(digitalRead(SWITCH_FUNC_PIN) != HIGH)
  {
    prncp_func1();
  }

  else
  {
    Serial.println(1);
  }
 
  //move_forward(MAX_SPEED);
  //Serial.println(digitalRead(SWITCH_FUNC_PIN));
  //delay(100);
}

// Executes the first principle function

void prncp_func1() {
  move_forward(MAX_SPEED);

  while (distanceFromSensor() > STOP_THRESHOLD) {
    Serial.print(distanceFromSensor());
    Serial.print("\t");
    Serial.println(analogRead(A0));
    //adjustCourse(0);
  }

  if (distanceFromSensor() <= STOP_THRESHOLD) {
    int turnDirection;
    int i;
  
    // Slow down if an object is detected and come to a stop
    slow_down(SLOW_DOWN_TIME);
  
    // Scan the surroundings and take appropriate action
    turnDirection = scanEnvironment();
    switch(turnDirection) {
      case LEFT:
        for (i = 0; i < F1_TURN_TIME; i++) {
          turn_robot(LEFT, TURN_SPEED);
        }
        Serial.println("TURNING LEFT");
        break;
      case RIGHT:
        for (i = 0; i < F1_TURN_TIME; i++) {
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

//--------------------------------------
// HELPER FUNCTIONS FOR MOTORS
//--------------------------------------

/*
 * Moves the robot forward at a given speed
 * 
 * Param: speed - the speed to set both motors
 */
void move_forward(int speed) {
  set_motors(FORWARD);              // Ensure motors are set up to move forwards
  current_left_speed = speed;       // Set left speed variable
  current_right_speed = speed;      // Set right speed variable
  analogWrite(M1_SPEED_PIN, speed); // Set the speed of the left motor
  analogWrite(M2_SPEED_PIN, speed); // Set the speed of the right motor
}

/*
 * Turns the robot from forward direction towards input direction for 1 millisecond
 * 
 * Param: direction - the direction to turn (NOTE: use predefined directions, i.e. LEFT, RIGHT)
 * Param: turn_speed - the speed with which to turn
 */
void turn_robot(int direction, int turn_speed) {
  set_motors(direction);                    // Set direction of motors to be same as user input
  analogWrite(M1_SPEED_PIN, turn_speed);    // Set speed of left motor
  analogWrite(M2_SPEED_PIN, turn_speed);    // Set speed of right motor
  delay(1);                                 // Turn robot for 1 millisecond
  stop_motors();                            // Once turning is finished, stop motors
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

/* 
 * Speeds up a motor by a given amount.
 * 
 * Param: motor - the motor to speed up (RIGHT or LEFT)
 * Param: amount - the amount to speed up by (0-255)
 */

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

/* 
 * Slows down a motor by a given amount.
 * 
 * Param: motor - the motor to slow down (RIGHT or LEFT)
 * Param: amount - the amount to slow down by (0-255)
 */

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

/* Stops the motors from turning */

void stop_motors() {
  analogWrite(M1_SPEED_PIN, MIN_SPEED);
  analogWrite(M2_SPEED_PIN, MIN_SPEED);
}

/*
 * Set the motors for a desired direction for the robot
 * Param: direction - either FORWARD, LEFT, RIGHT, or BACKWARDS
 */

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
  else {  // BACKWARDS
    digitalWrite(M1_DIR_PIN, LOW);
    digitalWrite(M2_DIR_PIN, LOW);
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
float distanceFromSensor() {
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

/**
   The function followLine() allows for the robot to follow a guided path using the reflective optical sensors. The sensors are attached at the front of the robot in a triangle formation.
   Algorithm:
             -when front sensor detects black tape, will move forward at _ speed
             -when front sensor is off the black tape, the robot will read the left and right sensors. The robot will turn in the direction of whatever sensor that detects the line, and
              will not stop turning until the front sensor detects the black line. When the line is detected, the robot continues to travel straight. 
             -when no sensors detect the line, the robot will travel forward for 0.5s before it reads the left and right sensors again. 
             -when both sensors detect line, the robot will choose a random direction to spin.         
*/

void followLine() {
    sensorf_value = analogRead(SENSOR_F); //read the front sensor
    
    //if the front sensor detects the path, continue moving forward
    if(sensorf_value > BLACK_THRESHOLD)
    {
        move_forward(MAX_SPEED/2);
        delay(1);
    }
    
    else
    {
        sensorl_value = analogRead(SENSOR_L); //read the left sensor
        sensorr_value = analogRead(SENSOR_R); //read the right sensor

        //if the left sensor detects the path AND the right sensor does not, turn the robot to the left until the front sensor detects the path again,
        //and then continue moving forward
        if(sensorl_value > BLACK_THRESHOLD && sensorr_value <= BLACK_THRESHOLD)
        {
              turn_to_check_path(LEFT);
        }

        //if the right sensor detects the path AND the left sensor does not, turn the robot to the right until the front sensor detects the path again,
        //and then continue moving forward
        else if(sensorl_value <= BLACK_THRESHOLD && sensorr_value > BLACK_THRESHOLD)
        {
              turn_to_check_path(RIGHT);
        }

        //if neither sensor detects the path, move the robot forward a tiny bit and try again
        else if(sensorl_value <= BLACK_THRESHOLD && sensorr_value <= BLACK_THRESHOLD)
        {
              move_forward(MAX_SPEED);
        }

        //if both sensors detect the path, turn the robot left until the front sensor detects the path
        else
        {
              turn_to_check_path(LEFT);
        }
    }
 
}

//Function that turns the robot in a certain direction for 90 degrees. Stops if the front sensor detects the path
void turn_to_check_path(int direction) {
  for(int i = 0; i <= 150; i++)
  {
    turn_robot(direction, TURN_SPEED);
    if(analogRead(SENSOR_F) > BLACK_THRESHOLD)
    {
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
  float leftDistance, rightDistance;
  
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


