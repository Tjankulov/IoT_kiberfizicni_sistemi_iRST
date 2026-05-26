#include <Arduino.h>
#include "WiFi.h"
#include "WebSocketsClient.h"
#include <ArduinoJson.h>

const char* ssid = "A1-39A3F1-EXT";
const char* password = "nazaxa0060";

int portNumber = 8811; // preko vrat 8811 se bo esp32 povezal na node.js strežnik
WebSocketsClient webSocket; // kreiramo objekt razreda WebSocketsClient


// Koda, ki se izvede ob prejemu sporočila preko spletnega vtičnika 8811
// payload - sporočilo, ki ga dobimo s strani strežnika - npr. json
void onWebSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type)
  {
    case WStype_TEXT:
    {
      // v tem delu prenešeni string z JSON sporočilom pretvorimo v JSON objekt
      // <256> - določa kapaciteto objekta doc - StaticJsonDocument - 256 bytov
      StaticJsonDocument<256> doc;
      // deserializacija JSON dokumenta oz. sporočila
      // sporočilo bo shranjeno v spremenljivki z imenom doc
      DeserializationError error = deserializeJson(doc, payload); // string spravimo v spremenljivko doc

      // Ali je razčlenjevanje uspelo?
      if(error){
        Serial.print(F("deserializeJson() - napaka"));
        Serial.println(error.c_str());
        return;
      } 

      // prestrezanje sporočila JSON, ki ga dobimo v spremenljivki payload
      // pretvorimo payload v spremenljivko doc
      // in iz nje lahko izložimo tip sporočila, vrednost in pin
      // glede na to kak tip sporočila dobimo ... (LED prižgemo ali ugasnemo)
      // če je tipa LED,
      // ji določimo vrednost in pin
      // potem pogledamo ali je pin enak 2 
      // če je 0, na pin 2 ničlo
      // če je 1, na pin 2 enko
    
      char tip[40]; // spremenljivka za besedo iz JSON strukture
      memset(tip, 0, 40); // funkcija memset izbriše vse vrednosti iz znakovnega niza tip
      strcpy(tip, doc["tipSporočila"]); // 'string copy' - v spremenljivko tip kopiramo vrednsot pod ključem "tipSporočila" | v našem primeru je tip = "LED"

      if(strcmp(tip, "LED") == 0){ // 'string compare', če je tip sporočila LED ...
        int vrednost = doc["vrednost"]; // ... zapišemo v spremenljivko vrednost iz JSON strukture pod ključem 'vrednost' | v našem primeru je ta enaka 1
        int pin = doc["pin"]; // pin je enak 2
        if(pin == 2){         // če jih imamo več
          if(vrednost == 0){
            digitalWrite(2,0); // na pin 2 zapišemo 0 - na pin 2 je vgrajena modra LED dioda
          }
          else if(vrednost == 1){
            digitalWrite(2, 1); // na pin 2 zapišemo 1
          }
        }
        else // default:
        {
          Serial.println("Neprepoznana vrednost v JSON sporočilu.");
        }
        break;
      } // konec if stavka, če je tip sporočila "LED"
    }
    default:
    break;
  } // konec switch stavka (po C++ standardu, moramo obvezno imeti stavek default: break;)
}; // konec onWebSocketEvent



void setup() {

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 bitov/s
  WiFi.begin(ssid, password); // povežemo se na WiFi omrežje z imenom in geslom

  while (WiFi.status() != WL_CONNECTED) { // dokler se modul ne poveže na WiFi omrežje, ponavljamo zanko
    delay(500); // počakamo 500ms preden preverimo stanje povezave znova
    Serial.println("Povezovanje z WiFi omrežjem ..."); // izpišemo sporočilo na serijski monitor, da vidimo, da se še vedno poskuša povezati
  }

  // IZPIS IP NASLOVA
  Serial.println("Modul esp32 je povezan!");
  Serial.print("IP naslov esp32 modula: ");
  Serial.println(WiFi.localIP());
  Serial.print("Številka port-a: ");
  Serial.println(portNumber);
  Serial.print("WiFi.macAddress: ");
  Serial.println(WiFi.macAddress());


  pinMode(2, OUTPUT); // pin (nožica) št. 2 bo uporabljen kot digitalni izhod (na nožici 2 je povezana vgrajena LED dioda na ESP32)

  // ta del kode uporabimo kot indikator, da se je del kode prenesel na esp32 module, modra LED dioda bo 2x utripnila
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 500ms = 0,5 s
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(500);
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500); 
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(500);

   // poženemo WebSocket strežnik in določimo "callback" funkcijo, ki se izvede ob vsakem dogodku na spletnem vtičniku (WebSocket-u)
  webSocket.begin("192.168.1.219", portNumber); // priključimo se na node.js strežnik na IP naslovu 192... port 8811 
  delay(1000);
  webSocket.onEvent(onWebSocketEvent); // poženemo onWebSocketEvent funkcijo
}

void loop() {
  // Poslušamo na spletnem vtičniku 8811 (WebSocket-u) za podatke in dogodke
  // tu zapišemo kodo, ki se ponavljajoče izvaja
  webSocket.loop(); // spremlja komunikacijo na webSocket-u
}