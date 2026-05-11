#include <Arduino.h>
#include <WiFi.h>


const char* ssid = "A1-39A3F1";
const char* password = "nazaxa0060";

void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo za nastavitve, koda bo izvecena le enkrat, ko se naprava zažene:

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 btiov/s
  WiFi.begin(ssid, password); // povežemo se na WiFi omrežje z imenom "A1-39A3F1"in geslom

  while (WiFi.status() != WL_CONNECTED) { // dokler se modul ne poveže na WiFi omrežje, ponavljamo zanko
    delay(500); // počakamo 500ms preden preverimo stanje povezave znova
    Serial.println("Povezovanje z WiFi omrežjem ..."); // izpišemo sporočilo na serijski monitor, da vidimo, da se še vedno poskuša povezati
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena."); // izpišemo sporočilo, ko je povezava uspešna
  Serial.print("Moj IP naslov je: "); // spodnji ukaz bo izpisalo v isti vrstici, zato print in ne println
  Serial.println(WiFi.localIP()); // izpišemo IP naslov, ki ga je modul dobil od WiFi omrežja


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
}
