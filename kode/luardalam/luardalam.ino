//sensor luar
const int irJendelaLuarPin = 4; //pin D2
long irJendelaLuarValue;

//sensor luar
const int pirPintuLuarPin = 16; //pin D0
long pirPintuLuarValue;

//sensor dalam
const int irPintuDalamPin = 12; //pin D6
long irPintuDalamValue;

//sensor dalam
const int pirPintuDalamPin = 14; //pin D5
long pirPintuDalamValue;

const int pirJendelaDalamPin = 13; //pin D7
long pirJendelaDalamDalamValue;

//output
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
    Serial.println("Deteksi Luar Jendela");
//    isAlarm = true;
    pesan = "Deteksi Luar Jendela";
  } else {
//    Serial.println("Clear");
  }

  pirPintuLuarValue = digitalRead(pirPintuLuarPin);

  if (pirPintuLuarValue == HIGH) {
    Serial.println("Deteksi Luar Pintu");
//    isAlarm = true;
    pesan = "Deteksi Luar Pintu";
  } else {
//    Serial.println("Clear");
  }
  //  sendNotification(pesan);
}

void deteksiDalam() {
  irPintuDalamValue = digitalRead(irPintuDalamPin);

  if (irPintuDalamValue == LOW) {
    Serial.println("Deteksi Dalam Pintu");
    pesan = "Deteksi Dalam Pintu";
    isAlarm = true;
    //    lampuDanBuzzer(true);
  } else {
//    Serial.println("Clear");
    //        isAlarm = false;
  }

  pirPintuDalamValue = digitalRead(pirPintuDalamPin);
  pirJendelaDalamDalamValue = digitalRead(pirJendelaDalamPin);

  if (pirPintuDalamValue == HIGH) {
    Serial.println("Deteksi Dalam Pintu");
    pesan = "Deteksi Dalam Pintu";
    isAlarm = true;
  } else if (pirJendelaDalamDalamValue == HIGH) {
    Serial.println("Deteksi Dalam Jendela");
    pesan = "Deteksi Dalam Jendela";
    isAlarm = true;
  } else {
//    Serial.println("Clear");
    //    isAlarm = false;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
      long currentMillis = millis();
deteksiLuar();
deteksiDalam();

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
