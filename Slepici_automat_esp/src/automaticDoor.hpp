#pragma once

#include <Arduino.h>
#include <Wire.h> 
#include <RtcDS3231.h>
#include <EEPROM.h>

//#include <DNSServer>


#define RELAY_A 32
#define RELAY_B 33
#define EEPROM_SIZE 10

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
        int oTime = 0;
        int cTime = 0;
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
        void open();
        void close();
        bool timeToOpen();
        bool timeToClose();
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
        int ret();

    
};

