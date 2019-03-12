#include <ESP8266WiFi.h>

// PushingBox scenario DeviceId code and API
String deviceId = "vEBE5398D26E6C78";
const char* logServer = "api.pushingbox.com";

//const char* ssid = "HOME";
//const char* password = "1234h0me";
const char* ssid = "Apotik Condong Catur";
const char* password = "ssssssss";



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
//int ledPin = LED_BUILTIN; // GPIO13

String pesan;
boolean isAlarm = false;
boolean isBuzzer = false;
long previousMillis;

boolean isNotification = false;
//boolean isSend = false;
//long previousMillis2;

boolean flag1, flag2, flag3 = false;
boolean sensor1 = true;
boolean sensor2 = true;
boolean sensor3 = true;
boolean sensor4 = true;
boolean sensor5 = true;

long totalNotification;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzerDanLampuPin, OUTPUT);
  //  pinMode(ledPin, OUTPUT);
  //  digitalWrite(ledPin, LOW);
  /////////////////////
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(5000);

  // menuliskan ip address
  Serial.println(WiFi.localIP());
}

void deteksiLuar() {
  irJendelaLuarValue = digitalRead(irJendelaLuarPin);

  if (sensor1) {
    if (irJendelaLuarValue == LOW) {
      Serial.println("Deteksi Luar Jendela");
      //    isAlarm = true;
      pesan = "Deteksi Luar Jendela";
    } else {
      //    Serial.println("Clear");
    }
  }

  pirPintuLuarValue = digitalRead(pirPintuLuarPin);

  if (sensor2) {
    if (pirPintuLuarValue == HIGH) {
      Serial.println("Deteksi Luar Pintu");
      //    isAlarm = true;
      pesan = "Deteksi Luar Pintu";
    } else {
      //    Serial.println("Clear");
    }
  }
}

