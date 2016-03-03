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

// Arduino digital pins used for the LC display
#define RS_pin      0   
#define Enable_pin  1
#define D4          3
#define D5          4
#define D6          5
#define D7          6


//--------------------------------------
// CONSTANT DEFINITIONS
//--------------------------------------

// Speed constants
#define MAX_SPEED   255   // Maximum speed (maximum voltage that can be applied to the motors)
#define MIN_SPEED   0     // Not moving
#define SPEED_DEC   5     // Decrement value for speed (when slowing down)
#define TURN_SPEED  175   // The speed of the robot when turning

// Threshold values
#define BLACK_THRESHOLD     500   // Threshold for optical sensor - below this value means sensor is reading black (F2)
#define STOP_THRESHOLD      45.00 // Below this value and the robot starts slowing down (F1)
#define SCAN_THRESHOLD      10.00 // Below this value and the robot will definitely not turn in this direction (F1)
#define SLOW_DOWN_TIME      1000  // How many milliseconds it will take for the robot to slow down (F1)
#define F1_TURN_TIME        300   // How many milliseconds the robot will turn (F1)
#define HALL_THRESHOLD      50    // Below this value means a magnet is detected (F1)
#define ADJUST_THRESHOLD    200   // Threshold for adjusting the current speed of the motors - above = decrease, below = increase (F1)

// Robot directions (i.e. enumerating directions)
#define FORWARD   0
#define RIGHT     1
#define LEFT      2
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
int sensorf_value, sensorl_value, sensorr_value = 0;
int currentServoDegrees = 90;   // Servo is initially facing forwards
int current_left_speed = MAX_SPEED;
int current_right_speed = MAX_SPEED;

int SIZE_IN = 5;
int SIZE_OUT = 3;
int inputPins[]  = {ECHO, SENSOR_F, SENSOR_L, SENSOR_R, SWITCH_FUNC_PIN};
int outputPins[] = {M1_DIR_PIN, M2_DIR_PIN, TRIGGER};

// initialize the library with the number of the interface pins for the LCD
LiquidCrystal lcd(RS_pin, Enable_pin, D4, D5, D6, D7);

void setup() {
  int i;  // Counter variable used for initializing pins later in the setup() function
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

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

    followLine();
  }
 
  //move_forward(MAX_SPEED);
  //Serial.println(digitalRead(SWITCH_FUNC_PIN));
  //delay(100);
  //followLine();
  //prncp_func1();
  //readSensor();
}

// Executes the first principle function

