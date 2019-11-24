#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

#define REQUEST_STRING "\
temp=currentTemp&moisture=currentMoisture\
\
"

const char serverAddress[] = "reading-terrarium.glitch.me";  // server address
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


}

void loop() {
  // assemble the path for the POST message:
  String postData = String(REQUEST_STRING);
  String path = "/thermostat";
  String contentType = "application/x-www-form-urlencoded";

  // assemble the body of the POST message:
  int sensorReading = analogRead(A0);
  Serial.println(sensorReading);
//  float voltage = sensorReading *  (3.3 / 1024.0);
//  // convert the millivolts to temperature celsius:
//  float temperature = (voltage - 0.5) / 0.01;
//  sensorReading = analogRead(A1);
//  float setpoint = map(sensorReading, 0, 1023, 15, 25);
//  postData.replace("currentMac", setMacAddress(mac));
//  postData.replace("currentTime", setTimestamp());
    postData.replace("currentMoisture", String(sensorReading));
    postData.replace("currentTemp", String(sensorReading));
  Serial.println("making POST request");

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
  delay(65002);
}
