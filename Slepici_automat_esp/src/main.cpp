#include <Arduino.h>
#include "automaticDoor.hpp"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Kurnik";
const char* password = "NaseSlepice";

const char* OpenH = "open-h";
const char* OpenM = "open-m";
const char* CloseH = "close-h";
const char* CloseM = "close-m";
const char* Move = "move";

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
    <h3>Čas otevření kurníku</h3>
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

    <h3>Doba otevíráni/zavírání</h3>
    <form action="/get">
      Vteřin: <input type="number" name="move">
      <br>
      <br>
      <input type="submit" value="Submit">
    </form>
  </body></html>
)rawliteral";

AutomaticDoor dvere(7, 40, 1);
AsyncWebServer server(80);


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
      inputMessage = request->getParam(OpenH)->value();
      inputMessage += request->getParam(OpenM)->value();
      inputParam = OpenH;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(CloseH)) {
      inputMessage = request->getParam(CloseH)->value();
      inputMessage += request->getParam(CloseM)->value();
      inputParam = CloseH;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(Move)) {
      inputMessage = request->getParam(Move)->value();
      inputParam = Move;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();  
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start");

  dvere.begin();

  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

}

void loop() {
  Serial.print("Cas");

  
  dvere.printDateTime();
  delay(500);

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


