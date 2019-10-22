/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card mounted on the YunShield/Yún using the Bridge library.

 The circuit:
 * analog sensors on analog pins 0, 1 and 2
 * SD card attached to SD card slot of the YunShield/Yún

 Prepare your SD card creating an empty folder in the SD root
 named "arduino". This will ensure that the Yún will create a link
 to the SD to the "/mnt/sd" path.

 You can remove the SD card while the Linux and the
 sketch are running but be careful not to remove it while
 the system is writing to it.

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 adapted to the Yún Bridge library 20 Jun 2013
 by Federico Vanzati
 modified  21 Jun 2013
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/YunDatalogger

 */

#include <FileIO.h>

int loudness; // A0
int counter = 0; // for file write

int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, cm, inches;

int light_1 = 0;
int light_2 = 0;

void setup() {
  // Initialize the Bridge and the Serial
  Bridge.begin();
  Serial.begin(9600);

  //Define inputs and outputs for ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  
  FileSystem.begin();

  while (!SerialUSB); // wait for Serial port to connect.
  SerialUSB.println("Filesystem datalogger\n");
}


void loop() {

    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135

  // Light
  light_1 = analogRead(A1);
  light_2 = analogRead(A2);
  
  // make a string that start with a timestamp for assembling the data to log:
  String dataString;
  dataString += getTimeStamp();
  dataString += " = ";

  // read three sensors and append to the string:
//  for (int analogPin = 0; analogPin < 3; analogPin++) {
//    int sensor = analogRead(analogPin);
//    dataString += String(sensor);
//    if (analogPin < 2) {
//      dataString += ",";  // separate the values with a comma
//    }
//  }

dataString += "testing the sd card";

    loudness = analogRead(0);
    Serial.println( "Loudness: " + String(loudness) + " Ultrasonic: " + String(cm) + "cm" + " Light 1 : " + String(light_1) + " Light 2 : " + String(light_2));

    Serial.println("Printing to SD card...");

      // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // The FileSystem card is mounted at the following "/mnt/FileSystema1"
  File dataFile = FileSystem.open("/mnt/sd/datalog.csv", FILE_APPEND);

  // if the file is available, write to it:
  if (dataFile) {
//    dataFile.println(dataString);

    unsigned long currentTime = millis();
    dataFile.print(currentTime);
    dataFile.print(",");
    dataFile.print(String(loudness));
    dataFile.print(",");
    dataFile.print(String(cm));
    dataFile.print(",");
    dataFile.print(String(light_1));
    dataFile.print(",");
    dataFile.println(String(light_2));

    
    dataFile.close();
    // print to the serial port too:
//    SerialUSB.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    SerialUSB.println("error opening datalog.csv");
  }

//  // For writing to SD card
//  counter += 1;
//  if (counter == 9) {
//    counter = 0;  
//    }

    delay(1000);

}

// This function return a string with the time stamp
String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
  //             T for the time hh:mm:ss
  time.run();  // run the command

  // read the output of the command
  while (time.available() > 0) {
    char c = time.read();
    if (c != '\n') {
      result += c;
    }
  }

  return result;
}
