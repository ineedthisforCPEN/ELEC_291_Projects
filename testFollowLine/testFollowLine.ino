#include <Servo.h>

// Speed constants
#define MAX_SPEED 255
#define MIN_SPEED 0 // not moving
#define SPEED_DEC 5 // decrement value for speed (when slowing down)

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

#define SENSOR_F A2
#define SENSOR_L A4
#define SENSOR_R A3
#define BLACK_THRESHOLD 100

#define STOP_THRESHOLD 40.00
#define SCAN_THRESHOLD 10.00
#define SLOW_DOWN_TIME 1000

Servo scanningServo;
bool leave = false;
int sensorf_value, sensorl_value, sensorr_value = 0;
int currentServoDegrees = 90;   // Servo is initially facing forwards
int current_speed = 0;

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
  
  
  followLine();
  //readSensor();
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
  else if (direction == LEFT) {
    digitalWrite(M1_DIR_PIN, LOW);
    digitalWrite(M2_DIR_PIN, HIGH);
  }
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
    if(sensorf_value <= BLACK_THRESHOLD)
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
        if(sensorl_value <= BLACK_THRESHOLD && sensorr_value > BLACK_THRESHOLD)
        {
              turn_to_check_path(LEFT);
        }

        //if the right sensor detects the path AND the left sensor does not, turn the robot to the right until the front sensor detects the path again,
        //and then continue moving forward
        else if(sensorl_value > BLACK_THRESHOLD && sensorr_value <= BLACK_THRESHOLD)
        {
              turn_to_check_path(RIGHT);
        }

        //if neither sensor detects the path, move the robot forward a tiny bit and try again
        else if(sensorl_value > BLACK_THRESHOLD && sensorr_value > BLACK_THRESHOLD)
        {
              move_forward(MAX_SPEED/4);
              delay(20);
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
    if(analogRead(SENSOR_F) <= BLACK_THRESHOLD)
    {
      //move_forward(MAX_SPEED/4);
      break;
    }
    
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