void deteksiDalam() {
  irPintuDalamValue = digitalRead(irPintuDalamPin);

  if (sensor3) {
    if (irPintuDalamValue == LOW ) {
      if (flag1 == false) {
        flag1 = true;
        Serial.println("Deteksi Dalam Pintu");
        pesan = "Deteksi Dalam Pintu";
        isNotification = true;
        isAlarm = true;

        //    lampuDanBuzzer(true);
      }
    } else {
      flag1 = false;
      //    Serial.println("Clear");
    }
  }

  pirPintuDalamValue = digitalRead(pirPintuDalamPin);
  pirJendelaDalamDalamValue = digitalRead(pirJendelaDalamPin);

  if (sensor4) {
    if (pirPintuDalamValue == HIGH) {
      if (flag2 == false) {
        flag2 = true;
        Serial.println("Deteksi Dalam Pintu");
        pesan = "Deteksi Dalam Pintu Pir";
        isNotification = true;
        isAlarm = true;
      }
    } else {
      flag2 = false;
      //    Serial.println("Clear");
    }
  }

  if (sensor5) {
    if (pirJendelaDalamDalamValue == HIGH) {
      if (flag3 == false) {
        flag3 = true;
        Serial.println("Deteksi Dalam Jendela");
        pesan = "Deteksi Dalam Jendela";
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
//  isSend = false;
  totalNotification = totalNotification + 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  long currentMillis = millis();
  //    deteksiLuar();
  //    deteksiDalam();

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
  monitoring();
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
  int value1 = LOW;
  int value2 = LOW;
  int value3 = LOW;
  int value4 = LOW;
  int value5 = LOW;
  
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(buzzerDanLampuPin, HIGH);
    value = HIGH;

  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(buzzerDanLampuPin, LOW);
    value = LOW;
  }

  if (request.indexOf("/SENSOR1=ON") != -1)  {
    //    digitalWrite(LED_BUILTIN, HIGH);
    sensor1 = true;
    value1 = HIGH;

  }
  if (request.indexOf("/SENSOR1=OFF") != -1)  {
    //    digitalWrite(LED_BUILTIN, LOW);
    sensor1 = false;
    value1 = LOW;

  }


  if (request.indexOf("/SENSOR2=ON") != -1)  {
    //    digitalWrite(LED_BUILTIN, HIGH);
    sensor2 = true;
    value2 = HIGH;

  }
  if (request.indexOf("/SENSOR2=OFF") != -1)  {
    //    digitalWrite(LED_BUILTIN, LOW);
    sensor2 = false;
    value2 = LOW;

  }


  if (request.indexOf("/SENSOR3=ON") != -1)  {
    //      digitalWrite(LED_BUILTIN, HIGH);
    sensor3 = true;
    value3 = HIGH;

  }
  if (request.indexOf("/SENSOR3=OFF") != -1)  {
    //      digitalWrite(LED_BUILTIN, LOW);
    sensor3 = false;
    value3 = LOW;

  }

  if (request.indexOf("/SENSOR4=ON") != -1)  {
    //    digitalWrite(LED_BUILTIN, HIGH);
    sensor4 = true;
    value4 = HIGH;

  }
  if (request.indexOf("/SENSOR4=OFF") != -1)  {
    //    digitalWrite(LED_BUILTIN, LOW);
    sensor4 = false;
    value4 = LOW;

  }

  if (request.indexOf("/SENSOR5=ON") != -1)  {
    //    digitalWrite(LED_BUILTIN, HIGH);
    sensor5 = true;
    value5 = HIGH;

  }
  if (request.indexOf("/SENSOR5=OFF") != -1)  {
    digitalWrite(LED_BUILTIN, LOW);
    sensor5 = false;
    value5 = LOW;

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
  //  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  //  client.println("<head>");
  //  client.println("<style>");
  //client.println(".button {");
  //  client.println("<style>");
  //
  //“background-color: #990033;”
  //“border: none;”
  //“color: white;”
  //“padding: 7px 15px;”
  //“text-align: center;”
  //“cursor: pointer;”
  //
  //“}”
  //
  //“</style>”
  //
  //“</head>”
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
  client.println("<center>"); // we can see if there is Motion or not.
  client.println("<h1>");
  client.println("Monitoring!");
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
  //    client.print("<h3>Total Notification : </h3>");
  //  client.print(totalNotification);//fahrenheitTemp
  client.println("<br>");

  client.println("<table border=2 width=500 bordercolor=#ffffff >");
  client.println("<tr>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Sensor 1 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.print("Sensor 2 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.print("Sensor 3 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.print("Sensor 4 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.print("Sensor 5 :");
  client.println("</th>");
  client.println("</tr>");

  client.println("<tr>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println(irJendelaLuarValue);
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.print(pirPintuLuarValue);
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.print(irPintuDalamValue);
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5");
  client.print(pirPintuDalamValue);
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5");
  client.print(pirJendelaDalamDalamValue);
  client.println("</td>");
  client.println("</tr>");
  client.println("</table>");


  //  client.print("<h3>Sensor 1 :</h3>");
  //  client.println(irJendelaLuarValue);//fahrenheitTemp
  //  client.print("<h3>Sensor 2 :</h3>");
  //  client.println(pirPintuLuarValue);//fahrenheitTemp
  //  client.print("<h3>Sensor 3 :</h3>");
  //  client.println(irPintuDalamValue);//fahrenheitTemp
  //  client.print("<h3>Sensor 4 :</h3>");
  //  client.println(pirPintuDalamValue);//fahrenheitTemp
  //  client.print("<h3>Sensor 5 :</h3>");
  //  client.println(pirJendelaDalamDalamValue);


  client.print("<p>");
  client.println("<font color = #80CBC4 size=5 text-align=center>");
  client.print("Relay is now : ");

  if (value == HIGH) {
    client.print("On");
    //      sendNotification("on");
  } else {
    client.print("Off");
    //      sendNotification("off");
  }
  client.println("</font>");

  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button class=\"button\">Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button class=\"button2\">Turn Off </button></a><br />");
  client.print("</p>");

  client.println("<br><br>");

  //  client.print("<h3>Total Notification : </h3>");
  //  client.print(totalNotification);//fahrenheitTemp

  //  client.print("<h3>Sensor 1 :</h3>");
  //  client.println(irJendelaLuarValue);//fahrenheitTemp
  //  client.print("<h3>Sensor 2 :</h3>");
  //  client.println(pirPintuLuarValue);//fahrenheitTemp
  //  client.print("<h3>Sensor 3 :</h3>");
  //  client.println(irPintuDalamValue);//fahrenheitTemp
  //  client.print("<h3>Sensor 4 :</h3>");
  //  client.println(pirPintuDalamValue);//fahrenheitTemp
  //  client.print("<h3>Sensor 5 :</h3>");
  //  client.println(pirJendelaDalamDalamValue);//fahrenheitTemp
  //            client.println("*F < / h3 > <h3>Humidity: ");
  //                      client.println(h);
  //            client.println(" % < / h3 > u");
  //            client.print("Led pin is now: ");


  client.println("<table border=1 width=500 bordercolor=#81D4FA >");
  client.println("<tr>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.println("Sensor 1 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.print("Sensor 2 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.print("Sensor 3 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.print("Sensor 4 :");
  client.println("</th>");
  client.println("<th>");
  client.println("<font color = turquoise size=3>");
  client.print("Sensor 5 :");
  client.println("</th>");
  client.println("</tr>");

  client.println("<tr>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  if (value1 == HIGH) {
    client.print("On");
    //      sendNotification("on");
  } else {
    client.print("Off");
    //      sendNotification("off");
  }
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  if (value2 == HIGH) {
    client.print("On");
    //      sendNotification("on");
  } else {
    client.print("Off");
    //      sendNotification("off");
  }
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  if (value3 == HIGH) {
    client.print("On");
    //      sendNotification("on");
  } else {
    client.print("Off");
    //      sendNotification("off");
  }
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5");
  if (value4 == HIGH) {
    client.print("On");
    //      sendNotification("on");
  } else {
    client.print("Off");
    //      sendNotification("off");
  }
  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5");
  if (value5 == HIGH) {
    client.print("On");
    //      sendNotification("on");
  } else {
    client.print("Off");
    //      sendNotification("off");
  }
  client.println("</td>");
  client.println("</tr>");

  client.println("<tr>");
  client.println("<td align=center>");
  client.println("<font color = white size=5>");
  client.println(" <a href = \"/SENSOR1=ON\"\"><button class=\"button\">Turn On </button></a>");
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
  client.println("<td align=center>");
  client.println("<font color = white size=5");
  client.println("<a href=\"/SENSOR4=ON\"\"><button class=\"button\">Turn On </button></a>");
  client.println("<a href=\"/SENSOR4=OFF\"\"><button class=\"button2\">Turn Off </button></a><br />");

  client.println("</td>");
  client.println("<td align=center>");
  client.println("<font color = white size=5");
  client.println("<a href=\"/SENSOR5=ON\"\"><button class=\"button\">Turn On </button></a>");
  client.println("<a href=\"/SENSOR5=OFF\"\"><button class=\"button2\">Turn Off </button></a><br />");

  client.println("</td>");
  client.println("</tr>");
  client.println("</table>");




  //  client.println("<br><br>");
  //  client.println(" <a href = \"/SENSOR1=ON\"\"><button>Turn On </button></a>");
  //  client.println("<a href=\"/SENSOR1=OFF\"\"><button>Turn Off </button></a><br />");
  //
  //  if (value1 == HIGH) {
  //    client.print("On");
  //    //      sendNotification("on");
  //  } else {
  //    client.print("Off");
  //    //      sendNotification("off");
  //  }
  //
  //
  //  client.println("<br><br>");
  //  client.println("<a href=\"/SENSOR2=ON\"\"><button>Turn On </button></a>");
  //  client.println("<a href=\"/SENSOR2=OFF\"\"><button>Turn Off </button></a><br />");
  //
  //  if (value2 == HIGH) {
  //    client.print("On");
  //    //      sendNotification("on");
  //  } else {
  //    client.print("Off");
  //    //      sendNotification("off");
  //  }
  //
  //  client.println("<br><br>");
  //  client.println("<a href=\"/SENSOR3=ON\"\"><button>Turn On </button></a>");
  //  client.println("<a href=\"/SENSOR3=OFF\"\"><button>Turn Off </button></a><br />");
  //
  //  if (value3 == HIGH) {
  //    client.print("On");
  //    //      sendNotification("on");
  //  } else {
  //    client.print("Off");
  //    //      sendNotification("off");
  //  }
  //
  //
  //  client.println("<br><br>");
  //  client.println("<a href=\"/SENSOR4=ON\"\"><button>Turn On </button></a>");
  //  client.println("<a href=\"/SENSOR4=OFF\"\"><button>Turn Off </button></a><br />");
  //
  //  if (value4 == HIGH) {
  //    client.print("On");
  //    //      sendNotification("on");
  //  } else {
  //    client.print("Off");
  //    //      sendNotification("off");
  //  }
  //
  //
  //  client.println("<br><br>");
  //  client.println("<a href=\"/SENSOR5=ON\"\"><button>Turn On </button></a>");
  //  client.println("<a href=\"/SENSOR5=OFF\"\"><button>Turn Off </button></a><br />");
  //
  //  if (value5 == HIGH) {
  //    client.print("On");
  //    //      sendNotification("on");
  //  } else {
  //    client.print("Off");
  //    //      sendNotification("off");
  //  }


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
  //      delay(1);
  //  }
}


