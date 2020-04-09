#pragma once

#include <Arduino.h>
#include <Wire.h> 
#include <RtcDS3231.h>

//#include <DNSServer>


#define RELAY_A 32
#define RELAY_B 33

class AutomaticDoor : public RtcDS3231<TwoWire>, public RtcDateTime
{
    private:
        int _hourOpen;
        int _minuteOpen;
        int _hourClose;
        int _minuteClose;
        int _tMove;
        int _minuteNow;
        int _hourNow;        
        int _minuteLast;        
        int _hourLast;        
        bool _openState;
        bool _closeState;

    public:
        AutomaticDoor(int hourOpen, int minuteOpen, int hourClose, int minuteClose , int tMove);
        void begin();
        int getHour();
        int getMinute();
        void open();
        void close();
        bool timeToOpen();
        bool timeToClose();
        void printDateTime();
        void setHourOpen(int hour);
        void setMinuteOpen(int minute);
        void setHourClose(int hour);
        void setMinuteClose(int minute);
        void setMove(int tMove);
        int StringToInt(String s);
        String getTimeOpen();
        String getTimeClose();
        String getMove();
        void rUp();
        void rDown();
        void rStop();

    
};

