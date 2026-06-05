#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char *ssid = "..."; // tukaj bo hotspot
const char *password = "...";

int portNumber = 8811;      // preko vrat 8811 se bo esp32 povezal na node.js strežnik
WebSocketsClient webSocket; // ustvarimo objekt za delo s spletnim vtičnikom

String dataString; // za pošiljanje json sporočil

// na nožico 32 je priključen fotoupornik s 5,6 kOhm
// ADC pretvornik (Analog to Digital Converter) na esp32
// vrednosti med 0V in 3.3V pretvorimo v vrednosti med 0 in 4095
// imamo 4096 vrednosti, ker je esp32 12-biten: 2^12=4096, 12-bitna natančnost ADC pretvornika (Analog to Digital Converter) - uporaba s funkcijo analogRead()
const int nozicaFotoupornika = 32; // številka nožice, ki je povezana s fotoupornikom

const int glavnaLedDioda = 33;  // rumena LED
const int pomoznaLedDioda = 26; // rdeča LED
const int modraLedDioda = 2;    // modra LED na esp32

const int pragSvetlobe = 2000; // če bo vrednost fotoupornika večja od 2000, sklepam, da dioda sveti

int vrednostFotoupornika = 0; // spremenljivka, v katero bomo shranili vrednost, ki jo preberemo s fotoupornika
int stanjeGlavneLed = 0;
int stanjeAlarma = 0;

unsigned long zadnjiCasAlarma = 0;  
bool stanjeModreLed = false;  

// Koda, ki se izvede ob prejemu sporočila preko spletnega vtičnika 8811
// payload - sporočilo, ki ga dobimo s strani strežnika - npr. json
void onWebSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
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
    if (error)
    {
      Serial.print(F("deserializeJSON() - napaka"));
      Serial.println(error.c_str());
      return;
    }

    // prestrezanje sporočila JSON, ki ga dobimo v spremenljivki 'payload'
    // pretvorimo payload v spremenljivko 'doc'
    // in iz nje lahko izluščimo tip sporočila, vrednost in pin
    // glede na to kak tip sporočila dobimo ... (LED prižgemo ali ugasnemo)
    // če je tipa LED, ji določimo vrednost in pin
    // potem pogledamo ali je pin enak 33
    // če je 0, na pin 33 ničlo
    // če je 1, na pin 33 enko
    char tip[40];                     // spremenljivka za besedo iz JSON strukture
    memset(tip, 0, 40);               // funkcija memset izbriše vse vrednosti iz znakovnega niza tip
    strcpy(tip, doc["tipSporočila"]); // 'string copy' - v spremenljivko tip kopiramo vrednost pod ključem 'tipSporočila' | v našem primeru je tip enak "LED"

    if (strcmp(tip, "LED") == 0)
    {
      // 'string compare', 'Če je tip sporočila LED ...'
      int vrednost = doc["vrednost"]; // ... zapišemo v spremenljivko vrednost iz JSON strukture pod ključem 'vrednost' | v našem primeru je ta enaka 1
      int pin = doc["pin"];

      if (pin == glavnaLedDioda)
      {
        if (vrednost == 0)
        {
          digitalWrite(glavnaLedDioda, 0); // na pin 33 zapišemo 0 - na pin 33 je vgrajena glavna (rumena) LED dioda
          stanjeGlavneLed = 0;

          dataString = R"({"tipSporočila":"LED","pin":33,"vrednost":)";
          dataString = dataString + String(stanjeGlavneLed);
          dataString = dataString + "}";

          webSocket.sendTXT(dataString);
        }
        else if (vrednost == 1)
        {
          digitalWrite(glavnaLedDioda, 1); // na pin 33 zapišemo 1 - na pin 33 je vgrajena glavna (rumena) LED dioda
          stanjeGlavneLed = 1;

          dataString = R"({"tipSporočila":"LED","pin":33,"vrednost":)";
          dataString = dataString + String(stanjeGlavneLed);
          dataString = dataString + "}";

          webSocket.sendTXT(dataString);
        }
      }
    }
    break;
  } // konec if stavka, če je tip sporočila "LED"
  default:
    break;
  } // konec switch stavka (po C++ standardu moramo obvezno imeti stavek default: break;)
} // konec onWebSocketEvent

