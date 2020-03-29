#include <Arduino.h>
#include "automaticDoor.hpp"

AutomaticDoor dvere(7, 40, 1);

void setup() {
  Serial.begin(115200);
  Serial.println("Start");

  dvere.begin();

  //dvere.open();

}

void loop() {
  Serial.print("Cas");

  
  dvere.printDateTime();
  delay(500);

  if(dvere.timeToOpen())
  {
    dvere.open();
    Serial.println("Open function");
  }

  if(dvere.timeToClose())
  {
    dvere.close();
    Serial.println("Close function");
  }





}


