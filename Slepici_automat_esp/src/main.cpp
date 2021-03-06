#include <Arduino.h>
#include "automaticDoor.hpp"
#include <WiFi.h>
#include <EEPROM.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "webpage.h"



const char* ssid = "Chytry-kurnik";
const char* password = "naseslepice";

const char* OpenH = "open-h";
const char* OpenM = "open-m";
const char* CloseH = "close-h";
const char* CloseM = "close-m";
const char* ActualH = "act-h";
const char* ActualM = "act-m";

const char* Move = "move";

int NowTime  = 0;
long LastAutoTime = 0;
int AutoTime = 200;

long LastMesageTime = 0;
int MesageTime = 1000;

long LastWebTime = 0;
int WebTime = 50;


AutomaticDoor dvere(5);
WebServer server(80);


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    String arg = " " + server.argName(i) + ": " + server.arg(i) + "\n";
    Serial.println(arg);
  }
  server.send(404, "text/plain", message);
}


void handleRoot() {
  String page ;
  page += page_intro;
  page += "<h3>Čas otevření kurníku "+ dvere.getTimeOpen() + "</h3>" + page_open;
  page += "<h3>Čas zavření kurníku "+ dvere.getTimeClose() + "</h3>" + page_close;
  page += "<h3>Aktuální čas "+ String(dvere.getHour()) + ":" + String(dvere.getMinute())+ "</h3>" + page_time;
  page += page_end;

  server.send(200, "text/html", page);
}

void handleGet() {

  if (server.argName(0)==OpenH) {     
    dvere.setHourOpen(stoi(server.arg(0)));
    dvere.setMinuteOpen(stoi(server.arg(1)));
    Serial.println(OpenH);
    Serial.println(dvere.getTimeOpen());
  }
  // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
  else if (server.argName(0) == CloseH) {
    dvere.setHourClose(stoi(server.arg(0)));
    dvere.setMinuteClose(stoi(server.arg(1)));
    Serial.println(CloseH);
    Serial.println(dvere.getTimeClose());
  }
  else if (server.argName(0) == ActualH) {
    int h = (stoi(server.arg(0)));
    int m = (stoi(server.arg(1)));
    dvere.setActualTime(h, m);
    Serial.println(ActualH);
    Serial.printf("%d:%d",h, m);
  }    
  else {
    server.send(200, "text/html", "Nekde nastala chyba");
    dprintf("No parameter send");
    return;
  }
  dvere.readEEP();
  server.send(200, "text/html", "<h1>Nastaveni bylo upraveno</h1><br><a href=\"/\">Zpet na Nastaveni</a>");
}


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  dprintf("DEBUG JEDE!!!");
  
  EEPROM.begin(EEPROM_SIZE);
  dvere.begin();
  

  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  if (MDNS.begin("kurnik")) {
    Serial.println("MDNS responder started");
  }


  dvere.printDateTime();

  server.on("/", handleRoot);

  server.on("/get", handleGet);
    
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  //dvere.setActualTime(20, 30);

  // dvere.setHourOpen(17);
  // dvere.setMinuteOpen(35);
  // dvere.setHourClose(19);
  // dvere.setMinuteClose(51);
  //Serial.println(dvere.ret());


  //dvere.readEEP();


}

void loop() {
 

  NowTime = millis();

  // while (1)
  // {
  //   Serial.printf("Up: %d; Down: %d; E1: %d; E2: %d \n", dvere.getButtonUp(), dvere.getButtonDown(), dvere.getButtonEnd1(), dvere.getButtonEnd2());


  //   delay(500);
  // }
  
  

  //Auto open/close thread
  if(NowTime-LastAutoTime>=AutoTime)
  {
    //dprintf("AutoTime");
    if(dvere.timeToOpen())
    {
      dprintf("Open function");
      dvere.open();
    }
    if(dvere.timeToClose())
    {
      dprintf("Close function");
      dvere.close();
    }
    
    if(!dvere.getButtonUp())
    {
      dprintf("Open function-button");
      delay(200);
      dvere.open();
    }

    if(!dvere.getButtonDown())
    {
      dprintf("Close function-button");
      delay(200);
      dvere.close();
    }
    if(dvere.timeToReboot())
    {
      dprintf("REBOOTING!!!");
      ESP.restart();
    }

    LastAutoTime = NowTime;
  }

  //Message thread
  if(NowTime-LastMesageTime>=MesageTime)
  {
    //dprintf("Message");
    dvere.printDateTime();
    LastMesageTime = NowTime;
  }

  //Web thread
  if(NowTime-LastWebTime>=WebTime)
  {
    server.handleClient();    
    LastWebTime = NowTime;
  }



   




}


