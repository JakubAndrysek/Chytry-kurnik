#include "automaticDoor.hpp"
#include <Arduino.h>
#include <string>
#include <EEPROM.h> 


/**
 * Constructor
 * @param open Time to open the door in 24h format
 * @param close Time to close the door in 24h format
 * @param tMove Time to open/close the door in seconds
 * 
 */ 


AutomaticDoor::AutomaticDoor(int tMove) :  RtcDS3231<TwoWire>(Wire)
{
    Begin(); //RTC begin
    _tMove = tMove*1000;
    _openState = false;
    _closeState = false;
    pinMode(RELAY_A, OUTPUT);
    pinMode(RELAY_B, OUTPUT);
    digitalWrite(RELAY_A, true);
    digitalWrite(RELAY_B, true); 
    dprintf("Door constructor");
    delay(500);
    
   

}

void AutomaticDoor::begin()
{
    _minuteLast = getMinute();
    _hourLast = getHour();
    readEEP();
    dprintf("Door begin");

    // Serial.println(EEPROM.read(EEoH));
    // delay(1000);
    // Serial.println(EEPROM.read(EEoM));
    // Serial.print("EE");
    // Serial.println(EEPROM.read(0));
    // Serial.println(EEPROM.read(1));    
}

void AutomaticDoor::readEEP()
{
    _hourOpen = EEPROM.read(EEoH);
    _minuteOpen = EEPROM.read(EEoM);
    _hourClose = EEPROM.read(EEcH);
    _minuteClose = EEPROM.read(EEcM); 
    // Serial.println(EEPROM.read(0));
    // Serial.println(EEPROM.read(1));
    dprintf("Read EEPROM - setup variables") ;       
}


int AutomaticDoor::getHour()
{
    RtcDateTime dt = GetDateTime();
    //dprintf("GetH-%d",dt.Hour());
    return dt.Hour();
}

int AutomaticDoor::getMinute()
{
    RtcDateTime dt = GetDateTime();
    //dprintf("GetM-%d",dt.Minute());
    return dt.Minute();
}

int AutomaticDoor::getSecond()
{
    RtcDateTime dt = GetDateTime();
    //dprintf("GetS-%d",dt.Second());
    return dt.Second();
}

void AutomaticDoor::open()
{
    Serial.println("Opening");

    switch (oState)
    {
    case 0:
        oTime = millis()/1000; //to seconds
        dprintf("Opening-Begin");
        oState++;   //next state
        break;
    case 1:
        dprintf("Opening-Start");
        rUp();
        delay(_tMove);
        rStop();
        oState++;
        break;
    
    case 2:
        dprintf("Opening-Ending");
        if(millis()/1000-oTime>60)    //for one door opening
        {
            oState = 0;
        }
        break;
    }
}

void AutomaticDoor::close()
{
    Serial.println("Closeing");
    // rDown();
    // delay(_tMove);
    // rStop();

    switch (cState)
    {
    case 0:
        cTime = millis()/1000; //to seconds
        cState++;   //next state
        dprintf("Closing-Begin");
        break;
    case 1:
        dprintf("Closing-Start");    
        rDown();
        delay(_tMove);
        rStop();
        cState++;
        break;
    
    case 2:
        dprintf("Closing-Ending");
        if(millis()/1000-cTime>60)    //for one door opening
        {
            cState = 0;
        }
        break;
    }

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

void AutomaticDoor::printDateTime()
{

    _minuteNow = getMinute();
    _hourNow = getHour();
    _secondNow = getSecond();
    Serial.printf("Now-H%d M%d S%d; Open-H%d M%d; Close-H%d M%d Od zapnuti%ds\n", _hourNow,_minuteNow, _secondNow,_hourOpen,_minuteOpen,_hourClose,_minuteClose, millis()/1000);
}

void AutomaticDoor::setHourOpen(int hour)
{
    //_hourOpen = hour;
    dprintf("SetOpenH-%d", hour);
    EEPROM.write(EEoH, hour);
    EEPROM.commit();
    _hourOpen = EEPROM.read(EEoH);
}

void AutomaticDoor::setMinuteOpen(int minute)
{
    //_minuteOpen = minute;
    dprintf("SetOpenM-%d", minute);
    EEPROM.write(EEoM, minute);
    EEPROM.commit();
    _minuteOpen = EEPROM.read(EEoM);
}

void AutomaticDoor::setHourClose(int hour)
{
    //_hourClose = hour;
    dprintf("SetCloseH-%d", hour);
    EEPROM.write(EEcH, hour);
    EEPROM.commit();
    _hourClose = EEPROM.read(EEoM);

}

void AutomaticDoor::setMinuteClose(int minute)
{
    //_minuteClose = minute;
    dprintf("SetCloseM-%d", minute);
    EEPROM.write(EEcM, minute);
    EEPROM.commit();
    _minuteClose = EEPROM.read(EEcM);
}

void AutomaticDoor::setActualTime(int hour, int minute)
{
    dprintf("SetActualTime");
    SetDateTime(RtcDateTime(2020,1,30,hour,minute,0));
}

void AutomaticDoor::setMove(int tMove)
{
    dprintf("SetMove");
    _tMove = tMove;
}

String AutomaticDoor::getTimeOpen()
{
    dprintf("GetOpenTime");
    return String(EEPROM.read(EEoH)) + ":" + String(EEPROM.read(EEoM));
}

String AutomaticDoor::getTimeClose()
{
    dprintf("GetCloseTime");
    return String(EEPROM.read(EEcH)) + ":" + String(EEPROM.read(EEcM));
}

String AutomaticDoor::getMove()
{
    dprintf("GetMove");
    return String(_tMove);
}


void AutomaticDoor::rUp()
{
    dprintf("rUp");
    digitalWrite(RELAY_A, true);
    digitalWrite(RELAY_B, false);
}


void AutomaticDoor::rDown()
{
    dprintf("rDown");
    digitalWrite(RELAY_A, false);
    digitalWrite(RELAY_B, true);
}

void AutomaticDoor::rStop()
{
    dprintf("rStop");
    digitalWrite(RELAY_A, true);
    digitalWrite(RELAY_B, true);
}

