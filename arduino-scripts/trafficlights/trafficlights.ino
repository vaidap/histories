int ledPinGreen = 9;
int ledPinYellow = 6;
int ledPinRed = 5;

void setup() {
}

void loop() {
  for (int traffic_time = 0 ; traffic_time <=3; traffic_time += 1) {
    if (traffic_time == 0) {
        analogWrite(ledPinGreen, 255);
        analogWrite(ledPinYellow, 0);
        analogWrite(ledPinRed, 0);
      }
     else if (traffic_time == 1) {
        analogWrite(ledPinGreen, 0);
        analogWrite(ledPinYellow, 255);
        analogWrite(ledPinRed, 0);
      }
      else if (traffic_time == 2) {
        analogWrite(ledPinGreen, 0);
        analogWrite(ledPinYellow, 0);
        analogWrite(ledPinRed, 255);
        }
       else {
        analogWrite(ledPinGreen, 0);
        analogWrite(ledPinYellow, 255);
        analogWrite(ledPinRed, 255);
        }
    delay(3000);
    }
 
  // Debug
  analogWrite(ledPinGreen, 255);
  analogWrite(ledPinYellow, 255);
  analogWrite(ledPinRed, 255);
}
