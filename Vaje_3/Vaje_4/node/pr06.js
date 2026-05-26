// Povezava_streznik_klient_websocket_gumb_in_vnosno_polje_za_posiljanje_na_streznik

import http from "http";
import express from "express";
import ip from "ip";
import path from "path"; // knjižnica za delo s potmi do datotek
import { fileURLToPath} from "url"; // knjižnica za pretvorbo URL v pot do datoteke
import { join, dirname} from "path"; // knjižnica za združevanje poti do datotek


const _dirname = path.dirname(fileURLToPath(import.meta.url));

import WebSocket, {WebSocketServer} from "ws"; // za permanentno dvosmerno povezavo med strežnikom in klientom

// Zaženemo strežnik
// Gremo v brskalnik na naslov
// Se povežemo z dvosmerno povezavo preko vrat 8888
// Napišemo sporočilo in s klikom na gum 'Pošlji' se sporočilo pošlje strežniku
// (brez vmesnika to storimo preko konzole F12 - developer tools z ukazom - ws.send("Pozdrav strežnik") )
// za to potrebujemo ukaz ws.on("message")
// To sporočilo se izpiše v terminalu

const app = express(); // objekt aplikacije Express

const hostname = "192.168.1.219";
const port = 80; // standardni port za http strežnike (https pa ima port 443)

const server = http.createServer(app).listen(80); // express strežnik posluša na portu 80

const wss1 = new WebSocketServer({port: 8888}); // ko se bomo povezali z brskalnikom na strežnik, bo WebSocket povezava med brskalnikom in node.js strežnikom vzpostavljena preko porta 8888

app.get("/", (req, res) => {
    res.sendFile(join(_dirname, "pr06.html")); // pošlje datoteko pr06.html kot odgovor na zahtevo ("request - response")
});

// *********************************************************************
// Brskalnik na wss1 komunicira z node.js preko porta 8888
wss1.on("connection", function(ws, req){
    console.log("Klient - brskalnik se je povezal z dvosmerno povezavo prek vrat (porta) 8888.");

    ws.on("message", function(msgString){
        console.log("Sporočilo iz brskalnika prejeto, vsebina sporočila: " + msgString);
    }); // konec ws.on("message"...)
});
// konec wss1.on("connection"...)

console.log("Strežnik zagnan.");
console.log("IP=" + ip.address());
