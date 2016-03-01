#include <Servo.h>

// Speed constants
#define MAX_SPEED 255
#define MIN_SPEED 0 // not moving
#define SPEED_DEC 5 // decrement value for speed (when slowing down)
#define TURN_SPEED 175

// Arduino digital pins for controlling DC motors
#define M1_DIR_PIN 4
#define M1_SPEED_PIN 5
#define M2_DIR_PIN 7
#define M2_SPEED_PIN 6

// Arduino digital pins corresponding to the robot's functionalities
#define PRNCP_FUNC1_PIN 1
#define PRNCP_FUNC2_PIN 2
#define ADD_FUNC_PIN 3

// Robot directions
#define FORWARD 0
#define RIGHT 1
#define LEFT 2
#define BACKWARDS 3

#define ECHO 10         // Arduino pin to which the ultrasonic sensor's echo pin is connected
#define TRIGGER 11      // Arduino pin to which the ultrasonic sensor's trigger pin is connected
#define TEMPERATURE A0  // Arduino pin to which the LM35 output is connected
#define SERVO 12        // Arduino pin to which the servo output is connected

// Optical sensors
#define SENSOR_F A1
#define SENSOR_L A2
#define SENSOR_R A3
#define BLACK_THRESHOLD 200

#define STOP_THRESHOLD 40.00
#define SCAN_THRESHOLD 10.00
#define SLOW_DOWN_TIME 1000

Servo scanningServo;
bool leave = false;
int sensorf_value, sensorl_value, sensorr_value = 0;
int currentServoDegrees = 90;   // Servo is initially facing forwards
int current_left_speed = 0;
int current_right_speed = 0;

void setup() {
  Serial.begin(9600);

  pinMode(PRNCP_FUNC1_PIN, INPUT);
  pinMode(PRNCP_FUNC2_PIN, INPUT);
  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M2_DIR_PIN, OUTPUT);

  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(TEMPERATURE, INPUT);

  pinMode(SENSOR_F, INPUT);
  pinMode(SENSOR_L, INPUT);
  pinMode(SENSOR_R, INPUT);
  scanningServo.attach(SERVO);
}

void loop() {
  if (digitalRead(PRNCP_FUNC1_PIN)) {
    prncp_func1();
  }
  else if (digitalRead(PRNCP_FUNC2_PIN)) {
    prncp_func2();
  }
  else if (digitalRead(ADD_FUNC_PIN)) {
    add_func();
  }
}

// Executes the first principle function

void prncp_func1() {
  int obstacleDistance;
  int i;

  move_forward(MAX_SPEED);
  
  obstacleDistance = distanceFromSensor();
  if (obstacleDistance <= STOP_THRESHOLD) {
    int turnDirection;
    
    // Slow down if an object is detected and come to a stop
    slow_down(SLOW_DOWN_TIME);
    Serial.println("STOPPED");
    delay(500);
    
    // Scan the surroundings and take appropriate action
    turnDirection = scanEnvironment();
    switch(turnDirection) {
      case LEFT:
        for (i = 0; i < 300; i++) {
          turn_robot(LEFT);
        }
        Serial.println("TURNING LEFT");
        break;
      case RIGHT:
        for (i = 0; i < 300; i++) {
          turn_robot(RIGHT);
        }
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

// Executes the second principle function

void prncp_func2() {
  followLine(); 
}

// Executes the additional function

void add_func() {
  //nothing here yet
}

void run_tests() {
    //Test Case 1:
  //Move forward at MAX_SPEED for one second, then stopping for one second.
  move_forward(MAX_SPEED);
  delay(1000);
  stop_motors();
  delay(1000);
  
  //Test Case 2:
  //Turn left at MAX_SPEED for one second, then stopping for one second.
  for (int i = 0; i < 1000; i++) {
    turn_robot(LEFT);
  }
  delay(1000);
  //Test Case 3:
  //Turn right at MAX_SPEED for one second, then stopping for one second.
  for (int i = 0; i < 1000; i++) {
    turn_robot(RIGHT);
  }
  delay(1000);


  //Test Case 4:
  //Only run one of the motors
  int value;
  for (value = 0 ; value <= 255; value += 5)
  {
    digitalWrite(M2_DIR_PIN, LOW);
    //digitalWrite(M2, HIGH);
    analogWrite(M2_SPEED_PIN, value);   //PWM Speed Control
    //analogWrite(E2, value);   //PWM Speed Control
    delay(30);
  }

  //Test Case 5:
  //Move forward for maximum speed, when an object is detected to be within our threshold of 50cm, slow down the robot
  //Robot should come to a stop in 2 seconds
  
  move_forward(MAX_SPEED);
  //turn_left(2000);
  //turn_left(150);

  float dist;
//  dist = distanceFromSensor();
  Serial.println(dist);
  if (dist < 50.00) {
    slow_down(2000);
    delay(1000);
  }
}

//------------------------------------------------------------------------------------------------------

// Move robot forward at a given speed

void move_forward(int speed) {
  set_motors(FORWARD);
  current_left_speed = speed;
  current_right_speed = speed;
  analogWrite(M1_SPEED_PIN, speed);
  analogWrite(M2_SPEED_PIN, speed);
}

// Turn robot from forward direction towards input direction for 1 ms

void turn_robot(int direction) {
  set_motors(direction);
  analogWrite(M1_SPEED_PIN, TURN_SPEED);
  analogWrite(M2_SPEED_PIN, TURN_SPEED);
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

void speed_right(int speed_amount) {
  int desired_right_speed = current_right_speed + speed_amount;
  if (desired_right_speed > MAX_SPEED) {
    analogWrite(M2_SPEED_PIN, MAX_SPEED);
    current_right_speed = MAX_SPEED;
  }
  else {
    analogWrite(M2_SPEED_PIN, desired_right_speed);
    current_right_speed = desired_right_speed;
  }
}

void speed_left(int speed_amount) {
  int desired_left_speed = current_left_speed + speed_amount;
  if (desired_left_speed > MAX_SPEED) {
    analogWrite(M1_SPEED_PIN, MAX_SPEED);
    current_left_speed = MAX_SPEED;
  }
  else {
    analogWrite(M1_SPEED_PIN, desired_left_speed);
    current_left_speed = desired_left_speed;
  }
}

void slow_right(int slow_amount) {
  int desired_right_speed = current_right_speed - speed_amount;
  if (desired_right_speed < MIN_SPEED) {
    analogWrite(M2_SPEED_PIN, MIN_SPEED);
    current_right_speed = MIN_SPEED;
  }
  else {
    analogWrite(M2_SPEED_PIN, desired_right_speed);
    current_right_speed = desired_right_speed;
  }

void slow_left(int slow_amount) {
  int desired_left_speed = current_left_speed - speed_amount;
  if (desired_left_speed < MIN_SPEED) {
    analogWrite(M1_SPEED_PIN, MIN_SPEED);
    current_left_speed = MIN_SPEED;
  }
  else {
    analogWrite(M1_SPEED_PIN, desired_left_speed);
    current_left_speed = desired_left_speed;
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
    //else
    //stop_motors();

//}
    
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
              delay(1);
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
    turn_robot(direction);
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
