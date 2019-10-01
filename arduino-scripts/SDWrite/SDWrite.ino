// link between the computer and the SoftSerial Shield
// at 9600 bps 8-N-1
// Computer is connected to Hardware UART
// SoftSerial Shield is connected to the Software UART:D2&D3

#include <SoftwareSerial.h>
#include <SD.h>
#include <ArduinoJson.h>

#define CHIP_SELECT 10                         // For an Uno it's 10

#define PIR_MOTION_SENSOR 2//Use pin 2 to receive the signal from the module
int loudness;
String motion;
unsigned long time;

SoftwareSerial SoftSerial(8, 7);
File logfile;

//
// Setup
//
void setup()
{
  pinMode(PIR_MOTION_SENSOR, INPUT);

  Serial.begin(9600);                      // the Serial port of Arduino baud rate.

  Serial.println("Setting up file on SD card");
  setupSDFile();
  
  SoftSerial.begin(9600);                  // the SoftSerial baud rate
}

//
// setupSDFile - Create and open for writing the file on the SD card
//
void setupSDFile()
{
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT)) 
  {
    Serial.println("Card init. failed!");
  }

  char filename[15];
  strcpy(filename, "LOGFIL00.TXT");
  for (uint8_t i = 0; i < 100; i++) 
  {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    
    // create if does not exist, do not open existing, write, sync after write
    if (!SD.exists(filename)) 
    {
        break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if ( !logfile ) 
  {
      Serial.print("FAILED TO CREATE "); 
  }
  else
  {
      Serial.print("Writing to "); 
  }

  Serial.println(filename);
}

//
// Loop
//
void loop()
{
    // Write the data to the SD card
    outputData();
        
    delay(500);
}

//
// outputData
//
void outputData()
{
  int sensor = analogRead(A0);
  time = millis();
    if(digitalRead(PIR_MOTION_SENSOR)) {
        //Serial.println(String(time) + " MOTION: Hi,people is coming");
        motion = String(time) + " Hi,people is coming";
    }
    else {
        //Serial.println(String(time) + " Watching");
        motion = String(time) + " Watching";
    }
    
    // loudness sensor
    loudness = analogRead(0);
    //Serial.println(String(time) + " LOUDNESS: " + loudness);

  
  String dataString = "";

  dataString += motion;
  dataString += String(time) + " LOUDNESS: " + String(loudness);

    const int capacity = JSON_OBJECT_SIZE(5);
    StaticJsonDocument<capacity> doc;
    doc["motion"] = motion;
    doc["loud"] = String(time) + " " + String(loudness);
    //serializeJsonPretty(doc, Serial);
    


  // if the file is available, write to it:
  if (logfile) 
  {
    //logfile.println(dataString);
    serializeJsonPretty(doc, logfile);
    // print to the serial port too:
    //Serial.println(dataString);
    serializeJsonPretty(doc, Serial);
  }

  logfile.flush();
}
