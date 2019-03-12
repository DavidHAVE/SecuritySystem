#include <ESP8266WiFi.h>

// PushingBox scenario DeviceId code and API
String deviceId = "vEBE5398D26E6C78";
const char* logServer = "api.pushingbox.com";

const char* ssid = "HOME";
const char* password = "1234h0me";

//const char* ssid = "Apotik Condong Catur";
//const char* password = "ssssssss";


//Web Server pada port 80
WiFiServer server(80);

////////////////////////////

//sensor luar
const int irJendelaLuarPin = 4; //pin D2
long irJendelaLuarValue;

const int pirPintuLuarPin = 16; //pin D0
long pirPintuLuarValue;

//sensor dalam
const int pirJendelaDalamPin = 13; //pin D7
long pirJendelaDalamDalamValue;

//output
const int buzzerDanLampuPin = 15; //pin D8
//int ledPin = LED_BUILTIN; // GPIO13

String pesan;
boolean isAlarm = false;
boolean isBuzzer = false;
long previousMillis;

boolean isNotification = false;
//boolean isSend = false;
//long previousMillis2;

int value = LOW;
int value1 = HIGH;
int value2 = HIGH;
int value3 = HIGH;


boolean flag1, flag2, flag3, flag4, flag5 = false;
boolean sensor1 = true;
boolean sensor2 = true;
boolean sensor3 = true;

long totalNotification;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzerDanLampuPin, OUTPUT);
  //  pinMode(ledPin, OUTPUT);
  //  digitalWrite(ledPin, LOW);
  /////////////////////

  WiFi.begin(ssid, password);
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

  if (sensor1) {
    if (irJendelaLuarValue == LOW) {
      if (flag1 == false) {
        flag1 = true;
        Serial.println("Deteksi Luar Jendela");
        pesan = "Deteksi Luar Jendela";
        isNotification = true;
      }
    } else {
      flag1 = false;
      //    Serial.println("Clear");
    }
  }

  pirPintuLuarValue = digitalRead(pirPintuLuarPin);

  if (sensor2) {
    if (pirPintuLuarValue == HIGH) {
      if (flag2 == false) {
        flag2 = true;
        Serial.println("Deteksi Luar Pintu");
        pesan = "Deteksi Luar Pintu";
        isNotification = true;
      }
    } else {
      flag2 = false;
      //    Serial.println("Clear");
    }
  }
}

void deteksiDalam() {
  pirJendelaDalamDalamValue = digitalRead(pirJendelaDalamPin);

  if (sensor3) {
    if (pirJendelaDalamDalamValue == HIGH) {
      if (flag3 == false) {
        flag3 = true;
        Serial.println("Deteksi Dalam Ruangan");
        pesan = "Deteksi Dalam Ruangan";
        isNotification = true;
        isAlarm = true;
      }
    } else {
      flag3 = false;
      //    Serial.println("Clear");
    }
  }
}


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

    //devid=vEBE5398D26E6C78&message_parameter=""

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
  totalNotification = totalNotification + 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  long currentMillis = millis();
  ///////////////////
//  deteksiLuar();
//  deteksiDalam();
  //////////////////

//  if (isAlarm) {
//    if ((isBuzzer == false) && (currentMillis - previousMillis >= 4000)) {
//      Serial.println("1");
//      Serial.println("Membunyikan Sirine");
//      digitalWrite(buzzerDanLampuPin, HIGH); //Hidup
//      isBuzzer = true;
//      previousMillis = currentMillis;
//
//    } else if ((isBuzzer == true) && (currentMillis - previousMillis >= 8000)) {
//      Serial.println("Mematikan Sirine");
//      Serial.println("2");
//      digitalWrite(buzzerDanLampuPin, LOW); //Mati
//      isAlarm = false;
//      isBuzzer = false;
//      previousMillis = currentMillis;
//    }
//  }

  ////////////////////////
  //
