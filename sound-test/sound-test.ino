#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include "arduino_secrets.h"
// the VS1053 chip and SD card are both SPI devices.
// Set their respective pins:

#define VS1053_RESET    -1     // VS1053 reset pin
#define VS1053_CS       7     // VS1053 chip select pin 
#define VS1053_DCS      4     // VS1053 Data/command select pin 
#define CARDCS          3     // SD card chip select pin
#define VS1053_DREQ     5     // VS1053 Data request

int buttonPin = 6;      // pin for the pushbutton
int lastButtonState = HIGH;   // previous state of the pushbutton

// make an instance of the MP3 player library:
Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

char* soundFile[] = {"sound001.MP3", "story002.MP3"};
//const char soundFile[] = "sound001.MP3";

//server
const char serverAddress[] = "reading-terrarium.herokuapp.com";  // server address
int port = 443;
WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
int statusCode = 0;
String response;

long lastRequest = 0;
long interval = 5000;

boolean haveStatusCode = false;

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
//  WiFi.macAddress(mac);
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  pinMode(buttonPin, INPUT_PULLUP);
  
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

  // Set volume for left and right channels.
  // 0 = loudest, 100 = silent:
//  musicPlayer.setVolume(10, 10);
//  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
//  // file names must be 8 characters long, maximum, plus .mp3:
//  musicPlayer.startPlayingFile("sound001.MP3");
}

void loop() {
  if (millis() - lastRequest > interval) {
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
//
//  // loop the player:
//  if (musicPlayer.stopped()) {
//    musicPlayer.startPlayingFile("SOUND001.MP3");
//  }
//
//  // read a pushbutton for play/pause:
//  int buttonState = digitalRead(buttonPin);
//    
//  if (buttonState != lastButtonState) {
//    if (buttonState == LOW) {
//      // switch play/pause state:
//      if (musicPlayer.paused()) {
//        musicPlayer.pausePlaying(false);
//      } else {
//        musicPlayer.pausePlaying(true);
//      }
//    }
//  }
//  // save current button state for comparison next time:
//  lastButtonState = buttonState;
//  Serial.println(buttonState);
}

void getSound(){
    // get server request
//    if (millis() - lastRequest > interval) {
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
//  }
}
