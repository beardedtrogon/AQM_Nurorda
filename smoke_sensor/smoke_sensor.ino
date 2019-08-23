#include <avr/sleep.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <SPI.h>
#include <SD.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))
#define NUM_SAMPLES 10

File myFile;
ThreeWire myWire(4,5,2);
RtcDS1302<ThreeWire> Rtc(myWire);

int led = 13;
int sum = 0; //sum of samples taken
int smokeA0 = A5;
int sensorThres = 400;

const int chipSelect = 10;
unsigned char sample_count = 0; // current sample number
float voltage = 0.0; // calculated voltage

void setup() {
  Serial.begin(9600);
  
  while(!Serial){
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  
  
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  //to setup CO2 checker
  pinMode(smokeA0, INPUT);
  
  //to setup RTC
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();
}
 
void loop() {
  myFile = SD.open("TEST.txt", FILE_WRITE);// open the file test.txt  
  
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
  
  if(!now.IsValid()){
    Serial.println("RTC lost confidence in the DateTime!");
  }

  int analogSensor = analogRead(smokeA0);
  
  // Write to file
  if(myFile){
    printDateTime(now);
    myFile.print(" air_quality ");
    myFile.println(analogSensor);
    myFile.println(" ");
    if(analogSensor > 350){
      myFile.println("Dangerous level ");
    }
    myFile.close();
  }
  else{
    Serial.println("Error opening test.txt");
  }
  
//  printDateTime(now);
//  Serial.println();
//
//  Serial.print(" air_quality ");
//  Serial.println(analogSensor);
//  if(analogSensor > 350){
//    Serial.println("Dangerous level");
//  }
  // Reading the file
  myFile = SD.open("TEST.txt");
  if(myFile){
    Serial.println("TEST.txt:");
    while(myFile.available()){
      Serial.write(myFile.read());
    }
    myFile.close();
  }else{
    Serial.println("error opening test.txt");
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
    myFile.println(datestring);
}
