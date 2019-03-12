const int irPin = 5;
long irValue;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(irPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
irValue = digitalRead(irPin);
if(irValue == HIGH){
  Serial.println("Motion Detected");
}else{
  Serial.println("Clear");
}
}
