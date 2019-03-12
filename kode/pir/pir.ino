const int pirPin = 13; //pin D0
long pirValue;

void setup() {
  // put your setup code here, to run once:
  pinMode(pirPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pirValue = digitalRead(pirPin);
  if (pirValue == HIGH) {
    Serial.println("Motion Detected");
    delay(500);
  } else {
    Serial.println("Clear");
    delay(500);
  }
}
