//sensor luar
const int irJendelaLuarPin = 4; //pin D2
long irJendelaLuarValue;

//sensor luar
const int pirPintuLuarPin = 16; //pin D0
long pirPintuLuarValue;

//outputbhn
const int buzzerDanLampuPin = 15; //pin D8

String pesan;
boolean isAlarm = false;
boolean isBuzzer = false;
long previousMillis;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

   pinMode(buzzerDanLampuPin, OUTPUT);
}

void deteksiLuar() {
  irJendelaLuarValue = digitalRead(irJendelaLuarPin);

 if (irJendelaLuarValue == LOW) {
    Serial.println("Deteksi IR Luar Jendela");
    isAlarm = true;
    pesan = "Deteksi IR Luar Jendela";
  } else {
    Serial.println("Clear");
  }

  pirPintuLuarValue = digitalRead(pirPintuLuarPin);

  if (pirPintuLuarValue == HIGH) {
    Serial.println("Deteksi PIR Luar Pintu");
    isAlarm = true;
    pesan = "Deteksi PIR Luar Pintu";
  } else {
    Serial.println("Clear");
  }
  //  sendNotification(pesan);
}

void loop() {
  // put your main code here, to run repeatedly:
      long currentMillis = millis();
deteksiLuar();

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
