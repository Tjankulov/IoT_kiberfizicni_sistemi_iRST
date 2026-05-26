#include <Arduino.h>
#include "WiFi.h"
#include "WebSocketsClient.h"

const char* ssid = "A1-39A3F1-EXT";
const char* password = "nazaxa0060";

int portNumber = 8811; // preko vrat 8811 se bo esp32 povezal na node.js strežnik

WebSocketsClient webSocket; // kreiramo objekt razreda WebSocketsClient

uint8_t LED1pin = 2; // 8-bitna celoštevilska spremenljivka


void setup() {


  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 bitov/s
  WiFi.begin(ssid, password); // povežemo se na WiFi omrežje z imenom in geslom

  while (WiFi.status() != WL_CONNECTED) { // dokler se modul ne poveže na WiFi omrežje, ponavljamo zanko
    delay(500); // počakamo 500ms preden preverimo stanje povezave znova
    Serial.println("Povezovanje z WiFi omrežjem ..."); // izpišemo sporočilo na serijski monitor, da vidimo, da se še vedno poskuša povezati
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena."); 
  Serial.print("IP naslov esp32 modula je: "); 
  Serial.println(WiFi.localIP());

  // poženemo WebSocket strežnik in določimo "callback" funkcijo, ki se izvede ob vsakem dogodku na spletnem vtičniku (WebSocket-u)
  webSocket.begin("192.168.1.219", portNumber); // priključimo se na node.js strežnik na IP naslovu 192... port 8811 

  // ta del kode uporabimo kot indikator, da se je del kode prenesel na esp32 module, modra LED dioda bo 2x utripnila
  pinMode(LED1pin, OUTPUT); // pin (nožica) št. 2 bo uporabljen kot digitalni izhod (na nožici 2 je povezana vgrajena LED dioda na ESP32)

  digitalWrite(LED1pin, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 500ms = 0,5 s

  digitalWrite(LED1pin, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(500);

  digitalWrite(LED1pin, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500); 

  digitalWrite(LED1pin, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(500);

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getMode());
}

void loop() {
  // Poslušamo na spletnem vtičniku 8811 (WebSocket-u) za podatke in dogodke
  // tu zapišemo kodo, ki se ponavljajoče izvaja
  webSocket.loop(); // spremlja komunikacijo na webSocket-u
}