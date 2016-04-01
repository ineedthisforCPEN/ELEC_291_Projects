/* Include the DHT library */
#include "DHT.h"
#define ECHO          11  // Arduino pin to which the ultrasonic sensor's echo pin is connected
#define TRIGGER       12  // Arduino pin to which the ultrasonic sensor's trigger pin is connected

/* Variables for pins */
const int humidityPin = 7;
const int temperaturePin = A0;
// Set the RGB LED pins
const int rPin = 9; 
const int bPin = 12; 
const int gPin = 11; 


const int buttonPin = 8;     //  pushbutton pin number

int buttonState = 0;         // record the push button status



/* Initialize the DHT */
DHT dht(humidityPin, DHT11);

void setup() {
  Serial.begin(9600);
  
  pinMode(humidityPin, INPUT);
  pinMode(temperaturePin, INPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(rPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  digitalWrite(rPin, HIGH);
  digitalWrite(bPin, HIGH);
  digitalWrite(gPin, HIGH);


  //push button input
  pinMode(buttonPin, INPUT);
}

void loop() {  
  // Measure humidity, temperature and distance levels
  // Print each value on a separate to be read through Python script on Raspberry Pi
  float humidity = dht.readHumidity();
  float temperature = avgUnits(temperaturePin, 500) * (5.0 / 1024.0) * 100.0;    // Temperature in degrees celcius
  float distance = distanceFromSensor();
    Serial.println(humidity);
    Serial.println(temperature);
    Serial.println(distance);

    // Take input from the Serial Monitor (Used to let the Raspberry Pi control the output of the RGB LED)
    if (Serial.available())  {
    int a = Serial.read() - '0'; // Converts from ASCII character 1-9 to decimal 1-9
    if( a == 1)
     flashGreen();  // Flash green on the RGB LED
    else
     flashRed();  // Flash red on the RGB LED



     
      // detect if the button is pressed:
  buttonState = digitalRead(buttonPin);

  // if the button is pressed, turn on voice command
  if (buttonState == HIGH) {
    Serial.print ("ON");
  } 
  else {
    Serial.print("OFF");
  }
     
  }
}

/*
 * This function performs analogRead on a pin and returns the average units measured.
 * One unit (u) is:
 *      u = Vc / 1024
 * 
 * Parameter: pin - the pin on which to perform the read
 * Parameter: wait - the period of measurement in milliseconds
 * 
 * Returns: the average size of units measured
 */
float avgUnits(int pin, int wait) {
  long startTime = millis();                              // Get current uptime of Arduino board
  long totalMilliVolts = 0;                               // Long for storing total millivolts (long to prevent overflow)
  long numMeasures = 0;                                   // Long for storing number of measurements (long to prevent overflow)

  while (millis() - startTime < wait) {                   // Measure for one second
    totalMilliVolts += analogRead(pin);                   // Add to total millivolts measures
    numMeasures++;                                        // Increment measure counter
  }

  return ((float) totalMilliVolts)/((float) numMeasures); // Return average millivolt measurement
}

float distanceFromSensor(void) {
  // Send a start signal to the trigger pin
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  // Measure how long it takes the echo signals to return
  unsigned long echoTime = pulseIn(ECHO, HIGH);

  // Build in delay with calculatePeriod(), so we do not need to delay an extra 50ms after calling this function
  float period = 58.0; //calculatePeriod();

  // Return the distance (in centimeters)
  float distance = (float) echoTime / period;

  // Saturation point for any fluctuating values
  if (distance > 1000) {
    return 1000.0;
  }
  return distance;
}

/*
 * This function flashes the RGB LED red 
 */
void flashRed(){
  for(int a = 0; a < 5; a++){
    digitalWrite(rPin, LOW); // Turn on the red LED
    digitalWrite(bPin, HIGH);
    digitalWrite(gPin, HIGH);   
   /* digitalWrite(rPin, HIGH);
    digitalWrite(bPin, HIGH);
    digitalWrite(gPin, HIGH);   
    delay(500);}*/
  }
 }
/*
 * This function flashes the RGB LED green 
 */
void flashGreen(){
  for(int b= 0; b < 5; b++){
    digitalWrite(gPin, LOW); // Turn on the green LED
    digitalWrite(bPin, HIGH);
    digitalWrite(rPin, HIGH);   
    /*delay(2000);
    digitalWrite(rPin, HIGH);
    digitalWrite(bPin, HIGH);
    digitalWrite(gPin, HIGH);   
    delay(500);}*/
  }
}