void prncp_func1() {
  scanningServo.write(90);
  set_motors(FORWARD);

  Serial.print(current_left_speed);
  Serial.print("\t");
  Serial.println(current_right_speed);

  if (current_left_speed < 125 || current_right_speed < 125) {
    current_left_speed = 255;
    current_right_speed = 255;
    analogWrite(M1_SPEED_PIN, 255);//current_left_speed);
    analogWrite(M2_SPEED_PIN, 255);//current_right_speed);
  } else {
    analogWrite(M1_SPEED_PIN, current_left_speed);
    analogWrite(M2_SPEED_PIN, current_right_speed);
  }

  int left_speed = current_left_speed;
  int right_speed = current_right_speed;

  if (distanceFromSensor() > STOP_THRESHOLD) {
    //adjustCourse();
    move_forward(MAX_SPEED);
  }

  if (distanceFromSensor() <= STOP_THRESHOLD) {
    float distance = distanceFromSensor();
    if (distance > STOP_THRESHOLD) {
      distance = STOP_THRESHOLD;
    }
    
    slow_down(SLOW_DOWN_TIME * ((int) (distance / STOP_THRESHOLD)));
    current_left_speed = left_speed;
    current_right_speed = right_speed;
    
    int turnDirection = scanEnvironment();
    int i;

    switch (turnDirection) {
      case LEFT:
        for (i = 0; i < F1_TURN_TIME; i++) {
          turn_robot(LEFT, TURN_SPEED);
          if(digitalRead(SWITCH_FUNC_PIN) == HIGH)
          {
            return;
          }
        }
        break;
      case RIGHT:
        for (i = 0; i < F1_TURN_TIME; i++) {
          turn_robot(RIGHT, TURN_SPEED);
          if(digitalRead(SWITCH_FUNC_PIN) == HIGH)
          {
            return;
          }
        }
        break;
      case BACKWARDS:
      default:
        set_motors(BACKWARDS);
        analogWrite(M1_SPEED_PIN, current_left_speed);
        analogWrite(M2_SPEED_PIN, current_right_speed);
        delay(1000);
        if(digitalRead(SWITCH_FUNC_PIN) == HIGH)
         {
            return;
         }
        stop_motors();
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
 * Param: speed - the speed to set both motors (values ranging from MIN_SPEED to MAX_SPEED)
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

/* 
 * Slows down robot from MAX_SPEED to MIN_SPEED for a specified duration.  
 * 
 * Param: time - duration over which robot slows down (in ms)
 */

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

/*
 * Stops the motors from turning
 */
void stop_motors() {
  analogWrite(M1_SPEED_PIN, MIN_SPEED);
  analogWrite(M2_SPEED_PIN, MIN_SPEED);
}

/*
 * Set the motors for a desired direction for the robot
 * 
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

  // We will not be using the temperature pin to make out period measurements more accurate
  // It is unlikely that temperature will fluctuate significantly in the test environment, and
  // a change of 10 degrees celcius will not impact the distance measurement significantly
  float period = 58.0;    // Assuming speed of sound is approximately 343 meters per second
  delay(50);

  // Return the distance (in centimeters)
  return ((float) echoTime / period);
}

//------------------------------------------------
// FUNCTIONALITY 2 HELPER FUNCTIONS
//------------------------------------------------

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
        move_forward(175);
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
              move_forward(175);
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
// FUNCTIONALITY 1 HELPER FUNCTIONS
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
    delay(10);
  }

  // Measure the distance to the nearest object
  delay(500);
  rightDistance = distanceFromSensor();
  delay(500);

  // Smoothly turn the sensor to face the right
  while (currentServoDegrees < 180) {
    scanningServo.write(++currentServoDegrees);
    delay(10);
  }

  // Measure the distance to the nearest object
  delay(500);
  leftDistance = distanceFromSensor();
  delay(500);

  // Smoothly return the sensor to face forwards
  while (currentServoDegrees > 90) {
    scanningServo.write(--currentServoDegrees);
    delay(10);
  }
  delay(500);

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
}

/*
 * Uses the hall-effect sensors (one on each wheel) to help the robot
 * move straight. Measures the period of each wheel and then appropriately
 * adjusts the speed of one of the wheels. 
 */

void adjustCourse() {
  long start_left = 0;  // Used to measure period of left wheel
  long start_right = 0; // Used to measure period of right wheel
  long period_left = 0;
  long period_right = 0;

  int left_measure;
  int right_measure;

  Serial.print("Left measure:\t");
  Serial.print(left_measure);
  Serial.print("\t\tRight measure:\t");
  Serial.println(right_measure);

  while (true) {
    left_measure = analogRead(LEFT_WHEEL);
    right_measure = analogRead(RIGHT_WHEEL);

    if (start_left   == 0 && left_measure  < HALL_THRESHOLD) start_left   = millis();
    if (start_right  == 0 && right_measure < HALL_THRESHOLD) start_right  = millis();

    // NOTE: 200 is used below because the fastest possible period is 450 milliseconds (200 is less than 450),
    // yet the wheel can spin enough in 200 milliseconds to pass the hall effect sensor wihtout being read
    // twice before completing a full revolution
    if (period_left  == 0 && left_measure  < HALL_THRESHOLD && millis() - start_left  > 200) {
      period_left  = millis() - start_left;
    }

    if (period_right == 0 && right_measure < HALL_THRESHOLD && millis() - start_right > 200) {
      period_right = millis() - start_right;
    }

    int period_diff = abs(period_left - period_right);

    // once both periods have been measured...
    if (period_left != 0 && period_right != 0) {

      // if both wheels are going a bit too slow...
      if ((current_left_speed < ADJUST_THRESHOLD) && (current_right_speed < ADJUST_THRESHOLD)) {

        // left wheel is slower?
        if (period_left > period_right) {
          //speedup values were empirically determined
          if (period_diff > 200)
            speedup_motor(LEFT, 17);
          else if (period_diff > 150)
            speedup_motor(LEFT, 14);
          else if (period_diff > 100)
            speedup_motor(LEFT, 11);
          else if (period_diff > 50)
            speedup_motor(LEFT, 8);
          else if (period_diff > 5)
            speedup_motor(LEFT, 5);
        }
        
        // right wheel is slower
        else {
          if (period_diff > 200)
            speedup_motor(RIGHT, 17);
          else if (period_diff > 150)
            speedup_motor(RIGHT, 14);
          else if (period_diff > 100)
            speedup_motor(RIGHT, 11);
          else if (period_diff > 50)
            speedup_motor(RIGHT, 8);
          else if (period_diff > 5)
            speedup_motor(RIGHT, 5);
        }
      }

        // otherwise, slow one of the wheels
      else {
        
        // left wheel is slower?
        if (period_left > period_right) {
          
          //slowdown values were empirically determined
          if (period_diff > 200)
            slowdown_motor(RIGHT, 17);
          else if (period_diff > 150)
            slowdown_motor(RIGHT, 14);
          else if (period_diff > 100)
            slowdown_motor(RIGHT, 11);
          else if (period_diff > 50)
            slowdown_motor(RIGHT, 8);
          else if (period_diff > 5)
            slowdown_motor(RIGHT, 5);
        }
        
        // right wheel is slower
        else {
          if (period_diff > 200)
            slowdown_motor(LEFT, 17);
          else if (period_diff > 150)
            slowdown_motor(LEFT, 14);
          else if (period_diff > 100)
            slowdown_motor(LEFT, 11);
          else if (period_diff > 50)
            slowdown_motor(LEFT, 8);
          else if (period_diff > 5)
            slowdown_motor(LEFT, 5);
        }
      }
    }
    break;
  }
}

void readSensor()
{
// put your main code here, to run repeatedly
  int analogF = analogRead(SENSOR_F);
  int analogL = analogRead(SENSOR_L);
  int analogR = analogRead(SENSOR_R);
  
  //int digital = digitalRead(10);
  
  Serial.print(analogL);
  Serial.print("\t\t");
  Serial.print(analogF);
  Serial.print("\t\t");
  Serial.print(analogR);
  Serial.println();
  delay(100);

}


