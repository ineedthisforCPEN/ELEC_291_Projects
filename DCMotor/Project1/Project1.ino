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
 
void setup() { 
  Serial.begin(9600);
  pinMode(M1_DIR_PIN, OUTPUT);   
  pinMode(M2_DIR_PIN, OUTPUT); 

  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(TEMPERATURE, INPUT);
} 
 
void loop() { 
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
  /*
  stop_motors();
  delay(3000);
  turn_right(3000);
  delay(1000);
  turn_left(3000);
  */
  /*
  int value;
  for(value = 0 ; value <= 255; value+=5) 
  { 
    digitalWrite(M2_DIR_PIN, LOW);   
    //digitalWrite(M2, HIGH);       
    analogWrite(M2_SPEED_PIN, value);   //PWM Speed Control
    //analogWrite(E2, value);   //PWM Speed Control
    delay(30);
  } 
   */
}

// Move robot forward at a given speed ranging from MIN_SPEED to MAX_SPEED

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

