#include "automaticDoor.hpp"
#include <Arduino.h>
#include <string> 


/**
 * Constructor
 * @param open Time to open the door in 24h format
 * @param close Time to close the door in 24h format
 * @param tMove Time to open/close the door in seconds
 * 
 */ 


AutomaticDoor::AutomaticDoor(int hourOpen, int minuteOpen, int hourClose, int minuteClose , int tMove) : L298N(EN, IN1, IN2) , RtcDS3231<TwoWire>(Wire)
{
    Begin();
    _hourOpen = hourOpen;
    _minuteOpen = minuteOpen;
    _hourClose = hourClose;
    _minuteClose = minuteClose;
    _tMove = tMove*1000;
    _openState = false;
    _closeState = false;


}

void AutomaticDoor::begin()
{
    _minuteLast = getMinute();
    _hourLast = getHour();
    setSpeed(255);  
}


int AutomaticDoor::getHour()
{
    RtcDateTime dt = GetDateTime();
    return dt.Minute();
}

int AutomaticDoor::getMinute()
{
    RtcDateTime dt = GetDateTime();
    return dt.Second();
}

void AutomaticDoor::open()
{
    Serial.println("Opening");
    forward();
    delay(_tMove);
    stop();
}

void AutomaticDoor::close()
{
    Serial.println("Closeing");
    backward();
    delay(_tMove);
    stop();
}

bool AutomaticDoor::timeToOpen()
{
    _minuteNow = getMinute();
    _hourNow = getHour();

    if(_minuteNow == _minuteOpen && _hourNow == _hourOpen)
    {
        _minuteLast = _minuteNow;
        _hourLast = _hourNow;
        return true;
    }

    return false;    
}

bool AutomaticDoor::timeToClose()
{
    _minuteNow = getMinute();
    _hourNow = getHour();
    //Serial.printf("Now-H%d M%d; Last-H%d M%d; state%d; Poz-H%d M%d\n", _hourNow,_minuteNow,_hourNow,_minuteLast,_closeState,_hourClose,_minuteClose);


    if(_minuteNow == _minuteClose && _hourNow == _hourClose )
    {
        _minuteLast = _minuteNow;
        _hourLast = _hourNow;
        return true;
    }
    return false;    
}


#define countof(a) (sizeof(a) / sizeof(a[0]))
void AutomaticDoor::printDateTime()
{
	RtcDateTime dt = GetDateTime();
    
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
    //Serial.println(datestring);

    _minuteNow = getMinute();
    _hourNow = getHour();
    Serial.printf("Now-H%d M%d; Open-H%d M%d; Close-H%d M%d\n", _hourNow,_minuteNow,_hourOpen,_minuteOpen,_hourClose,_minuteClose);
}

void AutomaticDoor::setHourOpen(int hour)
{
    _hourOpen = hour;
}

void AutomaticDoor::setMinuteOpen(int minute)
{
    _minuteOpen = minute;
}

void AutomaticDoor::setHourClose(int hour)
{
    _hourClose = hour;
}

void AutomaticDoor::setMinuteClose(int minute)
{
    _minuteClose = minute;
}

void AutomaticDoor::setMove(int tMove)
{
    _tMove = tMove;
}

String AutomaticDoor::getTimeOpen()
{
    return String(_hourOpen) + ":" + String(_minuteOpen);
}

String AutomaticDoor::getTimeClose()
{
    return String(_hourClose) + ":" + String(_minuteClose);
}

String AutomaticDoor::getMove()
{
    return String(_tMove);
}