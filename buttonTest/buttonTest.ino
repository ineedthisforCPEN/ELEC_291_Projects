
const int buttonPin = 8;     //  pushbutton pin number

int buttonState = 0;         // record the push button status


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//push button inputs and outputs 
  pinMode(buttonPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


 

   // detect if the button is pressed:
  buttonState = digitalRead(buttonPin);

  // if the button is pressed, turn on voice command
 
  if (buttonState == HIGH) {
    Serial.println ("ON");
  } 
  
  else {
    Serial.println("OFF");
  }
     
}
