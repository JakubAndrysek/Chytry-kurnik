#pragma once

#include <Arduino.h>
#include <L298N.h>
#include <Wire.h> 
#include <RtcDS3231.h>

#define EN 25
#define IN1 26
#define IN2 27

class AutomaticDoor : public L298N, public RtcDS3231<TwoWire>, public RtcDateTime
{
    private:
        int _open;
        int _close;
        int _tMove;
        int _timeNow;
        int _timeLast;
        bool _openState;
        bool _closeState;


    public:
        AutomaticDoor(int open, int close, int tMove);
        void begin();
        int getTime();
        void open();
        void close();
        bool timeToOpen();
        bool timeToClose();
        void printDateTime();
    
};