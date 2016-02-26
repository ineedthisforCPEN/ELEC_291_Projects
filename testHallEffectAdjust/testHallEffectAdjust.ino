int leftWheel = 11;
int rightWheel = 7;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(leftWheel, INPUT);
  pinMode(rightWheel, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long startTime;
  long stopTime;
  long threshold = 2000;
  int slowerWheel;

  Serial.print(digitalRead(leftWheel));
  Serial.println(digitalRead(rightWheel));
  /*
  while (digitalRead(leftWheel) == 1 && digitalRead(rightWheel) == 1) {
    Serial.print(digitalRead(leftWheel));
    Serial.print("\t");
    Serial.println(digitalRead(rightWheel));
  }

  switch (digitalRead(rightWheel)) {
    case 0:
      slowerWheel = rightWheel;
      Serial.println("LEFT");
      break;
    case 1:
    default:
      slowerWheel = leftWheel;
      Serial.println("RIGHT");
      break;
  }

  startTime = millis();

  while (digitalRead(slowerWheel) == 1) {
    Serial.println("waiting...");
    continue;
  }

  stopTime = millis();

  if (stopTime - startTime > threshold) {
    adjust();
    Serial.println("adjust");
  } else {
    Serial.println("don't adjust");
  }*/
}

void adjust(void) {
}

