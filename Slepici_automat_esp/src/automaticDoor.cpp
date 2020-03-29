#include "automaticDoor.hpp"
#include <Arduino.h>


/**
 * Constructor
 * @param open Time to open the door in 24h format
 * @param close Time to close the door in 24h format
 * @param tMove Time to open/close the door in seconds
 * 
 */ 


AutomaticDoor::AutomaticDoor(int open, int close, int tMove) : L298N(EN, IN1, IN2) , RtcDS3231<TwoWire>(Wire)
{
    Begin();
    _open = open;
    _close = close;
    _tMove = tMove*1000;
    _openState = false;
    _closeState = false;


}

void AutomaticDoor::begin()
{
    _timeLast = getTime();
    setSpeed(255);  
}


int AutomaticDoor::getTime()
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
    _timeNow = getTime();

    if(_timeNow == _open && _openState == false)
    {
        _openState = true;
        _timeLast = _timeNow;
        return true;
    }

    if(_timeNow != _timeLast)
    {
        _openState = false;
    }

    return false;    
}

bool AutomaticDoor::timeToClose()
{
    _timeNow = getTime();

    if(_timeNow == _close && _closeState == false)
    {
        _closeState = true;
        _timeLast = _timeNow;
        return true;
    }

    if(_timeNow != _timeLast)
    {
        _closeState = false;
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
    Serial.println(datestring);
}

void changeOpenTime(int hour, int minute)
{
    
}

