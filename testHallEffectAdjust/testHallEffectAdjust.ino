#define LEFT_WHEEL  A4          // The Arduino pin to which the left wheel hall effect sensor is attached
#define RIGHT_WHEEL A5          // The Arduino pin to which the right wheel hall effect sensor is attached
#define HALL_THRESHOLD 50       // The threshold for the hall effect sensor - if below the threshold, magnet is detected
#define HALL_RATIO 50           // The maximum acceptable ratio - if crossed, adjust the path
#define ROTATION_THRESHOLD 20  // Maximum allowable difference in periods of left and right wheel
#define ADJUST_THRESHOLD 200
// Arduino digital pins for controlling DC motors
#define M1_DIR_PIN 4
#define M1_SPEED_PIN 5
#define M2_DIR_PIN 7
#define M2_SPEED_PIN 6

// Robot directions
#define FORWARD 0
#define RIGHT 1
#define LEFT 2

// Speed constants
#define MAX_SPEED 255
#define MIN_SPEED 0 // not moving
#define SPEED_DEC 5 // decrement value for speed (when slowing down)

int current_speed = 0;
int current_left_speed = 0;
int current_right_speed = 0;

/* Function prototypes */
void adjustCourse(void);
float calculateSpeed(int);

void setup() {
  // Initialize serial input and output
  Serial.begin(9600);

  // Initialize pins for the hall effect sensors
  pinMode(LEFT_WHEEL, INPUT);
  pinMode(RIGHT_WHEEL, INPUT);
}

void loop() {
  move_forward(MAX_SPEED);
  while (true) {
    adjustCourse();
  }
}

void adjustCourse() {
  long start_left = 0;  // Used to measure period of left wheel
  long start_right = 0; // Used to measure period of right wheel
  long period_left = 0;
  long period_right = 0;

  int left_measure;
  int right_measure;

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

//------------------------------------------------------------------------------------------------------

// Move robot forward at a given speed


void move_forward(int speed) {
  set_motors(FORWARD);
  current_right_speed = speed;
  current_left_speed = speed;
  analogWrite(M1_SPEED_PIN, speed);
  analogWrite(M2_SPEED_PIN, speed);
}

// Turn robot from forward direction towards input direction for 1 ms

void turn_robot(int direction) {
  set_motors(direction);
  analogWrite(M1_SPEED_PIN, MAX_SPEED);
  analogWrite(M2_SPEED_PIN, MAX_SPEED);
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
  else {      // direction == LEFT
    digitalWrite(M1_DIR_PIN, LOW);
    digitalWrite(M2_DIR_PIN, HIGH);
  }
}
