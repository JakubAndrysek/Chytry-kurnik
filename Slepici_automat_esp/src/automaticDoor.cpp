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
    if (!EEPROM.begin(EEPROM_SIZE))
    {
        Serial.println("failed to initialise EEPROM");
    }
    delay(500);
    
   

}

void AutomaticDoor::begin()
{
    _minuteLast = getMinute();
    _hourLast = getHour();
    readEEP();

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
    Serial.println(EEPROM.read(0));
    Serial.println(EEPROM.read(1));        
}


int AutomaticDoor::getHour()
{
    RtcDateTime dt = GetDateTime();
    return dt.Hour();
}

int AutomaticDoor::getMinute()
{
    RtcDateTime dt = GetDateTime();
    return dt.Minute();
}

int AutomaticDoor::getSecond()
{
    RtcDateTime dt = GetDateTime();
    return dt.Second();
}

void AutomaticDoor::open()
{
    Serial.println("Opening");
    // rUp();
    // delay(_tMove);
    // rStop();


    switch (oState)
    {
    case 0:
        oTime = millis()/1000; //to seconds
        oState++;   //next state
        break;
    case 1:
        rUp();
        delay(_tMove);
        rStop();
        oState++;
        break;
    
    case 2:
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
        break;
    case 1:
        rDown();
        delay(_tMove);
        rStop();
        cState++;
        break;
    
    case 2:
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
    _secondNow = getSecond();
    Serial.printf("Now-H%d M%d S%d; Open-H%d M%d; Close-H%d M%d\n", _hourNow,_minuteNow, _secondNow,_hourOpen,_minuteOpen,_hourClose,_minuteClose);
}

void AutomaticDoor::setHourOpen(int hour)
{
    //_hourOpen = hour;
    EEPROM.write(EEoH, hour);
    EEPROM.commit();
    _hourOpen = EEPROM.read(EEoH);
}

void AutomaticDoor::setMinuteOpen(int minute)
{
    //_minuteOpen = minute;
    EEPROM.write(EEoM, minute);
    EEPROM.commit();
    _minuteOpen = EEPROM.read(EEoM);
}

void AutomaticDoor::setHourClose(int hour)
{
    _hourClose = hour;
    EEPROM.write(EEcH, hour);
    EEPROM.commit();

}

void AutomaticDoor::setMinuteClose(int minute)
{
    _minuteClose = minute;
    EEPROM.write(EEcM, minute);
    EEPROM.commit();

}

void AutomaticDoor::setActualTime(int hour, int minute)
{
    SetDateTime(RtcDateTime(2020,1,30,hour,minute,0));
}

void AutomaticDoor::setMove(int tMove)
{
    _tMove = tMove;
}

String AutomaticDoor::getTimeOpen()
{
    return String(EEPROM.read(EEoH)) + ":" + String(EEPROM.read(EEoM));
}

String AutomaticDoor::getTimeClose()
{
    return String(EEPROM.read(EEcH)) + ":" + String(EEPROM.read(EEcM));
}

String AutomaticDoor::getMove()
{
    return String(_tMove);
}


void AutomaticDoor::rUp()
{
    digitalWrite(RELAY_A, true);
    digitalWrite(RELAY_B, false);
}


void AutomaticDoor::rDown()
{
    digitalWrite(RELAY_A, false);
    digitalWrite(RELAY_B, true);
}

void AutomaticDoor::rStop()
{
    digitalWrite(RELAY_A, true);
    digitalWrite(RELAY_B, true);
}

int AutomaticDoor::ret()
{
    return EEPROM.read(EEoH);
}