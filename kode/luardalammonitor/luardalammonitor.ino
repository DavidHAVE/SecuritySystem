#include <ESP8266WiFi.h>

// PushingBox scenario DeviceId code and API
String deviceId = "vEBE5398D26E6C78";
const char* logServer = "api.pushingbox.com";

const char* ssid = "HOME";
const char* password = "1234h0me";

//Web Server pada port 80
WiFiServer server(80);

////////////////////////////

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

boolean isNotification = false;
boolean isSend = false;
long previousMillis2;

boolean flag1, flag2, flag3 = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzerDanLampuPin, OUTPUT);

  /////////////////////
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

  if (irPintuDalamValue == LOW ) {
    if (flag1 == false) {
      flag1 = true;
      Serial.println("Deteksi Dalam Pintu");
      pesan = "Deteksi Dalam Pintu";
      //    sendNotification(pesan);
      isNotification = true;
      isAlarm = true;

      //    lampuDanBuzzer(true);
    }
  } else {
    flag1 = false;
    //    Serial.println("Clear");
    //        isAlarm = false;
  }

  pirPintuDalamValue = digitalRead(pirPintuDalamPin);
  pirJendelaDalamDalamValue = digitalRead(pirJendelaDalamPin);

  if (pirPintuDalamValue == HIGH) {
    if (flag2 == false) {
      flag2 = true;
      Serial.println("Deteksi Dalam Pintu");
      pesan = "Deteksi Dalam Pintu Pir";
      //    sendNotification(pesan);
      isNotification = true;
      isAlarm = true;
    }
  } else {
    flag2 = false;
    //    Serial.println("Clear");
    //    isAlarm = false;
  }

  if (pirJendelaDalamDalamValue == HIGH) {
    if (flag3 == false) {
      flag3 = true;
      Serial.println("Deteksi Dalam Jendela");
      pesan = "Deteksi Dalam Jendela";
      //    sendNotification(pesan);
      isNotification = true;
      isAlarm = true;
    }
  } else {
    flag3 = false;
    //    Serial.println("Clear");
    //    isAlarm = false;
  }
}

//void sendNotification(String message) {
//
//  //  Serial.println("- connecting to Home Router SID: " + String(ssid));
//  //
//  //  WiFi.begin(ssid, password);
//  //  while (WiFi.status() != WL_CONNECTED) {
//  //    delay(500);
//  //    Serial.print(".");
//  //  }
//  //
//  //  Serial.println();
//  //  Serial.println("- succesfully connected");
//  //  // menuliskan ip address
//  //  Serial.println(WiFi.localIP());
//  //  Serial.println("- starting client");
//
//  WiFiClient client;
//
//  Serial.println("- connecting to pushing server: " + String(logServer));
//  if (client.connect(logServer, 80)) {
//    Serial.println("- succesfully connected");
//
//    String postStr = "devid=";
//    postStr += String(deviceId);
//    postStr += "&message_parameter=";
//    postStr += String(message);
//    postStr += "\r\n\r\n";
//
//    Serial.println("- sending data...");
//
//    client.print("POST /pushingbox HTTP /1.1\n");
//    client.print("Host: api.pushingbox.com\n");
//    client.print("Connection: close\n");
//    client.print("Content-Type: application/x-www-form-urlencoded\n");
//    client.print("Content-Length: ");
//    client.print(postStr.length());
//    client.print("\n\n");
//    client.print(postStr);
//  }
//  client.stop();
//  Serial.println("- stopping the client");
//}

void sendNotification(String message) {

  Serial.println("- connecting to Home Router SID: " + String(ssid));

      while (WiFi.status() != WL_CONNECTED) {
              WiFi.begin(ssid, password);

//        delay(500);
//        Serial.print(".");
      }
  
    Serial.println();
    Serial.println("- succesfully connected");
    Serial.println("- starting client");
  //
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
  isNotification = false;
  isSend = false;
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

  ////////////////////////

  if (isNotification == true) {
//    if ((isSend == false) && (currentMillis - previousMillis2 >= 1000)) {
      Serial.println("Mengirim Notifikasi");
//      isSend = true;
      sendNotification(pesan);

      //      previousMillis2 = currentMillis;
      //
      //    } else if ((isSend == true) && (currentMillis - previousMillis2 >= 1500)) {
      //      Serial.println("Mematikan Notifikasi");
      //      isNotification = false;
      //      isSend = false;
      //      previousMillis2 = currentMillis;
//    }
  }
  //    //  monitoring()
  //    }
}

void monitoring() {
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(LED_BUILTIN, HIGH);
    value = HIGH;

  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(LED_BUILTIN, LOW);
    value = LOW;

  }
  //  if (request.indexOf("/BUZZER=ON") != -1)  {
  //    digitalWrite(LED_BUILTIN, HIGH);
  //    value = HIGH;
  //    sendNotification("on");
  //  }
  //  if (request.indexOf("/BUZZER=OFF") != -1)  {
  //    digitalWrite(LED_BUILTIN, LOW);
  //    value = LOW;
  //    sendNotification("off");
  //  }

  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Led pin is now: ");

  if (value == HIGH) {
    client.print("On");
    //      sendNotification("on");
  } else {
    client.print("Off");
    //      sendNotification("off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  client.println("</html>");

  //  delay(1);
  //  Serial.println("Client disonnected");
  //  Serial.println("");

  //  if (client) {
  //    boolean isBlankLine = true;
  //    while (client.connected()) {
  //      if (client.available()) {
  //        char c = client.read();
  //
  //        String request = client.readStringUntil('\r');
  //        client.flush();
  //
  //        int buzzerValue = LOW;
  //        if(request.indexOf("/BUZZER=ON") != -1){
  //          digitalWrite(LED_BUILTIN, HIGH);
  //          buzzerValue = HIGH;
  //        }
  //        if(request.indexOf("/BUZZER=OFF") != -1){
  //          digitalWrite(LED_BUILTIN, LOW);
  //          buzzerValue = LOW;
  //        }

  //        if (c == '\n' && isBlankLine) {
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


  //          client.println("HTTP/1.1 200 OK");
  //          client.println("Content-Type: text/html");
  //          client.println("Connection: close");
  //          client.println();
  //          // Halaman web Anda yang sebenarnya menampilkan suhu dan kelembaban
  //          client.println("<!DOCTYPE HTML>");
  //          client.println("<html>");
  //          client.println("<head></head><body><h1>NodeMCU ESP8266 - Temperature and Humidity Sensor DHT11</h1><h3>Temperature in Celsius: ");
  //          //          client.println(t);//celsiusTemp
  //          client.println("*C</h3><h3>Temperature ke dalam Fahrenheit: ");
  //          //          client.println(f);//fahrenheitTemp
  //          client.println("*F</h3><h3>Humidity: ");
  //          //          client.println(h);
  //          client.println("%</h3>u");
  //          client.print("Led pin is now: ");
  //
  //          if (value == HIGH) {
  //            client.print("On");
  //          } else {
  //            client.print("Off");
  //          }
  //          client.println("<br><br>");
  //          client.println("<a href=\"/BUZZER=ON\"\"><button>Turn On </button></a>");
  //          client.println("<a href=\"/BUZZER=OFF\"\"><button>Turn Off </button></a><br />");
  //          client.println("</body></html>");




  //          break;
  //        }
  //        if (c == '\n') {
  //          //Saat membaca baris baru
  //          isBlankLine = true;
  //        } else if (c != '\r') {
  //          //Ketika menemukan karakter pada baris saat ini
  //          isBlankLine = false;
  //        }
  //      }
  //    }
  //Menutup koneksi klien
  //    delay(1);
  //  }
}


