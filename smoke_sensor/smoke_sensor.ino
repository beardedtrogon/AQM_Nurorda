#include <avr/sleep.h>
//#include <ArduinoLowPower.h>
int smokeA0 = A5;
int sensorThres = 400;
 
void setup() {
  pinMode(smokeA0, INPUT);
  Serial.begin(9600);

}
 
void loop() {
  int analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  if(analogSensor > 350){
    Serial.println("Dangerous level");
  }
  delay(10000);
}
