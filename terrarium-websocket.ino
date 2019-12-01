#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1015.5)

//Sensor
Adafruit_BME280 bme;

#define REQUEST_STRING "\
temp=currentTemp&moisture=currentMoisture\
\
"

//const char serverAddress[] = "reading-terrarium.glitch.me";  // server address
const char serverAddress[] = "reading-terrarium.herokuapp.com";  // server address
int port = 443;
int count = 0;
WiFiSSLClient wifi;
HttpClient https = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
int statusCode = 0;
String response;
//byte mac[6];

void setup() {
  Serial.begin(9600);
  // try to connect to the network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);       // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(SECRET_SSID, SECRET_PASS);
  }

  // print the SSID of the network you're attached to:
  Serial.print("Connected to: ");
  Serial.println(WiFi.SSID());
//  WiFi.macAddress(mac);
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  
  //BME Sensor Init
  Serial.println(F("BME280 test"));
  bool status;
  status = bme.begin();  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
  Serial.println("BME280 Initialized");
  Serial.println(); 
}

void loop() {
  // assemble the path for the POST message:
  String postData = String(REQUEST_STRING);
  String path = "/thermostat";
  String contentType = "application/x-www-form-urlencoded";
//  String contentType = "text/plain";

  // assemble the body of the POST message:
  int soilMoisture = analogRead(A0);
  float temp = bme.readTemperature();
  temp = temp*1.8 + 32;
  
  postData.replace("currentMoisture", String(soilMoisture));
  postData.replace("currentTemp", String(temp));
  Serial.println("making POST request");
  Serial.println(soilMoisture, temp);

  // send the POST request
  https.post(path, contentType, postData);
  Serial.println(postData);

  // read the status code and body of the response
  statusCode = https.responseStatusCode();
  response = https.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println(count++);

  //Serial.println("Wait ten seconds\n");
  delay(6500);
}
