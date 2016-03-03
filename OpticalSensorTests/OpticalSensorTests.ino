#define FRONT A4
#define LEFT A3
#define RIGHT A5

void setup() {
  // put your setup code here, to run once:
  pinMode(FRONT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
  //pinMode(10, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly
  int analogF = analogRead(FRONT);
  int analogL = analogRead(LEFT);
  int analogR = analogRead(RIGHT);
  
  //int digital = digitalRead(10);
  
  Serial.print(analogL);
  Serial.print("\t\t");
  Serial.print(analogF);
  Serial.print("\t\t");
  Serial.print(analogR);
  Serial.println();
  delay(100);
}

/*
 * Number output is higher when the sensor is over a dark line
 * Number output is lower when the sensor is over a light surface
 * 
 * Number skyrockets when the sensor makes contact with the surface
 */
