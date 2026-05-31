#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char *ssid = "A1-39A3F1-EXT";
const char *password = "nazaxa0060";

int portNumber = 8811;      // preko vrat 8811 se bo esp32 povezal na node.js strežnik
WebSocketsClient webSocket; // ustvarimo objekt za delo s spletnim vtičnikom

String dataString; // za pošiljanje json sporočil

// na nožico 34 je priključen potenciometer 10kOhm
// ADC pretvornik (Analog to Digital Converter) na esp32
// vrednosti med 0V in 3.3V pretvorimo v vrednosti med 0 in 4095
// imamo 4096 vrednsot 2^12=4096, 12-bitna naančnost ADC pretvornika
const int nozicaPotenciometra = 34; // številka nožice, ki je povezana s potenciometrom

int vrednostPotenciometra = 0; // spremenljivka, v katero bomo shranili vrednost, ki jo preberemo s potenciometra

void setup()
{

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 bitov/s

  WiFi.begin(ssid, password); // povežemo se na WiFi omrežje z imenom in geslom

  while (WiFi.status() != WL_CONNECTED)
  {                                                    // dokler se modul ne poveže na WiFi omrežje, ponavljamo zanko
    delay(500);                                        // počakamo 500ms preden preverimo stanje povezave znova
    Serial.println("Povezovanje z WiFi omrežjem ..."); // izpišemo sporočilo na serijski monitor, da vidimo, da se še vedno poskuša povezati
  }

  // IZPIS IP NASLOVA
  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("IP naslov esp32 modula: ");
  Serial.println(WiFi.localIP());

  // ta del kode uporabimo kot indikator, da se je del kode prenesel na esp32 module, modra LED dioda bo 2x utripnila
  pinMode(2, OUTPUT);    // nožica št. 2 bo delovala kot izhod
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500);            // zakasnitev izvedbe programa za specificiran čas v milisekundah, 500ms = 0,5 s
  digitalWrite(2, LOW);  // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(500);
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500);
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)

  webSocket.begin("192.168.1.220", 8811); // priključimo se na node.js strežnik na IP naslovu 192 ...
}

void loop()
{

  // tu zapišemo kodo, ki se ponavljajoče izvaja

  webSocket.loop(); // vzdržujemo povezavo s strežnikom, preverjamo, če so npr. prišla sporočila, itd.

  vrednostPotenciometra = analogRead(nozicaPotenciometra); // preberemo vrednost s potenciometrom in jo shranimo
  Serial.println(vrednostPotenciometra);                   // izpišemo vrednost potenciometra na serijski monitor oz. v terminal

  dataString = R"({"tipSporočila": "potenciometer", "pin": "34", "vrednost":)"; // R"() nam omogoča enostaven vnos niza znakov "literal string" brez potrebe po uporabi escape znakov, npr. za narekovaje
  dataString = dataString + String(vrednostPotenciometra) + "}";

  webSocket.sendTXT(dataString); // preko spletnega vtičnika pošljemo vrednost potenciometra v json obliki na strežnik

  Serial.println(dataString);

  delay(50); // na 50ms izpišemo vrednost potenciometra
}