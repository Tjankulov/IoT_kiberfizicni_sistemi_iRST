#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h> // dodamo knjižnico WebSockets. Če datoteke nimamo, jo dodamo iz PlatformIO > Libraries > Add to project

const char* ssid = "A1-39A3F1";
const char* password = "nazaxa0060";

WebServer server(80); // objekt 'WebServer' razreda  ki bo poskušal na privzetem HTTP portu 80

// ustvarimo strežnik spletnih vtičnikov na vratih 81 - WebSocketServer, ki zagotavlja permanentno dvosmerno komunikacijo med klientom (npr. brskalnikom Chrome) in strežnikom na esp32 modulu, kar omogoča, da lahko strežnik pošilja podatke klientu tudi brez zahteve ("request") od klienta
// Inicializacija komunikacije spletnega vtičnika, šla pa bo preko porta 81
// webSocket je objet razreda WebSocketsServer, 81 pa je argument v konstruktorju
WebSocketsServer webSocket = WebSocketsServer(81);


uint8_t LED1pin = 2; //8-bitna celoštevilska spremenljivka
bool LED1status = LOW; // logična spremenljivka, ki hrani stanje LED1, LOW pomeni, da je LED ugasnjena, HIGH bi pomenilo, da je LED prižgana

// niz karakterjev
// spletno stran bomo spravili v spremenljivko 'HTML'
// ta koda nam omogoča, da besedilo spravimo v spremenljivo
const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <h2>Primer s spletnim vtičnikom - WebSocket</h2>
    <button onclick='prižgi()'>Prižgi LED</button>
    <button onclick='ugasni()'>Ugasni LED</button>
<script>
    let povezava = new WebSocket('ws://192.168.1.145:81/') // vzpostavitev povezave preko spletnega vtičnka WebSocket
    
    function prižgi() {
        povezava.send("1"); // na strežnik pošljem sporočilo z enko
    }

    function ugasni() {
        povezava.send("0"); // na strežnik pošljem sporočilo z ničlo
    }
</script>
</body>
</html>
)rawliteral"; // spletno stran spravimo v spremenljivko HTML


// ali dobimo poševnico, bomo posredovali klientu samo to osnovno spletno stran
void handle_root() {
  server.send(200, "text/html; charset=UTF-8", HTML); 
}

// *********************************************************************************************************
// Definicija funkcije onWebSocketEvent, ki se izvede kadarkoli dobimo sporočilo preko spletnega vtičnika
// t.j. preko WebSocket-a (oz. se dogodi dogodek - "event" na spletnem vtičniku WebSocket-u)
// *********************************************************************************************************

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // določimo funkcionalnost glede na tip dogodka na spletnem vtičniku (WebSocket-u)

  // num - številka klienta, ki je poslal sporočilo preko WebSocket-a (lahko bi bilo več klientov povezanih, če bi 
  // več ljudi klicalo na telefon 81) ...
  // type - tip dogodka (npr. WStype_TEXT pomeni, da je klient poslal besedilno sporočilo ali klic)_npr. connect,  
  // disconnect
  // payLoad - vsebina sporočila, ki ga je klient poslal preko WebSocket-a (npr. prižgi 1. ali 5. luč)
  // Length - dolžina vsebine sporočila

  switch(type) {
    // če je tip dogodka connected, takrat se je klient (npr. brskalnik Chrome) povezal na spletni vtičnik WebSocket na esp32 modulu
    case WStype_CONNECTED:
    {
      // Dobimo IP naslov klienta, ki se je povezal
      IPAddress ip = webSocket.remoteIP(num);
      // Izpišemo sporočilo na serijski monitor, da vidimo, da se je klient povezal in njegov IP naslov
      Serial.printf("[%u] Klient povezan z IP: %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
    }
    break;

    // če je tip dogodka disconnected, takrat se klient (npr. brskalnik Chrome) odklopil od spletnega vtičnika WebSocket na esp32 modulu
    case WStype_DISCONNECTED: 
    {
      // izpišemo sporočilo na serijski monitor, da vidimo, da se je klient odklopil (in izpišemo št. klienta)
      Serial.printf("[%u] Klient odklopljen!\n", num); 
    }
    break;

    case WStype_TEXT: // če je tip dogodka "WStype_text", tedaj smo preko spletnega vtičnika WebSocket na esp32 modulu prejeli besedilno sporočilo od klienta (npr. brskalnika Chrome)
    {
      Serial.printf("[%u] Prejeto besedilno sporočilo: %s\n", num, payload); // izpišemo sporočilo na serijski monitor, da vidimo, da smo prejeli besedilno sporočilo in kakšna je njegova vsebina
      // namesto n bo število, namesto s bo sporočilo (C++ sintaksa)

      // Prvi znak prejetega sporočila, spravimo v spremenljivko 'besedilo', ki je tipa String
      String besedilo = String((char)payload[0]);

      Serial.println("Prejeti prvi znak sporočila je: " + besedilo);

      if (besedilo == "1") { // če je prejeti prvi znak sporočila "1", tedaj vklopimo LED diodo
        digitalWrite(LED1pin, HIGH); // na pin 2 zapišemo visoko vrednost (LED dioda se prižge)
      }

      else if (besedilo == "0") { 
        digitalWrite(LED1pin, LOW); // na pin 2 zapišemo nizko vrednost (LED dioda se ugasne)
      }
    }

    default:
      break;
  } // KONEC switch stavka
} // KONEC funkcije onWebSocketEvent

