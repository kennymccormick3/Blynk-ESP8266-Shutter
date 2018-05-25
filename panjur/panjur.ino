#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "848ab248064a43a6aa050f7aa095d0d1";
const unsigned long interval = 2500;
unsigned long last = 0;
String levelString;
String shutterStatus;
String percentage = "%";
int levelInt = 0;
int pinStateV1;
int pinStateV2;

BlynkTimer timer;

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, "Superonline", "kenny--72");
  pinMode(5, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  digitalWrite(5, HIGH);

}

BLYNK_WRITE(V0){
  if (param.asInt() == 0){
    digitalWrite(16, LOW);
    digitalWrite(5, HIGH);
    timer.setTimeout(26000L, stopper);
    Blynk.virtualWrite(V3, "%100 Açık");
    levelInt = 0;
  }
  else{
    digitalWrite(5, LOW);
    digitalWrite(16, HIGH);
    timer.setTimeout(26000L, stopper);
    Blynk.virtualWrite(V3, "%100 Kapalı");
    levelInt = 100;
  }
}

BLYNK_WRITE(V1){
  if (param.asInt() == 0){
    digitalWrite(16, HIGH);
    pinStateV1 = 0;
  }
  else{
    digitalWrite(16,LOW);
    digitalWrite(5, HIGH);
    pinStateV1 = 1;
  }
}

BLYNK_WRITE(V2){
  if (param.asInt() == 0){
    digitalWrite(5, HIGH);
    //Blynk.virtualWrite(V3, "test1");
    pinStateV2 = 0;
  }
  else{
    digitalWrite(5,LOW);
    digitalWrite(16, HIGH);
    pinStateV2 = 1;
  }

  
}

void stopper(){
  digitalWrite(16, HIGH);
  digitalWrite(5, HIGH);
}



void loop()
{
  Blynk.run();
  timer.run();

  unsigned long current = millis();
  if((unsigned long)(current-last) >= interval && pinStateV2 == 1 && levelInt < 100){
    last = millis();
    levelInt += 10;
    levelString = " Kapalı";
    shutterStatus = percentage + levelInt + levelString;
    Blynk.virtualWrite(V3, shutterStatus);
  }

  if((unsigned long)(current-last) >= interval && pinStateV1 == 1 && levelInt > 0){
    last = millis();
    levelInt -= 10;
    levelString = " Açık";
    shutterStatus = percentage + ((levelInt-100)*(-1)) + levelString;
    Blynk.virtualWrite(V3, shutterStatus);
  }
}
