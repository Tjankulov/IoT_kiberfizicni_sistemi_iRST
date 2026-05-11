#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiAP.h>

const char* ssid = "esp32_1"; // ime esp32 modula, kot WiFi dostopne točke (SSID - Service Set Identifier)
const char* password = "12345678";

// ustvarimo objekt razreda WebServer, pot 80 je prednastavljeni port za HTTP strežnik
WebServer server(80); // objekt 'WebServer' razreda  ki bo poskušal na privzetem HTTP portu 80

// niz karakterjev
// spletno stran bomo spravili v spremenljivko 'HTML'
// ta koda nam omogoča, da besedilo spravimo v spremenljivo
const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>
</head>
<body>
    <h1>Esp32 kot WiFi dostopna točka - Access Point AP!</h1>
</body>
</html>
)rawliteral";


void handle_root() {
  server.send(200, "text/html; charset=UTF-8", HTML); 
}

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo za nastavitve, koda bo izvecena le enkrat, ko se naprava zažene:

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 btiov/s

  // Vzpostavitev programske dostopne točke - "Access Point (AP)"
  // "Software enabled access point", npr. računalnik, ki ni bil zasnovan kot WiFi dostopna točka, lahko deluje kot WiFi dostopna točka, če se vklopi funkcija "SoftAP" (Software enabled access point)
  // esp32 se spremeni v dostopno točko
  WiFi.softAP(ssid, password); // funkcija softAP() vzpostavi WiFi dostopno točko z imenom "esp32_1" in geslom "12345678" 

  // izpišemo IP naslov
  Serial.println("Vzpostavljanje WiFi dostopne točke na esp32 modulu..."); // izpišemo sporočilo na serijski monitor, da vidimo, da se poskuša vzpostaviti WiFi dostopna točka
  Serial.print("IP naslov esp32 dostopne točke: ");
  Serial.println(WiFi.softAPIP()); // izpišemo IP naslov, ki ga je modul dobil kot dostopno točko 


  // ko se povezava vzpostavi .... naprej ...
  // ko vpišemo IP naslov, na koncu "/", v brskalnik, tedaj strežnik na esp32 modulu dobi zahtevo "request" "/" in zvece se funkcija handle_root(), ki smo jo definirali zgoraj
  server.on("/", handle_root); 

  // strežnik na esp32 modulu moramo še pognati, ki posluša na portu 80 in čaka na zahteve ("requests") od klientov (npr. brskalnika Chrome)
  server.begin(); 


  // ta del kode uporabimo kot indikator, da se je program prenesel na esp32 module, modra LED dioda bo 2x utripnila
  pinMode(2, OUTPUT); // pin (nožica) št. 2 bo uporabljen kot digitalni izhod (na nožici 2 je povezana vgrajena LED dioda na ESP32)
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(250); // počakamo 250 ms
  digitalWrite(2, LOW); // na pin 2 zapišemo visoko vrednost (LED dioda se ugasne)
  delay(250); // počakamo 250 ms
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(250); // počakamo 250 ms
  digitalWrite(2, LOW); // na pin 2 zapišemo visoko vrednost (LED dioda se ugasne)
  delay(250); // počakamo 250 ms
}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja, dokler je esp32 vklopljen

  // preverimo, če je prišla kakšna zahteva od klienta (npr. brskalnik Chrome), in če je, jo obdelamo in pošljemo ustrezen odgovor ("response")
  server.handleClient(); 
}