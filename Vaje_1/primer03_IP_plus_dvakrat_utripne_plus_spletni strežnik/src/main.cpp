#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "....";
const char* password = "....";

// ustvarimo objekt razreda WebServer, pot 80 je prednastavljeni port za HTTP strežnik
WebServer server(80); // objekt 'WebServer' razreda  ki bo poskušal na privzetem HTTP portu 80

// tu določimo, kaj se zgodi, če uporabnik vpiše
// korenski ('root') naslov našega strežnika na esp32 modulu (npr. http://192.168.1.223/) - request "/" pomeni korenski naslov
void handle_root() {
  server.send(200, "text/html; charset=UTF-8",  "Pozdravljen svet iz esp32! - Tadeja Jankulov"); // pošljemo HTTP odgovor s statusno kodo 200 (OK), vsebino tipa "text/html" in besedilom "Pozdravljen svet iz esp32! - Tadeja Jankulov"
  // strežnik klientu (Chrome ali Firefox ...) pošlje kot odziv ("response")
  // string "Pozdravljen svet ..."
  // Koda 200 predstavlja standardni odziv na uspešno HTTP zahtevo ("request")
}

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


// tako deluje tudi router
// Z zelo malo kode lahko realiziramo spletni strežnik
// v nastavitvah routerja, gremo na IP routerja, in router nam prikaže spletno stran
// tudi tu smo sedaj to realizirali > tu imamo spletni strežnik, ko bomo vpisali ip in / bo vrnil 'Pozdravljen svet..'
// router pa vrne nastavitve, password itd... (kar se lahko pač nastavlja na routerju)

// če je modul ne enem wi-fiju, mora biti tudi računalnik
// IP je od strežnika

// modul esp32 ima prostor za par sto kilobytov prostora (pol MB, 300 do 500 KB), flasha (pomnilnika) je pa 4MB
// par mega lahko spravimo na naš modul
// lahko pa dodamo SD kartico (s ploščico za prototipiranje) > potem pa je pomnilnika lahko poljubno mnogo -> če ni veliko klientov, bi zadela delovala
// danes imajo računalniki več gigaherčne frekvence
// raspberryPI pa lahko uporabimo kot spletni strežnik, imamo ga doma, poraba energije pa nizka, okoli 5W moči (manj kot klasična žarnica)

// vsi obroči prižgani na kuhalni plošči -> imamo par KWattov moči (enkrat kosilo > bi Raspberry Pi gorel 10 let ... )

// tako kot SD kartico, bi lahko priključili tudi neko tipalo temperature (2 pina za napajanje, 2 za komunikacijo in modul zadevo prebere), lako je tipalo razdalje, vlage (v našem kompletu je fotoupornik)