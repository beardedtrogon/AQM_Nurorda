#include <avr/sleep.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))
#define NUM_SAMPLES 10

ThreeWire myWire(4,5,2);
RtcDS1302<ThreeWire> Rtc(myWire);

int led = 13;
int sum = 0; //sum of samples taken
int smokeA0 = A5;
int sensorThres = 400;

unsigned char sample_count = 0; // current sample number
float voltage = 0.0; // calculated voltage

void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
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
  while(sample_count < NUM_SAMPLES){
    sum += analogRead(A2);
    sample_count++;
  }

  voltage = ((float)sum / (float)NUM_SAMPLES * 5.015) / 1024.0;

  if(voltage * 2 < 6.0){
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }
  Serial.print(voltage * 2);
  Serial.println(" V");
  
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
  sample_count = 0;
  sum = 0;
  delay(1000);
}

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
