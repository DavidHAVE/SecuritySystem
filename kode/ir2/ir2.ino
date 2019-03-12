//sensor luar
//const int irPintuLuarPin = 5; //pin D1
//long irPintuLuarValue;

const int irJendelaLuarPin = 4; //pin D2
long irJendelaLuarValue;

//sensor dalam
const int irPintuDalamPin = 12; //pin D6
long irPintuDalamValue;

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
    pesan = "Deteksi Luar Jendela";
  } else {
    Serial.println("Clear");
  }
  //  sendNotification(pesan);
}

void deteksiDalam() {
  long currentMillis = millis();
  irPintuDalamValue = digitalRead(irPintuDalamPin);

 if (irPintuDalamValue == LOW) {
    Serial.println("Deteksi Dalam Pintu");
    pesan = "Deteksi Dalam Pintu";
//    isAlarm = true;
//    lampuDanBuzzer(true);
  } else {
    Serial.println("Clear");
    //    isAlarm = false;
  }
}

//void lampuDanBuzzer(boolean isAlarm){
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
//}

void loop() {
  // put your main code here, to run repeatedly:
  deteksiLuar();
  deteksiDalam();

}
