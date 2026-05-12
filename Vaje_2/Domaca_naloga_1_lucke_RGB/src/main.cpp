#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "...";
const char* password = " ";

WebServer server(80); // objekt 'WebServer' razreda ki bo poslušal na HTTP portu 80

uint8_t LED1pin = 2;   // modra
bool LED1status = LOW;

uint8_t LED2pin = 5;   // rdeča
bool LED2status = LOW;

uint8_t LED3pin = 18;  // zelena
bool LED3status = LOW;

// funkcija, ki sproti sestavi HTML stran (del 'generateHTML' - pomoč chat GPT - da je koda bolj optimizirana, namesto da se vsak status LED posebej vključi v HTML kodo, se statusi LED sestavijo v string in se ta string vključi v HTML kodo)
String generateHTML() {
  String statusModra  = LED1status ? "Modra LED je prižgana." : "Modra LED je ugasnjena.";
  String statusRdeca  = LED2status ? "Rdeča LED je prižgana." : "Rdeča LED je ugasnjena.";
  String statusZelena = LED3status ? "Zelena LED je prižgana." : "Zelena LED je ugasnjena.";

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="sl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>esp32</title>

    <style>
        html {
            font-family: Helvetica;
            text-align: center;
        }

        body {
            margin-top: 50px;
        }

        h1 {
            color: #444444;
            margin: 50px auto 30px;
        }

        h3 {
            color: #444444;
            margin-bottom: 50px;
        }

        .container {
            display: flex;
            justify-content: center;
            gap: 40px;
            margin-top: 20px;
            flex-wrap: wrap;
        }

        .box {
            text-align: center;
        }

        .gumb_vklop {
            background-color: #07e42c;
            display: block;
            width: 110px;
            color: white;
            padding: 13px 30px;
            text-decoration: none;
            font-size: 20px;
            margin: 10px auto;
            border-radius: 5px;
            cursor: pointer;
        }

        .gumb_izklop {
            background-color: #e60b0b;
            display: block;
            width: 110px;
            color: white;
            padding: 13px 30px;
            text-decoration: none;
            font-size: 20px;
            margin: 10px auto;
            border-radius: 5px;
            cursor: pointer;
        }

        .led {
            width: 30px;
            height: 20px;
            border-radius: 50%;
            margin: 0 auto 15px auto;
            border: none;
        }

        .modra {
            background-color: #1e90ff;
        }

        .rdeca {
            background-color: #ff2b2b;
        }

        .zelena {
            background-color: #32cd32;
        }

        .status {
            margin-top: 30px;
            font-size: 20px;
            line-height: 1.8;
        }
    </style>
</head>

<body>

    <h1>Esp32 spletni strežnik</h1>
    <h3>Priklop na WiFi usmerjevalnik</h3>

    <div class="container">

        <div class="box">
          <div class="led modra"></div>
          <a class="gumb_vklop" href="/1">Prižgi LED</a>
          <a class="gumb_izklop" href="/0">Ugasni LED</a>
        </div>

        <div class="box">
          <div class="led rdeca"></div>
          <a class="gumb_vklop" href="/3">Prižgi LED</a>
          <a class="gumb_izklop" href="/2">Ugasni LED</a>
        </div>

        <div class="box">
          <div class="led zelena"></div>
          <a class="gumb_vklop" href="/5">Prižgi LED</a>
          <a class="gumb_izklop" href="/4">Ugasni LED</a>
        </div>

    </div>

    <div class="status">
        <p>)rawliteral";

  html += statusModra;

  html += R"rawliteral(</p>
        <p>)rawliteral";

  html += statusRdeca;

  html += R"rawliteral(</p>
        <p>)rawliteral";

  html += statusZelena;

  html += R"rawliteral(</p>
    </div>

    <p>HTTP strežnik je zagnan.</p>

</body>
</html>
)rawliteral";

  return html;
}

void handle_root() {
  server.send(200, "text/html; charset=UTF-8", generateHTML());
}

void handle_1() {
  LED1status = HIGH;
  server.send(200, "text/html; charset=UTF-8", generateHTML());
}

void handle_0() {
  LED1status = LOW;
  server.send(200, "text/html; charset=UTF-8", generateHTML());
}

void handle_3() {
  LED2status = HIGH;
  server.send(200, "text/html; charset=UTF-8", generateHTML());
}

void handle_2() {
  LED2status = LOW;
  server.send(200, "text/html; charset=UTF-8", generateHTML());
}

void handle_5() {
  LED3status = HIGH;
  server.send(200, "text/html; charset=UTF-8", generateHTML());
}

void handle_4() {
  LED3status = LOW;
  server.send(200, "text/html; charset=UTF-8", generateHTML());
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Povezovanje z WiFi omrežjem ...");
  }

  Serial.println("Povezava z WiFi omrežjem je vzpostavljena.");
  Serial.print("Moj IP naslov je: ");
  Serial.println(WiFi.localIP());

  pinMode(LED1pin, OUTPUT); // modra
  pinMode(LED2pin, OUTPUT); // rdeča
  pinMode(LED3pin, OUTPUT); // zelena

  server.on("/", handle_root);
  server.on("/1", handle_1);
  server.on("/0", handle_0);
  server.on("/3", handle_3);
  server.on("/2", handle_2);
  server.on("/5", handle_5);
  server.on("/4", handle_4);

  server.begin();

  // indikator ob zagonu - modra LED 2x utripne
  digitalWrite(LED1pin, HIGH);
  delay(250);
  digitalWrite(LED1pin, LOW);
  delay(250);
  digitalWrite(LED1pin, HIGH);
  delay(250);
  digitalWrite(LED1pin, LOW);
  delay(250);
}

void loop() {
  server.handleClient();

  digitalWrite(LED1pin, LED1status);  // modra
  digitalWrite(LED2pin, LED2status);  // rdeča
  digitalWrite(LED3pin, LED3status);  // zelena
}