void setup()
{
  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 bitov/s

  pinMode(glavnaLedDioda, OUTPUT);
  pinMode(pomoznaLedDioda, OUTPUT);

  digitalWrite(glavnaLedDioda, LOW);
  digitalWrite(pomoznaLedDioda, LOW);

  // ta del kode uporabimo kot indikator, da se je del kode prenesel na esp32 modul -> modra LED dioda bo 2x utripnila
  pinMode(modraLedDioda, OUTPUT);    // nožica št. 2 bo delovala kot izhod
  digitalWrite(modraLedDioda, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500);                        // zakasnitev izvedbe programa za specificiran čas v milisekundah, 500ms = 0,5 s
  digitalWrite(modraLedDioda, LOW);  // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(500);
  digitalWrite(modraLedDioda, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500);
  digitalWrite(modraLedDioda, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)

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

  // priključimo se na node.js strežnik na IP naslovu 10.33. ...
  webSocket.begin("10.33.130.244", portNumber);

  // ob prejemu sporočila, se izvede funkcija 'onWebSocketEvent'
  webSocket.onEvent(onWebSocketEvent);
}

// tu zapišemo kodo, ki se ponavljajoče izvaja
void loop()
{
  // vzdržujemo povezavo s strežnikom, preverjamo, če so npr. prišla sporočila, itd.
  webSocket.loop();

  // preberemo vrednost s fotoupornika in jo shranimo
  vrednostFotoupornika = analogRead(nozicaFotoupornika);
  // izpišemo vrednost fotoupornika na serijski monitor oz. v terminal
  Serial.println(vrednostFotoupornika);

  // POVRATNA ZANKA
  if (stanjeGlavneLed == 1) // Če je glavna LED (rumena) prižgana
  {
    if (vrednostFotoupornika < pragSvetlobe) // in če je vrednost fotoupornika pod pragom svetlobe (glavna dioda pokvarjena, odstranjena ... ) - "odpoved" glavne LED diode
    {
      stanjeAlarma = 1; // sproži alarm
      
      digitalWrite(pomoznaLedDioda, HIGH);

      unsigned long trenutniCas = millis(); // funkcija 'millis' vrne čas v milisekundah, ki je pretekel od zagona esp32
      if (trenutniCas - zadnjiCasAlarma >= 100)  // vsake 100ms
      {
        zadnjiCasAlarma = trenutniCas;
        stanjeModreLed = !stanjeModreLed;               // toggle
        digitalWrite(modraLedDioda, stanjeModreLed);    // prižgi/ugasni
      }
    }
    else
    {
      stanjeAlarma = 0;

      digitalWrite(pomoznaLedDioda, LOW);
      digitalWrite(modraLedDioda, LOW);
    }
  }
  else // glavna LED dioda je ugasnjena, alarm ni potreben
  {
    stanjeAlarma = 0;

    digitalWrite(pomoznaLedDioda, LOW);
    digitalWrite(modraLedDioda, LOW);
  }

  // R"() nam omogoča enostaven vnos niza znakov "literal string" brez potrebe po uporabi escape znakov, npr. za narekovaje - JSON sporočilo za strežnik
  dataString = R"({"tipSporočila": "fotoupornik", "pin": "32", "vrednost":)";
  dataString = dataString + String(vrednostFotoupornika);
  dataString = dataString + R"(, "alarm":)";
  dataString = dataString + String(stanjeAlarma);
  dataString = dataString + "}";

  // preko spletnega vtičnika pošljemo vrednost fotoupornika v json obliki na strežnik
  webSocket.sendTXT(dataString);

  Serial.println(dataString);

  delay(100); // na 100ms izpišemo vrednost fotoupornika
}
