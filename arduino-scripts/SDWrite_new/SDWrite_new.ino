// link between the computer and the SoftSerial Shield
// at 9600 bps 8-N-1
// Computer is connected to Hardware UART
// SoftSerial Shield is connected to the Software UART:D2&D3

#include <SoftwareSerial.h>
#include <SD.h>

#define CHIP_SELECT 10                         // For an Uno it's 10, we're using a Yun tho? can use ReadWrite default Example instead as well

SoftwareSerial SoftSerial(8, 7);
File logfile;
unsigned long time;

//
// Setup
//
void setup()
{
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
  strcpy(filename, "LOGFILE00.csv");
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
  time = millis();

  //int sensor = analogRead(A0);
  String dataString = "";

  //dataString += String(sensor);

  // if the file is available, write to it:
  if (logfile) 
  {
    //logfile.println(dataString);

    logfile.print(String(time));
    logfile.print(",");
    logfile.print("oneOne");
    logfile.print(",");
    logfile.println("twoOne");

    logfile.print(String(time));
    logfile.print(",");
    logfile.print("oneTwo");
    logfile.print(",");
    logfile.println("TwoTwo");

    // print to the serial port too:
    Serial.print(String(time));
    Serial.print(",");
    Serial.print("oneOne");
    Serial.print(",");
    Serial.println("twoOne");

    Serial.print(String(time));
    Serial.print(",");
    Serial.print("oneTwo");
    Serial.print(",");
    Serial.println("TwoTwo");
  }

  logfile.flush();
}
