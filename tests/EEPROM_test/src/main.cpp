#include <Arduino.h>
#include <EEPROM.h>
#include "timercpp.h"

#define EEPROM_SIZE 10
int a = 0, b =1;

void setup() {
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);
  // EEPROM.write(a, 18);
  // EEPROM.write(b, 33);
  EEPROM.commit();
  Serial.println(EEPROM.read(a));


  Serial.println(EEPROM.read(b));


}

void loop() {
  
}