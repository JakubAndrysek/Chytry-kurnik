#include <Arduino.h>
#include <L298N.h>
#include <Wire.h> 
#include <RtcDS3231.h>

#define EN 2
#define IN1 3
#define IN2 4

#define MOVE_TIME 5000  //5 vterin

#define countof(a) (sizeof(a) / sizeof(a[0]))

RtcDS3231<TwoWire> Rtc(Wire);
L298N motor(EN, IN1, IN2);


int hour_last = 0;


void open()
{
  motor.backward();
  delay(MOVE_TIME);
  motor.stop();
}

void close()
{
  motor.forward();
  delay(MOVE_TIME);
  motor.stop();
}


void printDateTime(const RtcDateTime& dt)
{
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

void setup () 
{
  RtcDateTime dt = Rtc.GetDateTime();
  
  Serial.begin(9600);    
  Rtc.Begin();
  printDateTime(Rtc.GetDateTime());
  motor.setSpeed(255); // an integer between 0 and 255

  open();

  hour_last = dt.Second();
}

void loop () 
{  
  RtcDateTime dt = Rtc.GetDateTime();

  int hour = dt.Second();
  if(hour==7 && hour_last != hour)
  {
    Serial.println("Open");
    open();
    hour_last = hour;
  }

  if(hour==40 && hour_last != hour)
  {
    Serial.println("Close");
    close();
    hour_last = hour;
  }  

  
  printDateTime(Rtc.GetDateTime());

  delay(1000);

}




