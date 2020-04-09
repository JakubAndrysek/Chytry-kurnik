#include <Arduino.h>
#include "automaticDoor.hpp"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>


const char* ssid = "Kurnik";
const char* password = "NaseSlepice";

const char* OpenH = "open-h";
const char* OpenM = "open-m";
const char* CloseH = "close-h";
const char* CloseM = "close-m";

const char* Move = "move";


AutomaticDoor dvere(5);

AsyncWebServer server(80);
DNSServer gDnsServer;


// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html><head>
    <title>ESP Input Form</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    </head><body>
    <h1>Já jsem chytrý kurník</h1>
    <p>Naše slepice mají webové stránky :-)</p>
    <h4>By Kuba Andrysek<h4>

    <h2>Nastavení</h2>
    <h3>Čas otevření kurníku:</h3>
    <form action="/get">
      Hodiny: <input type="number" name="open-h">
      <br>
      Minuty: <input type="number" name="open-m">
      <br>
      <br>
      <input type="submit" value="Submit">
    </form><br>

    <h3>Čas zavření kurníku</h3>
    <form action="/get">
      Hodiny: <input type="number" name="close-h">
      <br>
      Minuty: <input type="number" name="close-m">
      <br>
      <br>
      <input type="submit" value="Submit">
    </form><br>

    <h3>Aktuální čas:</h3>
    <form action="/get">
      Hodiny: <input type="number" name="act-h">
      <br>
      Minuty: <input type="number" name="act-m">
      <br>
      <br>
      <input type="submit" value="Submit">
    </form><br>    

    <h3>Doba otevíráni/zavírání</h3>
    <form action="/get">
      Vteřin: <input type="number" name="move">
      <br>
      <br>
      <input type="submit" value="Submit">
    </form>
  </body></html>
)rawliteral";



int stoi(String s)
{
    
    return s.toInt();
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void serverRun()
{
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(OpenH)) {     
      dvere.setHourOpen(stoi(request->getParam(OpenH)->value()));
      dvere.setMinuteOpen(stoi(request->getParam(OpenM)->value()));
      Serial.println(OpenH);
      Serial.println(dvere.getTimeOpen());
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(CloseH)) {
      dvere.setHourClose(stoi(request->getParam(CloseH)->value()));
      dvere.setMinuteClose(stoi(request->getParam(CloseM)->value()));
      Serial.println(CloseH);
      Serial.println(dvere.getTimeClose());
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(Move)) {
      dvere.setMove(stoi(request->getParam(Move)->value()));
      Serial.println(Move);
      Serial.println(dvere.getMove());
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "Nastaveni bylo upraveno<br><a href=\"/\">Zpet na Nastaveni</a>");
  });
  server.onNotFound(notFound);
  server.begin();  
}


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  
  EEPROM.begin(EEPROM_SIZE);
  dvere.begin();
  

  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());


  dvere.printDateTime();

  //dvere.setActualTime(20, 30);

  // dvere.setHourOpen(17);
  // dvere.setMinuteOpen(35);
  // dvere.setHourClose(19);
  // dvere.setMinuteClose(51);
  //Serial.println(dvere.ret());


  //dvere.readEEP();


}

void loop() {
 

  //if()
  
  dvere.printDateTime();
  delay(1000);



  if(dvere.timeToOpen())
  {
    dvere.open();
    Serial.println("Open function");
  }

  if(dvere.timeToClose())
  {
    dvere.close();
    Serial.println("Close function");
  }

   serverRun();





}


