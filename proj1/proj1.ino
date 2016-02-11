#include <Servo.h> 

#define LM35Pin 6
#define trigPin 9   //set digital I/O pin 5 to Trig terminal on HC_SR04
#define echoPin 8   //set digital I/O pin 6 to Echo terminal on HC_SR04

float duration; //stores the length of the pulse received from ECHO. 
float distance = 0;
float betterDistance = 0;
float LM35Temp;         //temperature measured by LM35 sensor
float stime = 0;
float ptime = 0;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int left = 180; 
int right = 0; 
int front = 90;

//int[3] dir = {front, left, right};
void setup() {
  myservo.attach(10);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LM35Pin, INPUT);
}

void loop() {
  /*
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

*/

  myservo.write(front);
  generateDistance();
  delay(1000);
  myservo.write(left);
  generateDistance();
  delay(1000);
  myservo.write(right);
  generateDistance();
  delay(1000);
}

void generateDistance()
{
  LM35Temp = (500.0 * analogRead(LM35Pin)) / 1024; //obtain the temperature in degrees celsius
  stime = 331.5+(0.6*LM35Temp); //obtain the speed of sound in m/s
  ptime = 1/(stime/20000); //convert the speed of sound to cm/us and find the recriprocal
  digitalWrite(trigPin, HIGH); //set a 10us pulse as a trigger signal
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); //calculate the duration of the pulse time
  distance = duration/ptime; //calculate the distance of the object from the sensor
  Serial.println(distance); //print the value onto the serial monitor
  //Serial.print(",");
}