// Strežnik moramo še pognati
void setup() {
  // put your setup code here, to run once:
  // tu zapišemo kodo za nastavitve, koda bo izvecena le enkrat, ko se naprava zažene:

  Serial.begin(115200); // nastavimo hitrost serijske komunikacije na 115200 btiov/s
  WiFi.begin(ssid, password); // povežemo se na WiFi omrežje z imenom in geslom

  while (WiFi.status() != WL_CONNECTED) { // dokler se modul ne poveže na WiFi omrežje, ponavljamo zanko
    delay(500); // počakamo 500ms preden preverimo stanje povezave znova
    Serial.println("Povezovanje z WiFi omrežjem ..."); // izpišemo sporočilo na serijski monitor, da vidimo, da se še vedno poskuša povezati
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena."); // izpišemo sporočilo, ko je povezava uspešna
  Serial.print("Moj IP naslov je: "); // spodnji ukaz bo izpisalo v isti vrstici, zato print in ne println
  Serial.println(WiFi.localIP()); // izpišemo IP naslov, ki ga je modul dobil od WiFi omrežja

  // ko se povezava vzpostavi .... naprej ...
  // ko vpišemo IP naslov, na koncu "/", v brskalnik, tedaj strežnik na esp32 modulu dobi zahtevo "request" "/" in izvede se funkcija handle_root(), ki smo jo definirali zgoraj
  // če je poševnica, imamo handle root
  server.on("/", handle_root); 

  // DVA STREŽNIKA SE ZAŽENETA (Navaden spletni in strežnik spletnega vtičnika)
  server.begin(); 
  Serial.println("HTTP strežnik je zagnan.");

  // strežnik na esp32 modulu moramo še pognati, ki posluša na portu 80 in čaka na zahteve ("requests") od klientov (npr. brskalnika Chrome)
  server.begin(); 
  Serial.println("WebSocket strežnik je zagnan.");

  // poženemo WebSocket strežnik in določimo funkcijo onWebSocketEvent, ki se izvede kadarkoli dobimo sporočilo preko spletnega vtičnika WebSocket-a
  webSocket.begin(); // zaženemo spletni strežnik, ki posluša na portu 81
  webSocket.onEvent(onWebSocketEvent); // kadarkoli se bo dogodil določen dogodek preko spletnega vtičnika WebSocket-a, bomo izvedli funkcijo onWebSocketEvent, ki je definirana zgoraj

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

void loop() {
  // put your main code here, to run repeatedly:
  // tu zapišemo kodo, ki se ponavljajoče izvaja, dokler je esp32 vklopljen

  // preverimo, če je prišla kakšna zahteva od klienta (npr. brskalnik Chrome), in če je, jo obdelamo in pošljemo ustrezen odgovor ("response")
  server.handleClient(); 

  // Poslušamo na spletnem vtičniku 81 (WebSocket-u) za podatke in dogodke
  webSocket.loop();
}


// Poleg navadnega spletnega strežnika, smo dodali tudi WebSocket strežnik