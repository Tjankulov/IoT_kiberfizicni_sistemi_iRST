#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiAP.h>

const char* ssid = "esp32_1";
const char* password = "12345678";

// To spodaj je 'Hello world, za IOT :) 

// ustvarimo objekt razreda WebServer, pot 80 je prednastavljeni port za HTTP strežnik
WebServer server(80); // objekt 'WebServer' razreda  ki bo poskušal na privzetem HTTP portu 80

uint8_t LED1pin = 2; //8-bitna celoštevilska spremenljivka
bool LED1status = LOW; // logična spremenljivka, ki hrani stanje LED1, LOW pomeni, da je LED ugasnjena, HIGH bi pomenilo, da je LED prižgana

// zakaj bool ni true in false? > HIGH in LOW pride z Arduino knjižnico


// niz karakterjev
// spletno stran bomo spravili v spremenljivko 'HTML'
// ta koda nam omogoča, da besedilo spravimo v spremenljivo
const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <style>
        html {font-family: Helvetica; text-align:center;}
        body {margin-top: 50px;}
        h1 {color: #444444; margin: 50px auto 30px;}
        h3 {color: #444444; margin-bottom: 50px}
        .gumb {background-color: #3498db; display:block; width: 200px; color: white; padding: 13px 30px; text-decoration: none; font-size: 25px; margin: 0px auto 35px; border-radius: 5px; cursor: pointer; border: none;} 
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Esp32 spletni strežnik</h1>
    <h3>Priklop na WiFi usmerjevalnik</h3>
    <a class='gumb' href="/1"> Prižgi LED</a>
    <a class='gumb' href = "/0"> Ugasni LED</a>
    <p> HTTP strežnik je zagnan.</p>
</body>
</html>
)rawliteral";

const char HTML1[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <style>
        html {font-family: Helvetica; text-align:center;}
        body {margin-top: 50px;}
        h1 {color: #444444; margin: 50px auto 30px;}
        h3 {color: #444444; margin-bottom: 50px}
        .gumb {background-color: #3498db; display:block; width: 200px; color: white; padding: 13px 30px; text-decoration: none; font-size: 25px; margin: 0px auto 35px; border-radius: 5px; cursor: pointer; border: none;} 
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Esp32 spletni strežnik</h1>
    <h3>Priklop na WiFi usmerjevalnik</h3>
    <a class='gumb' href="/1"> Prižgi LED</a>
    <a class='gumb' href = "/0"> Ugasni LED</a>
    <p> Prejet ukaz za vklop LED diode.</p>
</body>
</html>
)rawliteral";

const char HTML0[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <style>
        html {font-family: Helvetica; text-align:center;}
        body {margin-top: 50px;}
        h1 {color: #444444; margin: 50px auto 30px;}
        h3 {color: #444444; margin-bottom: 50px}
        .gumb {background-color: #3498db; display:block; width: 200px; color: white; padding: 13px 30px; text-decoration: none; font-size: 25px; margin: 0px auto 35px; border-radius: 5px; cursor: pointer; border: none;} 
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Esp32 spletni strežnik</h1>
    <h3>Priklop na WiFi usmerjevalnik</h3>
    <a class='gumb' href="/1"> Prižgi LED</a>
    <a class='gumb' href = "/0"> Ugasni LED</a>
    <p> Prejet ukaz za izklop LED diode.</p>
</body>
</html>
)rawliteral";


void handle_root() {
  server.send(200, "text/html; charset=UTF-8", HTML); 
}

// tu določimo, kaj se zgodi, če uporabnik vpiše
// korenski ("root") naslov našega strežnika na esp32 modulu, (npr. http://..) - request "/1"
void handle_1() {
  LED1status = HIGH; // spremenljivki LED1status dodelimo vrednost HIGH, kar pomeni, da je LED prižgana, v zanki loop() bomo potem preverjali stanje LED1status in glede na to prižigali ali ugašali LED diodo
  server.send(200, "text/html; charset=UTF-8", HTML1); 
}

// tu določimo, kaj se zgodi, če uporabnik vpiše
// korenski ("root") naslov našega strežnika na esp32 modulu, (npr. http://..) - request "/0"
void handle_0() {
  LED1status = LOW; // spremenljivki LED1status dodelimo vrednost LOW, kar pomeni, da je LED ugasnjena, v zanki loop() bomo potem preverjali stanje LED1status in glede na to prižigali ali ugašali LED diodo
  server.send(200, "text/html; charset=UTF-8", HTML0); 
}

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo za nastavitve, koda bo izvecena le enkrat, ko se naprava zažene:

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 btiov/s

  // vzpostavitev programske dostopne točke - "Soft Access Point AP"
  // Tokrat modul deluje kot router
  WiFi.softAP(ssid, password); // ustvarimo WiFi dostopno točko z imenom "esp32_1" in geslom "12345678"
  

  // 'ne povezujemo se na router, ker smo sami router' 

  Serial.println("Vzpostavitev esp32 AP - Access Point."); // izpišemo sporočilo, ko je povezava uspešna
  Serial.print("IP naslov esp32 dostopne točke: "); // spodnji ukaz bo izpisalo v isti vrstici, zato print in ne println
  Serial.println(WiFi.softAPIP()); // izpišemo IP naslov, v modusu AP je prednastavljeni 192.168.4.1



  server.on("/", handle_root); 
  server.on("/1", handle_1); 
  server.on("/0", handle_0); 


  // strežnik na esp32 modulu moramo še pognati, ki posluša na portu 80 in čaka na zahteve ("requests") od klientov (npr. brskalnika Chrome)
  server.begin(); 


  // ta del kode uporabimo kot indikator, da se je program prenesel na esp32 module, modra LED dioda bo 2x utripnila
  pinMode(2, OUTPUT); // pin (nožica) št. 2 bo uporabljen kot digitalni izhod (na nožici 2 je povezana vgrajena LED dioda na ESP32)
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(250); // počakamo 250 ms
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(250); // počakamo 250 ms
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(250); // počakamo 250 ms
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(250); // počakamo 250 ms
}


// 'Loop' se nekje z 10kHZ vrti in gleda ali je status Low ali High in glede na to tudi zapiše na pin
// Torej, s frekvenco 10.000-krat na sekundo preverja kakšen je status
void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja, dokler je esp32 vklopljen

  // preverimo, če je prišla kakšna zahteva od klienta (npr. brskalnik Chrome), in če je, jo obdelamo in pošljemo ustrezen odgovor ("response")
  server.handleClient(); 

  if(LED1status == HIGH){ // če je spremenljivka LED1status enaka HIGH, potem ...
    digitalWrite(LED1pin, HIGH); // ... prižgemo LED diodo, ki je povezana na pin 2
  }
  else{ // sicer, če LED1status ni HIGH, potem ...
    digitalWrite(LED1pin, LOW); // ... ugasnemo LED diodo, ki je povezana na pin 2
  }
}