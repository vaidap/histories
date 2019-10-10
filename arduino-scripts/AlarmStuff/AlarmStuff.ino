/*
  ThingSpeak sensor client

  This sketch connects an analog sensor to ThingSpeak,
  using an Arduino Yún.

  created 15 March 2010
  updated 27 May 2013
  by Tom Igoe
  modified to work with ThingSpeak Sept 2014
*/

// include all Libraries needed:
#include <Process.h>
#include <Wire.h>
#include <Digital_Light_TSL2561.h>

// Write API Key
String writeAPIKey = "VDV0DDKP3IRT62SI";      // ENTER YOUR WRITE API KEY HERE!!!

// set up net client info:
const unsigned long postingInterval = 15000;  // delay between updates. Minimum 15000
unsigned long lastRequest = 0;                // when you last made a request
String dataString = "";

int loudness;
int light;
#define IRsensor A2 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)

//
void setup() 
{
  Wire.begin();
  
  // start serial port:
  Bridge.begin();
  Serial.begin(9600);
  TSL2561.init();
  
  while (!Serial);   // Block waiting for Network Serial to open
  Serial.println("ThingSpeak client");

  // Do a first update immediately
  updateData();
  sendData();
  lastRequest = millis();
}

// Main loop
void loop() 
{
  // Get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // LOUDNESS
  loudness = analogRead(1);
  Serial.println("Loudness: " + String(loudness));


  // IR SENSOR
  // 5v
  float volts = analogRead(IRsensor)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13*pow(volts, -1); // worked out from datasheet graph
  
  if (distance <= 30){
    Serial.println("Distance: " + String(distance));   // print the distance
  }

  // LIGHT
  Serial.print("The Light value is: ");
  //Serial.println(TSL2561.readVisibleLux());
  
  delay(1000);

  // If the sending interval has passed since the
  // last connection, then connect again and send data:
  if (now - lastRequest >= postingInterval) 
  {
    // not sending anything right now 
    //updateData();
    // sendData();
    lastRequest = now;
  }
}

//
void updateData() 
{
  // ADD CODE HERE TO READ YOUR SENSOR(S) 
  
  // Convert the readings to a String to send it:
  dataString = "";
  dataString = "api_key=" + writeAPIKey + "&";
  dataString += "field1=";
  dataString += random(10) + 20;
//  dataString += "&field2=";
//  dataString += ;
//  dataString += "&field3=";
//  dataString += ;
// ETC ...
  
  Serial.println("dataString " + dataString);
}

// This function makes a HTTP connection to the server:
void sendData() 
{
  // Form the string for the URL parameter:
  String url = "https://api.thingspeak.com/update";
  Serial.println("url " + url);
  
  // Send the HTTP GET request.
  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process thingSpeak;
  Serial.print("\n\nSending data... ");
  thingSpeak.begin("curl");
  thingSpeak.addParameter("-k");
  thingSpeak.addParameter("--request");
  thingSpeak.addParameter("GET");
  thingSpeak.addParameter("--data");
  thingSpeak.addParameter(dataString);
  thingSpeak.addParameter(url);
  thingSpeak.run();
  Serial.println("done!");

  // If there's incoming data from the net connection, send it out the Serial:
  while (thingSpeak.available() > 0) 
  {
    char c = thingSpeak.read();
    Serial.write(c);
  }
}
