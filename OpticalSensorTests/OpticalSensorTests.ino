void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  //pinMode(10, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly
  int analog = analogRead(A0);
  //int digital = digitalRead(10);
  
  Serial.print(analog);
  Serial.print("\t\t");
  //Serial.print(digital);
  Serial.println();
  delay(100);
}

/*
 * Number output is higher when the sensor is over a dark line
 * Number output is lower when the sensor is over a light surface
 * 
 * Number skyrockets when the sensor makes contact with the surface
 */
