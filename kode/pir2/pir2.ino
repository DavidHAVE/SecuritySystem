//sensor luar
const int pirPintuLuarPin = 16; //pin D0
long pirPintuLuarValue;

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
  pirPintuLuarValue = digitalRead(pirPintuLuarPin);

  if (pirPintuLuarValue == HIGH) {
    Serial.println("Deteksi Luar Pintu");
    pesan = "Deteksi Luar Pintu";
  } else {
    Serial.println("Clear");
  }
  //  sendNotification(pesan);
}

void deteksiDalam() {
  long currentMillis = millis();
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
    Serial.println("Clear");
    //    isAlarm = false;
  }

//  if ((isBuzzer == false) && (currentMillis - previousMillis >= 1000)) {
//    if (isAlarm) {
//      Serial.println("Membunyikan Sirine");
//      digitalWrite(buzzerDanLampuPin, HIGH);
//      //      sendNotification(pesan);
//      isBuzzer == true;
//      previousMillis = currentMillis;
//    }
//  } else if ((isBuzzer == true) && (currentMillis - previousMillis >= 10000)) {
//    //    Serial.println("Mematikan Sirine");
//    digitalWrite(buzzerDanLampuPin, LOW);
//    isAlarm = false;
//    isBuzzer == false;
//    previousMillis = currentMillis;
//  }
}



void loop() {
  // put your main code here, to run repeatedly:
  deteksiLuar();
  deteksiDalam();

}
