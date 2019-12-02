/////Server
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

#define REQUEST_STRING "\
temp=currentTemp&moisture=currentMoisture\
\
"

const char serverAddress[] = "reading-terrarium.herokuapp.com";  // server address
int port = 443;
WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
HttpClient https = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
int statusCode = 0;
String response;

long lastRequest = 0;
long interval = 5000;

boolean haveStatusCode = false;
int count = 0;

/////Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1015.5)
//define sensor
Adafruit_BME280 bme;

/////MP3
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#define VS1053_RESET    -1     // VS1053 reset pin
#define VS1053_CS       7     // VS1053 chip select pin 
#define VS1053_DCS      4     // VS1053 Data/command select pin 
#define CARDCS          3     // SD card chip select pin
#define VS1053_DREQ     5     // VS1053 Data request

Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

char* soundFile[] = {"sound001.MP3", "story002.MP3"};

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);       // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(SECRET_SSID, SECRET_PASS);
  }

  // print the SSID of the network you're attached to:
  Serial.print("Connected to: ");
  Serial.println(WiFi.SSID());
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

  // initialize the MP3 player:
  if (! musicPlayer.begin()) {
    Serial.println("Couldn't find VS1053. Do you have the right pin numbers set?");
    while (true);
  }

  // start communication with the MP3 shield:
  if (!SD.begin(CARDCS)) {
    Serial.println("SD failed, or not present");
    while (true);  // don't do anything more
  }
}
  

void loop() {
  // get server request
  if (millis() - lastRequest > interval) {
    sendSensor();
    getSound();
  }
  
  if (statusCode >= 0 && haveStatusCode == true) { // basically if the server sends any valid http response code
//    Serial.println("We have a response!");
    response = client.responseBody();
//    Serial.print("Status code: ");
//    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);

    if (response == "1 song"){
    musicPlayer.setVolume(10, 10);
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
    musicPlayer.startPlayingFile("/sound001.MP3");
    Serial.println("song playing");
  } else if (response == "1 story"){
     musicPlayer.setVolume(10, 10);
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
    musicPlayer.startPlayingFile("/story002.MP3");
    Serial.println("story playing");
  }
    haveStatusCode = false;
  }
  
  // read a potentiometer to set volume:
  int loudness = map(analogRead(A1), 0, 1023, 100, 0);
  musicPlayer.setVolume(loudness, loudness);
}

void getSound(){
    Serial.println("making a request");
    // assemble the path for the GET message:
    String path = "/playPause";
    client.get(path);
    lastRequest = millis();

    // read the status code and body of the response
    statusCode = client.responseStatusCode();
    haveStatusCode = true;
    Serial.print("Status code: ");
    Serial.println(statusCode);
}

void sendSensor(){
  String postData = String(REQUEST_STRING);
  String path = "/thermostat";
  String contentType = "application/x-www-form-urlencoded";

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
//  Serial.println(count++);
}
//
//void playSound(){
//    // basically if the server sends any valid http response code
//    response = client.responseBody();
//    Serial.print("Response: ");
//    Serial.println(response);
//
//    if (response == "1 song"){
//    musicPlayer.setVolume(10, 10);
//    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
//    musicPlayer.startPlayingFile("/sound001.MP3");
//    Serial.println("song playing");
//  } else if (response == "1 story"){
//     musicPlayer.setVolume(10, 10);
//    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
//    musicPlayer.startPlayingFile("/story002.MP3");
//    Serial.println("story playing");
//  }
//    haveStatusCode = false;
//}
