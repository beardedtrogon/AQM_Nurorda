#include <avr/sleep.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(4,5,2);
RtcDS1302<ThreeWire> Rtc(myWire);
int smokeA0 = A5;
int sensorThres = 400;
 
void setup() {
  //to setup CO2 checker
  pinMode(smokeA0, INPUT);
  Serial.begin(9600);

  //to setup RTC
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();
}
 
void loop() {
  RtcDateTime now = Rtc.GetDateTime();

  printDateTime(now);
  Serial.println();

  if(!now.IsValid()){
    Serial.println("RTC lost confidence in the DateTime!");
  }

  int analogSensor = analogRead(smokeA0);
  Serial.print(" air_quality ");
  Serial.println(analogSensor);
  if(analogSensor > 350){
    Serial.println("Dangerous level");
  }
  delay(1000);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt){
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}
