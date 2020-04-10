#include <Arduino.h>

//  String to int
int stoi(String s)
{   
    return s.toInt();
}

const char page_intro[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html><head>
    <title>ESP Input Form</title>
    <meta http-equiv="pragma" content="no-cache" />
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    </head><body>
    <h1>Já jsem chytrý kurník</h1>
    <p>Naše slepice mají webové stránky :-)</p>
    <h4>By Kuba Andrysek<h4>
    <h2>Nastavení</h2>    
)rawliteral";


const char page_open[] PROGMEM = R"rawliteral(
    <form action="/get">
      Hodiny: <input type="number" name="open-h" required>
      <br>
      Minuty: <input type="number" name="open-m" required>
      <br>
      <br>
      <input type="submit" value="Nastavit">
    </form><br>
)rawliteral";


const char page_close[] PROGMEM = R"rawliteral(
    <form action="/get">
      Hodiny: <input type="number" name="close-h" required>
      <br>
      Minuty: <input type="number" name="close-m" required>
      <br>
      <br>
      <input type="submit" value="Nastavit">
    </form><br>
)rawliteral";


const char page_time[] PROGMEM = R"rawliteral(
    <form action="/get">
      Hodiny: <input type="number" name="act-h" required>
      <br>
      Minuty: <input type="number" name="act-m" required>
      <br>
      <br>
      <input type="submit" value="Nastavit">
    </form><br> 
)rawliteral";

const char page_end[] PROGMEM = R"rawliteral(
    </body></html>
)rawliteral";

const char page_[] PROGMEM = R"rawliteral(

)rawliteral";

/*
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
      Hodiny: <input type="number" name="open-h" required>
      <br>
      Minuty: <input type="number" name="open-m" required>
      <br>
      <br>
      <input type="submit" value="Submit">
    </form><br>

    <h3>Čas zavření kurníku</h3>
    <form action="/get">
      Hodiny: <input type="number" name="close-h" required>
      <br>
      Minuty: <input type="number" name="close-m" required>
      <br>
      <br>
      <input type="submit" value="Submit">
    </form><br>

    <h3>Aktuální čas:</h3>
    <form action="/get">
      Hodiny: <input type="number" name="act-h" required>
      <br>
      Minuty: <input type="number" name="act-m" required>
      <br>
      <br>
      <input type="submit" value="Submit">
    </form><br>    

    <h3>Doba otevíráni/zavírání</h3>
    <form action="/get">
      Vteřin: <input type="number" name="move" required>
      <br>
      <br>
      <input type="submit" value="Submit">
    </form>
  </body></html>
)rawliteral";
*/