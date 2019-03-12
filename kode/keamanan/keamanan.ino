//sensor luar
const int pirPintuLuarPin = 16; //pin D0
long pirPintuLuarValue;
const int irPintuLuarPin = 5; //pin D1
long irPintuLuarValue;

const int irJendelaLuarPin = 4; //pin D2
long irJendelaLuarValue;

//sensor dalam
const int pirPintuDalamPin = 14; //pin D5
long pirPintuDalamValue;
const int irPintuDalamPin = 12; //pin D6
long irPintuDalamValue;

const int pirJendelaDalamPin = 13; //pin D7
long pirJendelaDalamDalamValue;

//output
const int buzzerDanLampuPin = 15; //pin D8

String pesan;
boolean isAlarm = false;
boolean isBuzzer = false;
long previousMillis;

#include <ESP8266WiFi.h>

// PushingBox scenario DeviceId code and API
String deviceId = "vEBE5398D26E6C78";
const char* logServer = "api.pushingbox.com";

const char* ssid = "HOME";
const char* password = "1234h0me";

void setup() {
  Serial.begin(74880);

  pinMode(buzzerDanLampuPin, OUTPUT);
  // Sending a notification to your mobile phone
  // function takes the message as a parameter
}

void deteksiLuar() {
  pirPintuLuarValue = digitalRead(pirPintuLuarPin);
  irPintuLuarValue = digitalRead(irPintuLuarPin);
  irJendelaLuarValue = digitalRead(irJendelaLuarPin);

  if (pirPintuLuarValue == HIGH) {
    Serial.println("Deteksi Luar Pintu");
    pesan = "Deteksi Luar Pintu";
  } else if (irPintuLuarValue == HIGH) {
    Serial.println("Deteksi Luar Pintu");
    pesan = "Deteksi Luar Pintu";
  } else if (irJendelaLuarValue == HIGH) {
    Serial.println("Deteksi Luar Jendela");
    pesan = "Deteksi Luar Jendela";
  } else {
    Serial.println("Clear");
  }
  sendNotification(pesan);
}

void deteksiDalam() {
  long currentMillis = millis();
  pirPintuDalamValue = digitalRead(pirPintuDalamPin);
  irPintuDalamValue = digitalRead(irPintuDalamPin);
  pirJendelaDalamDalamValue = digitalRead(pirJendelaDalamPin);

  if (pirPintuDalamValue == HIGH) {
    Serial.println("Deteksi Dalam Pintu");
    pesan = "Deteksi Dalam Pintu";
    isAlarm = true;
  } else if (irPintuDalamValue == HIGH) {
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

  if ((isBuzzer == false) && (currentMillis - previousMillis >= 1000)) {
    if (isAlarm) {
      Serial.println("Membunyikan Sirine");
      digitalWrite(buzzerDanLampuPin, HIGH);
      sendNotification(pesan);
      isBuzzer == true;
      previousMIllis = currentMillis;
    }
  } else if ((isBuzzer == true) && (currentMillis - previousMillis >= 10000)) {
    //    Serial.println("Mematikan Sirine");
    digitalWrite(buzzerDanLampuPin, LOW);
    isAlarm = false;
    isBuzzer == false;
    previousMIllis = currentMillis;
  }
}

void sendNotification(String message) {

  Serial.println("- connecting to Home Router SID: " + String(ssid));

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("- succesfully connected");
  // menuliskan ip address
  Serial.println(WiFi.localIP());
  Serial.println("- starting client");

  WiFiClient client;

  Serial.println("- connecting to pushing server: " + String(logServer));
  if (client.connect(logServer, 80)) {
    Serial.println("- succesfully connected");

    String postStr = "devid=";
    postStr += String(deviceId);
    postStr += "&message_parameter=";
    postStr += String(message);
    postStr += "\r\n\r\n";

    Serial.println("- sending data...");

    client.print("POST /pushingbox HTTP/1.1\n");
    client.print("Host: api.pushingbox.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();
  Serial.println("- stopping the client");
}

void loop() {
  deteksiLuar();
  deteksiDalam();
}
