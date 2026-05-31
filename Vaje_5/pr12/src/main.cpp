#include <Arduino.h>

// na nožico 34 je priključen potenciometer 10kOhm
// ADC pretvornik (Analog to Digital Converter) na esp32 
// vrednosti med 0V in 3.3V pretvorimo v vrednosti med 0 in 4095
// imamo 4096 vrednsot 2^12=4096, 12-bitna naančnost ADC pretvornika
const int nozicaPotenciometra = 34; // številka nožice, ki je povezana s potenciometrom

int vrednostPotenciometra = 0; // spremenljivka, v katero bomo shranili vrednost, ki jo preberemo s potenciometra


void setup() {

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 bitov/s


  // ta del kode uporabimo kot indikator, da se je del kode prenesel na esp32 module, modra LED dioda bo 2x utripnila
  pinMode(2, OUTPUT); // nožica št. 2 bo delovala kot izhod
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, 500ms = 0,5 s
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
  delay(500);
  digitalWrite(2, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
  delay(500); 
  digitalWrite(2, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
}

void loop() {

  // tu zapišemo kodo, ki se ponavljajoče izvaja

  vrednostPotenciometra = analogRead(nozicaPotenciometra); // preberemo vrednost s potenciometrom in jo shranimo
  Serial.println(vrednostPotenciometra); // izpišemo vrednost potenciometra na serijski monitor oz. v terminal

  delay(50); // na 50ms izpišemo vrednost potenciometra
}
