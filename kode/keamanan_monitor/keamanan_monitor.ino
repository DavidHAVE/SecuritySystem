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

//Web Server pada port 80
WiFiServer server(80);

void setup() {
  Serial.begin(74880);

  pinMode(buzzerDanLampuPin, OUTPUT);
  // Sending a notification to your mobile phone
  // function takes the message as a parameter

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(5000);

  // menuliskan ip address
  Serial.println(WiFi.localIP());
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
//  sendNotification(pesan);
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
//      sendNotification(pesan);
      isBuzzer == true;
      previousMillis = currentMillis;
    }
  } else if ((isBuzzer == true) && (currentMillis - previousMillis >= 10000)) {
    //    Serial.println("Mematikan Sirine");
    digitalWrite(buzzerDanLampuPin, LOW);
    isAlarm = false;
    isBuzzer == false;
    previousMillis = currentMillis;
  }
}

void sendNotification(String message) {

  //  Serial.println("- connecting to Home Router SID: " + String(ssid));
  //
  //  WiFi.begin(ssid, password);
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(500);
  //    Serial.print(".");
  //  }
  //
  //  Serial.println();
  //  Serial.println("- succesfully connected");
  //  // menuliskan ip address
  //  Serial.println(WiFi.localIP());
  //  Serial.println("- starting client");

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

  WiFiClient client = server.available();

  if (client) {
    boolean isBlankLine = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && isBlankLine) {
          //          //Header
          //          client.println("HTTP/1.1 200 OK");
          //          client.println("Content-Type: text/html");
          //          client.println("Connection: close");
          //          client.println();
          //          //Konten Web
          //          client.println("<!DOCTYPE HTML>");
          //          client.println("<html>");
          //          client.println("<head>Monitoring</head><body><h1>Monitoring Penyusup</h1><h3>Monitor: ");
          //          client.println();
          //          client.println("</body></html>");
          //          break;
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          // Halaman web Anda yang sebenarnya menampilkan suhu dan kelembaban
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head></head><body><h1>NodeMCU ESP8266 - Temperature and Humidity Sensor DHT11</h1><h3>Temperature in Celsius: ");
          //          client.println(t);//celsiusTemp
          client.println("*C</h3><h3>Temperature ke dalam Fahrenheit: ");
          //          client.println(f);//fahrenheitTemp
          client.println("*F</h3><h3>Humidity: ");
          //          client.println(h);
          client.println("%</h3><h3>");
          client.println("</body></html>");
          break;
        }
        if (c == '\n') {
          //Saat membaca baris baru
          isBlankLine = true;
        } else if (c != '\r') {
          //Ketika menemukan karakter pada baris saat ini
          isBlankLine = false;
        }
      }
    }
    //Menutup koneksi klien
    delay(1);
  }
}
