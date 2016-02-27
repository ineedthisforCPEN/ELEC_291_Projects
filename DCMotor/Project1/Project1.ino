//Test Case 1:
//Move forward at MAX_SPEED for one second, then stopping for one second.
move_forward(MAX_SPEED);
delay(1000);
stop_motors();
delay(1000);

#define MAX_SPEED 255
#define MIN_SPEED 0 // not moving
#define SPEED_DEC 5 // decrement value for speed

// Arduino digital pins for controlling DC motors
#define M1_DIR_PIN 4
#define M1_SPEED_PIN 5
#define M2_DIR_PIN 7
#define M2_SPEED_PIN 6

#define FORWARD 0
#define RIGHT 1
#define LEFT 2

#define ECHO 13
#define TRIGGER 12
#define TEMPERATURE A0
#define SENSOR_F A1
#define SENSOR_L A2
#define SENSOR_R A3

int sensorf_value, sensorl_value, sensorr_value = 0;

#define BLACK_THRESHOLD 100

void setup() {
  Serial.begin(9600);
  
  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M2_DIR_PIN, OUTPUT);

  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(TEMPERATURE, INPUT);

  pinMode(SENSOR_F, INPUT);
  pinMode(SENSOR_L, INPUT);
  pinMode(SENSOR_R, INPUT);

}

void loop() {
  //Test Case 2:
  //Turn left at MAX_SPEED for one second, then stopping for one second.
  turn_left(1000);
  delay(1000);
  stop_motors();
  delay(1000);

  //Test Case 3:
  //Turn right at MAX_SPEED for one second, then stopping for one second.
  turn_right(1000);
  delay(1000);
  stop_motors();
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
  float dist;

  move_forward(MAX_SPEED);
  //turn_left(2000);
  //turn_left(150);

  dist = distanceFromSensor();
  Serial.println(dist);
  if (dist < 50.00) {
    slow_down(2000);
    delay(1000);
  }
}

void move_forward(int speed) {
  set_motors(FORWARD);
  analogWrite(M1_SPEED_PIN, speed);
  analogWrite(M2_SPEED_PIN, speed);
}

// Turn robot right 90 degrees from forward direction
// time (ms) specifies the duration of turning (adjust for 90 degree turn)

void turn_right(int time) {
  set_motors(RIGHT);
  analogWrite(M1_SPEED_PIN, MAX_SPEED);
  analogWrite(M2_SPEED_PIN, MAX_SPEED);
  delay(time);
  // once turning is finished, stop motors
  stop_motors();
}

// Turn robot left 90 degrees from forward direction
// time (ms) specifies the duration of turning (adjust for 90 degree turn)

void turn_left(int time) {
  set_motors(LEFT);
  analogWrite(M1_SPEED_PIN, MAX_SPEED);
  analogWrite(M2_SPEED_PIN, MAX_SPEED);
  delay(time);
  // once turning is finished, stop motors
  stop_motors();
}

// Slow down robot from MAX_SPEED to MIN_SPEED
// time (ms) specifies duration over which robot slows down

void slow_down(int time) {
  for (int speed = MAX_SPEED; speed > MIN_SPEED; speed -= SPEED_DEC) {
    analogWrite(M1_SPEED_PIN, speed);
    analogWrite(M2_SPEED_PIN, speed);
    delay(time / (MAX_SPEED / SPEED_DEC));
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
    digitalWrite(M1_DIR_PIN, LOW);
    digitalWrite(M2_DIR_PIN, LOW);
  }
  else if (direction == RIGHT) {
    digitalWrite(M1_DIR_PIN, LOW );
    digitalWrite(M2_DIR_PIN, HIGH);
  }
  else {      // direction == LEFT
    digitalWrite(M1_DIR_PIN, HIGH);
    digitalWrite(M2_DIR_PIN, LOW);
  }
}

//------------------------------------------------------------------------------------------------------

/**
   This function is meant to calculate the sound's period based on the reading from the
   LM35 temperature sensor and lecture slides. It reads the temperature sensor value, then
   returns the calculated period.

   This function takes into account that the sound wave must travel twice the distance
   (hence, 20000.0 is used instead of 10000.0)
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
  return 20000.0 / (331.5 + 0.6 * (((float) totalMilliVolts) / ((float) numMeasures) * (0.415282392)));
  /*--------------------- Temperature in C -----------------------*/
}

/**
   This function determines how far away an object is from the sensor. The calculations
   are based on the formula given in the lecture slides

   Distance in cm = Echo pulse width (in us) / period
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
   This function allows for the robot to follow a guided path using the reflective optical sensors. The sensors are attached at the front of the robot in a triangle formation.
   Algorithm:
             -when front sensor detects black tape, will move forward at _ speed
             -when front sensor is off the black tape, the robot will read the left and right sensors. The robot will turn in the direction of whatever sensor that detects the line, and
              will not stop turning until the front sensor detects the black line. When the line is detected, the robot continues to travel straight. 
             -when no sensors detect the line, the robot will travel forward for 0.5s before it reads the left and right sensors again. 
             -when both sensors detect line, the robot will choose a random direction to spin.         
*/

void followLine() {
 
 while(true) { 
    sensorf_value = analogRead(SENSOR_F);
    if(sensor_value > threshold)
    {
        move_forward(MAX_SPEED);
    }

    else
    {
        spinRobot();
    }

    delay(500);
 }
}

void spinBot() {
  sensorl_value = analogRead(SENSOR_L);
  sensorr_value = analogRead(SENSOR_R);

  

}
}

