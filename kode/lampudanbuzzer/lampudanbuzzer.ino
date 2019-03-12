//output
const int buzzerDanLampuPin = 15; //pin D8

String pesan;
boolean isAlarm = true;
boolean isBuzzer = false;
long previousMillis;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzerDanLampuPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long currentMillis = millis();
  //        digitalWrite(buzzerDanLampuPin, HIGH); //Hidup

  if (isAlarm) {
    if ((isBuzzer == false) && (currentMillis - previousMillis >= 4000)) {
      Serial.println("1");
      Serial.println("Membunyikan Sirine");
      digitalWrite(buzzerDanLampuPin, HIGH); //Mati
      //      delay(8000);
      //      sendNotification(pesan);
      isBuzzer = true;
      previousMillis = currentMillis;

    } else if ((isBuzzer == true) && (currentMillis - previousMillis >= 8000)) {
      Serial.println("Mematikan Sirine");
      //      digitalWrite(buzzerDanLampuPin, LOW);
      Serial.println("2");
      digitalWrite(buzzerDanLampuPin, LOW); //Hidup
      isAlarm = false;
      isBuzzer = false;
      previousMillis = currentMillis;
    }
  }
}
