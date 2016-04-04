/* Include the DHT library */
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <math.h>

#define ECHO          6  // Arduino pin to which the ultrasonic sensor's echo pin is connected
#define TRIGGER       10  // Arduino pin to which the ultrasonic sensor's trigger pin is connected

#define HORIZONTAL 3
#define VERTICAL 4

/* Variables for pins */
const int humidityPin = 7;
const int temperaturePin = A0;
// Set the RGB LED pins
const int rPin = 9;
const int bPin = 12;
const int gPin = 11;


const int buttonPin = 8;     //  pushbutton pin number

int buttonState = 0;         // record the push button status
int position = HORIZONTAL;
float distance = 0;



/* Initialize the DHT */
DHT dht(humidityPin, DHT11);

Adafruit_MMA8451 mma = Adafruit_MMA8451();

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
  if (! mma.begin()) {
    while (1);
  }

  mma.setRange(MMA8451_RANGE_2_G);
}

void loop() {
  // Measure humidity, temperature and distance levels
  // Print each value on a separate to be read through Python script on Raspberry Pi
  float humidity = dht.readHumidity();
  while(isnan(humidity)){
    humidity = dht.readHumidity();
  }
  float temperature = avgUnits(temperaturePin, 500) * (5.0 / 1024.0) * 100.0;    // Temperature in degrees celcius
  distance = distanceFromSensor();
  while (distance == 1000) {
    distance = distanceFromSensor();
  }
  

  // detect if the button is pressed:
  buttonState = digitalRead(buttonPin);

  // if the button is pressed, turn on voice command

  mma.read();

  /* Get a new sensor event */
  sensors_event_t event;
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */

  /* Get the orientation of the sensor */
  uint8_t o = mma.getOrientation();

  switch (o) {
    case MMA8451_PL_PUF:
      position = HORIZONTAL;
      break;
    case MMA8451_PL_PUB:
      position = HORIZONTAL;
      break;
    case MMA8451_PL_PDF:
      break;
    case MMA8451_PL_PDB:
      break;
    case MMA8451_PL_LRF:
      break;
    case MMA8451_PL_LRB:
      break;
    case MMA8451_PL_LLF:
      position = VERTICAL;
      break;
    case MMA8451_PL_LLB:
      position = VERTICAL;
      break;
  }
  // Take input from the Serial Monitor (Used to let the Raspberry Pi control the output of the RGB LED)
  if (Serial.available() > 0)  {
    byte firstByte = Serial.read() - '0' ;
    byte secondByte = Serial.read() - '0';
    byte sensorByte = firstByte;
    if (secondByte < 100) {
      sensorByte = firstByte * 10 + secondByte;
    }

    if (sensorByte & 0x01) {
      Serial.print(temperature);
      Serial.print("|");
    }
    if ((sensorByte >> 1) & 0x01) {
      Serial.print(humidity);
      Serial.print("|");
    }
    if ((sensorByte >> 2) & 0x01) {
      Serial.print(distance);
      Serial.print("|");
    }
    if ((sensorByte >> 3 ) & 0x01) {
      Serial.print(position);
      Serial.print("|");
    }
    if ((sensorByte >> 4) & 0x01) {
      Serial.print(buttonState);
      Serial.print("|");
    }
    if ((sensorByte >> 5) & 0x01) {
      if ((sensorByte >> 6) & 0x01) {
        flashGreen();
      }
      else {
        flashRed();
      }
    }
  Serial.print("\n");

}
}

/*
   This function performs analogRead on a pin and returns the average units measured.
   One unit (u) is:
        u = Vc / 1024

   Parameter: pin - the pin on which to perform the read
   Parameter: wait - the period of measurement in milliseconds

   Returns: the average size of units measured
*/
float avgUnits(int pin, int wait) {
  long startTime = millis();                              // Get current uptime of Arduino board
  long totalMilliVolts = 0;                               // Long for storing total millivolts (long to prevent overflow)
  long numMeasures = 0;                                   // Long for storing number of measurements (long to prevent overflow)

  while (millis() - startTime < wait) {                   // Measure for one second
    totalMilliVolts += analogRead(pin);                   // Add to total millivolts measures
    numMeasures++;                                        // Increment measure counter
  }

  return ((float) totalMilliVolts) / ((float) numMeasures); // Return average millivolt measurement
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
   This function flashes the RGB LED red
*/
void flashRed() {
    digitalWrite(rPin, LOW); // Turn on the red LED
    digitalWrite(bPin, HIGH);
    digitalWrite(gPin, HIGH);
    delay(3000);
    digitalWrite(rPin, HIGH);
    /* digitalWrite(rPin, HIGH);
      digitalWrite(bPin, HIGH);
      digitalWrite(gPin, HIGH);
      delay(500);}*/
   
}
/*
   This function flashes the RGB LED green
*/
void flashGreen() {
    digitalWrite(gPin, LOW); // Turn on the green LED
    digitalWrite(bPin, HIGH);
    digitalWrite(rPin, HIGH);
    delay(3000);
    digitalWrite(gPin, HIGH);
    /*
      digitalWrite(rPin, HIGH);
      digitalWrite(bPin, HIGH);
      digitalWrite(gPin, HIGH);
      delay(500);}*/
  
}
