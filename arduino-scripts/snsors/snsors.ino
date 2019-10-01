/*macro definitions of PIR motion sensor pin and LED pin*/
#define PIR_MOTION_SENSOR 2//Use pin 2 to receive the signal from the module
int loudness;

void setup()
{
    pinMode(PIR_MOTION_SENSOR, INPUT);
    Serial.begin(9600);  

}

void loop()
{
    if(digitalRead(PIR_MOTION_SENSOR))//if it detects the moving people?
        Serial.println("Hi,people is coming");
    else
        Serial.println("Watching");
    
    // loudness sensor
    loudness = analogRead(0);
    Serial.println(loudness);

 delay(200);
}
