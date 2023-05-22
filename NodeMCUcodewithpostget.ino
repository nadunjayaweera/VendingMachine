#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 12 // D6 pin
#define DHTTYPE DHT11
//#define microSwitchDoor 16 //D0 pin
const int doorSwitch = D3; //D0 pin

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial myNode (D1, D2); //RX, TX
String git;
int a = 1;
int count = 0;
int respond = 0;
int mailcount =1;


//Wi-Fi name.
const char* ssid     = "SLT-ADSL-86938";
//Wi-Fi password
const char* password = "caj2753nmj";
// Device apikey for identifying the device. Each device has a different API key.
String apiKeyValue = "12345678";
// Link using GET and POST data to server.
const char* host = "http://cbx5e.localto.net/api/v1/data";
// Link using Send Email.
const char* ehost= "maker.ifttt.com";



void setup() {
  pinMode(doorSwitch, INPUT);
  // Initialize "debug" serial port
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  dht.begin();
  myNode.begin(9600);
  myNode.write(6);
  // Connecting to the WiFi.
  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  // Trying to connect to WiFi.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
//  email();
}

void email(){
  
  if(mailcount ==1){
    mailcount ++;
  WiFiClient client;
  if (!client.connect(ehost, 80)) {
        Serial.println("Connection failed");
        return;
      }
      String url = "/trigger/securitybreach/with/key/c2dLtgqA9rmZxR-gweIb7T";
      // Create a JSON object
      StaticJsonDocument<200> jsonDocument;
      jsonDocument["value1"] = "Vending ID 01";
      jsonDocument["value2"] = "";
      jsonDocument["value3"] = "";

      // Serialize the JSON object to a string
      String body;
      serializeJson(jsonDocument, body);
      
      Serial.println("Sending request...");
      
      client.print("POST " + url + " HTTP/1.1\r\n");
      client.print("Host: " + String(ehost) + "\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print("Content-Length: " + String(body.length()) + "\r\n");
      client.print("Connection: close\r\n");
      client.print("\r\n");
      client.print(body);
      delay(1000);
      
      while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      
      client.stop();
  }
}
void dhtdata(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C ");
  
  WiFiClient client;
  HTTPClient http;
  http.begin(client, host);
  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  DynamicJsonDocument doc(200);
  
  doc["humidity"] = String(h);
  doc["temperature"] = String(t);
//  doc["value"] = "6";

  String httpRequestData ="";

    serializeJson(doc, httpRequestData);
    Serial.print("httpRequestData: ");
    // post data to server.
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);

    // Getting http response code.
     if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    String payloadGetpart = http.getString();
    Serial.print("Incomming data : ");
    Serial.println(payloadGetpart);
    http.end();
}
void loop() {
  
  //////////////Get_Request////////////////////////////////////////
  while (!myNode.available()) {
    delay(100);
  }
  WiFiClient client;
  HTTPClient http;
  http.begin(client, host);

  String payloadincomming = http.getString();
  int httpResponseCodeget = http.GET();
  
  Serial.println(httpResponseCodeget);
  
  if (httpResponseCodeget > 0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCodeget);
    payloadincomming = http.getString();
    Serial.print("Incoming data: ");
//    git = payloadincomming;
//    Serial.println(git);
    DynamicJsonDocument jsonDocument(200);
    DeserializationError error = deserializeJson(jsonDocument, payloadincomming);
    if (error) {
      Serial.print("Deserialization error: ");
      Serial.println(error.c_str());
    } 
    else{
      git = jsonDocument["value"].as<String>();
      Serial.println(git);
    }

    // Remove any extra characters from the string and convert to integer.
    git.trim();
    a = git.substring(1, git.length() - 1).toInt();
    
    if (a == 0 && git[1] != '0') {
      Serial.println("Conversion error");
    } else {
      Serial.print("Number ");
      Serial.println(a);
      myNode.write(a);
      delay(30);
      while(respond != 5){
        respond = myNode.read();
        delay(30);
      }
      respond =0;
    }
  }
  
  http.end();
    Serial.println("---------------Send Data---------------");
  Serial.println("");
  delay(4000);
  dhtdata();
    Serial.println("---------------End OF Message---------------");
  Serial.println("");
  int doorSwitchState = digitalRead(doorSwitch);
  if(doorSwitchState == HIGH){
    Serial.println("------------------------SECURITY BTEACH--------------------");
    email();
  }
}
