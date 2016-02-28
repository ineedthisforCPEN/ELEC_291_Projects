#define LEFT_WHEEL  A0          // The Arduino pin to which the left wheel hall effect sensor is attached
#define RIGHT_WHEEL A1          // The Arduino pin to which the right wheel hall effect sensor is attached
#define HALL_THRESHOLD 50       // The threshold for the hall effect sensor - if below the threshold, magnet is detected
#define HALL_RATIO 50           // The maximum acceptable ratio - if crossed, adjust the path
#define ROTATION_THRESHOLD 20  // Maximum allowable difference in periods of left and right wheel

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
  delay(1000);
  while(true) {
    adjustCourse(255);
  }
}

void adjustCourse(int currentSpeedVoltage) {
  // Make sure the parameter is within a valid range (i.e. between 0 and 255 inclusive)
  if (currentSpeedVoltage < 0) currentSpeedVoltage = 0;
  else if (currentSpeedVoltage > 255) currentSpeedVoltage = 255;
  
  int slowerWheel;  // The pin whose wheel is going slower
  long start_left = 0;  // Used to measure period of left wheel
  long start_right = 0; // Used to measure period of right wheel
  long period_left = 0;
  long period_right = 0;

  int left_measure;
  int right_measure;

  while (true) {
    //left_measure = analogRead(LEFT_WHEEL);
    right_measure = analogRead(RIGHT_WHEEL);

    //Serial.println(left_measure);
    //Serial.print("\t");
    Serial.println(right_measure);
    delay(250);
    break;

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
      Serial.print("start_left:\t");
      Serial.println(start_left);
      Serial.print("start_right:\t");
      Serial.println(start_right);
      break;
    }
  }

  if (abs(period_left - period_right) <= ROTATION_THRESHOLD) {
    // Don't adjust if the difference is withing the rotation threshold
    return;
  }

  Serial.print("Period of LEFT  wheel: \t");
  Serial.println(period_left);
  Serial.print("Period of RIGHT wheel: \t");
  Serial.println(period_right);
  Serial.println();

  // Calculate how to adjust the motors
  // NOTE: 115.2 was determined empirically (based on max RPM, ratio between voltage and units (0 to 255))
  int changeVoltage = (int) ((115.2 / ((float) period_left - period_right)));
  
  if (period_left > period_right) {
    // Right wheel is faster, slow down the right wheel
    slow_right(changeVoltage);
    Serial.print("Slow down RIGHT motor by ");
    Serial.print(changeVoltage);
    Serial.println(" units");
    Serial.println("----------");
  } else {
    // Left wheel is faster, slow down the left wheel
    slow_left(changeVoltage);
    Serial.print("Slow down LEFT  motor by ");
    Serial.print(-changeVoltage);
    Serial.println(" units");
    Serial.println("----------");
  }
}

//------------------------------------------------------------------------------------------------------

// Move robot forward at a given speed

void move_forward(int speed) {
  set_motors(FORWARD);
  current_speed = speed;
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
    current_speed = speed;
    analogWrite(M1_SPEED_PIN, speed);
    analogWrite(M2_SPEED_PIN, speed);
    delay(time / (MAX_SPEED / SPEED_DEC));
  }
}

void slow_right(int slow_amount) {
  analogWrite(M2_SPEED_PIN, current_speed - slow_amount);
}

void slow_left(int slow_amount) {
  analogWrite(M1_SPEED_PIN, current_speed - slow_amount);
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

//------------------------------------------------------------------------------------------------------


