// Arduino digital pins for controlling DC motors
#define M1_DIR_PIN 4
#define M1_SPEED_PIN 5
#define M2_DIR_PIN 7
#define M2_SPEED_PIN 6
#define FORWARD 1
#define RIGHT 2
#define LEFT 3

int current_speed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  set_motors(FORWARD);
  analogWrite(M1_SPEED_PIN, 255);
  analogWrite(M2_SPEED_PIN, 230);
  
  // put your main code here, to run repeatedly:
  int leftFlag = 0;
  int rightFlag = 0;
  int leftCount = 0;
  int rightCount = 1;
  int leftStart = millis();
  int rightStart = millis();

  while (true) {
    if (leftFlag == 0 && analogRead(A0) < 100) {
      leftCount++;
      leftFlag = 1;
      Serial.print("LEFT:\t");
      Serial.println((millis() - leftStart) / leftCount);
    }

    if (leftFlag == 1 && analogRead(A0) > 100) {
      leftFlag = 0;
    }

    if (rightFlag == 0 && analogRead(A1) < 100) {
      rightCount++;
      rightFlag = 1;
      Serial.print("RIGHT:\t");
      Serial.println((millis() - rightStart) / rightCount);
    }

    if (rightFlag == 1 && analogRead(A1) > 100) {
      rightFlag = 0;
    }
  }
}

void move_forward(int speed) {
  set_motors(FORWARD);
  current_speed = speed;
  analogWrite(M1_SPEED_PIN, speed);
  analogWrite(M2_SPEED_PIN, speed);
}

void set_motors(int direction) {
  if (direction == FORWARD) {
    digitalWrite(M1_DIR_PIN, HIGH);
    digitalWrite(M2_DIR_PIN, HIGH);
  }
  else if (direction == RIGHT) {
    digitalWrite(M1_DIR_PIN, HIGH );
    digitalWrite(M2_DIR_PIN, LOW);
  }
  else {      // direction == LEFT
    digitalWrite(M1_DIR_PIN, LOW);
    digitalWrite(M2_DIR_PIN, HIGH);
  }
}