//    if (isNotification == true) {
//      Serial.println("Mengirim Notifikasi");
//      sendNotification(pesan);
//    }

  monitoring();
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

  if (digitalRead(buzzerDanLampuPin) == HIGH) {
    value = HIGH;
  } else {
    value = LOW;
  }

  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(buzzerDanLampuPin, HIGH);
    value = HIGH;

  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(buzzerDanLampuPin, LOW);
    value = LOW;
  }

  if (request.indexOf("/SENSOR1=ON") != -1)  {
    sensor1 = true;
    value1 = HIGH;
  }
  if (request.indexOf("/SENSOR1=OFF") != -1)  {
    sensor1 = false;
    value1 = LOW;
  }
  if (request.indexOf("/SENSOR2=ON") != -1)  {
    sensor2 = true;
    value2 = HIGH;
  }
  if (request.indexOf("/SENSOR2=OFF") != -1)  {
    sensor2 = false;
    value2 = LOW;
  }
  if (request.indexOf("/SENSOR3=ON") != -1)  {
    sensor3 = true;
    value3 = HIGH;
  }
  if (request.indexOf("/SENSOR3=OFF") != -1)  {
    sensor3 = false;
    value3 = LOW;
  }

  if (request.indexOf("/ALLSENSOR=ON") != -1)  {
    sensor1 = true;
    value1 = HIGH;
    sensor2 = true;
    value2 = HIGH;
    sensor3 = true;
    value3 = HIGH;
  }
  if (request.indexOf("/ALLSENSOR=OFF") != -1)  {
    sensor1 = false;
    value1 = LOW;
    sensor2 = false;
    value2 = LOW;
    sensor3 = false;
    value3 = LOW;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  //    client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");

  client.println("<html>");

  client.println("<head>");
  client.println("<title>");
  client.println("Monitoring");
  client.println("</title>");
  client.println("<style>");
  client.println(".button {background-color: #D50000;border: none;color: white;padding: 7px 15px;text-align: center;cursor: pointer;}");
  client.println(".button2 {background-color: #212121;border: none;color: white;padding: 7px 15px;text-align: center;cursor: pointer;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body bgcolor=#9E9E9E>");
  client.println("<font color=#FFD54F>");
  client.println("<center>");
  client.println("<h1>");
  client.println("Monitoring !");
  client.println("</h1>");
  client.println("<br><br><br>");

  client.println("<table border=2 width=200 bordercolor=#ffffff >");
  client.println("<tr>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Total Notification : ");
  client.println("</th>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println(totalNotification);
  client.println("</td>");
  client.println("<tr>");
  client.println("</table>");
  client.println("<br>");

  client.println("<table border=2 width=500 bordercolor=#ffffff >");
  client.println("<tr>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Sensor 1 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Sensor 2 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Sensor 3 :");
  client.println("</th>");
  client.println("</tr>");

  client.println("<tr>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println(irJendelaLuarValue);
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println(pirPintuLuarValue);
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println(pirJendelaDalamDalamValue);
  client.println("</td>");
  client.println("</tr>");
  client.println("</table>");

  client.print("<p>");
  client.println("<font color = #80CBC4 size=5>");
  client.print("Siren is now : ");

  if (value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("</font>");

  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button class=\"button\">Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button class=\"button2\">Turn Off </button></a><br />");
  client.print("</p>");

  client.println("<br><br>");

  client.println("<table border=1 width=500 bordercolor=#81D4FA >");
  client.println("<tr>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Sensor 1 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Sensor 2 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Sensor 3 :");
  client.println("</th>");
  client.println("</tr>");

  client.println("<tr>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  if (value1 == HIGH) {
    client.println("On");
  } else {
    client.println("Off");
  }
  client.println("</td>");

  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  if (value2 == HIGH) {
    client.println("On");
  } else {
    client.println("Off");
  }
  client.println("</td>");

  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  if (value3 == HIGH) {
    client.println("On");
  } else {
    client.println("Off");
  }
  client.println("</td>");
  client.println("</tr>");

  client.println("<tr>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println("<a href=\"/SENSOR1=ON\"\"><button class=\"button\">Turn On </button></a>");
  client.println("<a href=\"/SENSOR1=OFF\"\"><button class=\"button2\">Turn Off </button></a><br />");
  client.println("</td>");

  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println("<a href=\"/SENSOR2=ON\"\"><button class=\"button\">Turn On </button></a>");
  client.println("<a href=\"/SENSOR2=OFF\"\"><button class=\"button2\">Turn Off </button></a><br />");
  client.println("</td>");

  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println("<a href=\"/SENSOR3=ON\"\"><button class=\"button\">Turn On </button></a>");
  client.println("<a href=\"/SENSOR3=OFF\"\"><button class=\"button2\">Turn Off </button></a><br />");
  client.println("</td>");

  client.println("</tr>");
  client.println("</table>");

  client.println("<table border=1 width=500 bordercolor=#81D4FA >");
  client.println("<tr>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("All Sensor");
  client.println("</th>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println("<a href=\"/ALLSENSOR=ON\"\"><button class=\"button\">Turn On </button></a>");
  client.println("<a href=\"/ALLSENSOR=OFF\"\"><button class=\"button2\">Turn Off </button></a><br />");
  client.println("</td>");
  client.println("<tr>");
  client.println("</table>");


  client.println("</center>");

  client.print("<br><br>");

  client.print("<p>");
  client.println("<font color = #FFD54F size=4>");
  client.print("Keterangan :");
  client.print("</p>");
  client.println("Sensor 1 : IR Jendela Luar ");
  client.print("<br>");
  client.println("Sensor 2 : PIR Pintu Luar ");
  client.print("<br>");
  client.println("Sensor 3 : PIR Jendela Dalam ");


  client.println("</body");
  client.println("</html>");


  //Menutup koneksi klien
  //      delay(1);
}


