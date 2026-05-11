#include <Arduino.h>


void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo za nastavitve, koda bo izvedena le enkrat, ko se naprava zažene:
  pinMode(2, OUTPUT); // nožica (pin) 2 bo delovala kot izhod (OUTPUT)
}

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja, dokler je esp32 vklopljen:
  digitalWrite(2, HIGH); // na nožici 2 zapišemo visoko vrednost - LED dioda, ki je na ploščici na nožici 2 se prižge
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, v tem primeru 500ms (0.5s)
  digitalWrite(2, LOW); // na nožici 2 zapišemo nizko vrednost - LED dioda, ki je na ploščici na nožici 2 se ugasne
  delay(500); // zakasnitev izvedbe programa za specificiran čas v milisekundah, v tem primeru 500ms (0.5s)
}
