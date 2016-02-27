#define LEFT_WHEEL  A1      // The Arduino pin to which the left wheel hall effect sensor is attached
#define RIGHT_WHEEL A0      // The Arduino pin to which the right wheel hall effect sensor is attached
#define HALL_THRESHOLD 50   // The threshold for the hall effect sensor - if below the threshold, magnet is detected
#define HALL_RATIO 50       // The maximum acceptable ratio - if crossed, adjust the path

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
  
}

void adjustCourse(int currentSpeedVoltage) {
  // Make sure the parameter is within a valid range (i.e. between 0 and 255 inclusive)
  if (currentSpeedVoltage < 0) currentSpeedVoltage = 0;
  else if (currentSpeedVoltage > 255) currentSpeedVoltage = 255;
  
  int slowerWheel;  // The pin whose wheel is going slower
  long start_left;  // Used to measure period of left wheel
  long start_right; // Used to measure period of right wheel
  long period_left;
  long period_right;

  int left_measure;
  int right_measure;

  while (true) {
    left_measure = analogRead(LEFT_WHEEL);
    right_measure = analogRead(RIGHT_WHEEL);

    if (start_left   == NULL && left_measure  < HALL_THRESHOLD) start_left   = millis();
    if (start_right  == NULL && right_measure < HALL_THRESHOLD) start_right  = millis();
    
    // NOTE: 200 is used below because the fastest possible period is 450 milliseconds (200 is less than 450),
    // yet the wheel can spin enough in 200 milliseconds to pass the hall effect sensor wihtout being read
    // twice before completing a full revolution
    if (period_left  != NULL && left_measure  < HALL_THRESHOLD && millis() - start_left  > 200)
      period_left  = millis() - start_left;
    if (period_right != NULL && right_measure < HALL_THRESHOLD && millis() - start_right > 200)
      period_right = millis() - start_right;

    // If both periods have been measured, leave this while loop
    if (period_left != NULL && period_right != NULL) break;
  }
}

float calculateSpeed(int voltage) {
  // Max speed is 61 cm per second - hence 61.0
  // Maximum voltage that can be applied to the motor is 6 volts (255 units - hence 255.0)
  return (61.0 * ((float) voltage/255.0));
}
