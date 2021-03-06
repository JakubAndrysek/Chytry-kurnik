#pragma once

#include <Arduino.h>
#include <Wire.h> 
#include <RtcDS3231.h>
#include <EEPROM.h>

//#include <DNSServer>



// #define RELAY_A 18
// #define RELAY_B 19
// #define EEPROM_SIZE 10
// #define BUTTON_E1 5
// // #define BUTTON_E2 17
// #define BUTTON_E2 2
// #define OPEN_LIMIT 5
// #define BUTTON_UP 16
// #define BUTTON_DOWN 4


#define RELAY_A 18
#define RELAY_B 19
#define EEPROM_SIZE 10
#define BUTTON_E1 5
// #define BUTTON_E2 17
#define BUTTON_E2 15
#define OPEN_LIMIT 5
#define BUTTON_UP 16
#define BUTTON_DOWN 4


//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##
  // Pomucka pro ladeni. Pokud to nepouzijete, klidne to smazte.

  // Po odkomentovani se nebudou tisknout ladici zpravy.
  // #define NDEBUG

  // Prikaz dprintf pro tisk ladicich zprav. Funguje stejne jako printf,
  // ale jde jej globalne vypnout odkomentovanim definice symbolu NDEBUG.
  // Vhodny pro povidani s uzivatelem v prubehu ladeni.
  #ifdef NDEBUG
    #define dprintf(...)
  #else
    #define dprintf(...) Serial.printf(__VA_ARGS__);Serial.println()
  #endif //NDEBUG
//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##//##

class AutomaticDoor : public RtcDS3231<TwoWire>, public RtcDateTime
{
    private:
        int _hourOpen;
        int _minuteOpen;
        int _hourClose;
        int _minuteClose;
        int _tMove;
        int _secondNow;
        int _minuteNow;
        int _hourNow;        
        int _minuteLast;        
        int _hourLast;        
        bool _openState;
        bool _closeState;
        int oState = 0;
        int cState = 0;
        long long oTime = 0;
        long long cTime = 0;
        //EEPROM ADRESS
        int EEoM = 0;
        int EEoH = 1;
        int EEcM = 3;
        int EEcH = 4;

    public:
        AutomaticDoor(int tMove);
        void begin();
        void readEEP();
        int getHour();
        int getMinute();
        int getSecond();
        bool getButtonEnd1();
        bool getButtonEnd2();
        bool getButtonUp();
        bool getButtonDown();
        void open();
        void close();
        bool timeToOpen();
        bool timeToClose();
        bool timeToReboot();
        void printDateTime();
        void setHourOpen(int hour);
        void setMinuteOpen(int minute);
        void setHourClose(int hour);
        void setMinuteClose(int minute);
        void setActualTime(int hour, int minute);
        void setMove(int tMove);
        int StringToInt(String s);
        String getTimeOpen();
        String getTimeClose();
        String getMove();
        void rUp();
        void rDown();
        void rStop();

    
};

