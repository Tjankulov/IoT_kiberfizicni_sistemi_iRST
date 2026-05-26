// Povezava_streznik_klient_websocket_dvosmerna_povezava

import http from "http";
import express from "express";
import ip from "ip";
import path from "path"; // knjižnica za delo s potmi do datotek
import { fileURLToPath} from "url"; // knjižnica za pretvorbo URL v pot do datoteke
import { join, dirname} from "path"; // knjižnica za združevanje poti do datotek


const _dirname = path.dirname(fileURLToPath(import.meta.url));

import WebSocket, {WebSocketServer} from "ws"; // za permanentno dvosmerno povezavo med strežnikom in klientom


// ne moremo neposredno komunicirati preko vtičnika, zato moramo vpisati port 80, ki je prednastavljeni port za spletne strani

// v brskalnik napišemo naslov
// request gre na port 80
// express vrne HTML datoteko
// brskalnik prebere HTML
// JS v HTML-ju odpre WebSocket z:8888
// node zazna povezavo
// izpiše, da se je brskalnik povezal

// Preko enih vrat se lahko poveže več klientov (npr. na Google Chrome, vsi dostopamo preko vrat 80)

// strežnik lahko pošilja podatke klientu brez nove zahteve
// klient tudi pošilja podatke strežniku dokler je povezava odprta 
// uporabno za chat, senzorje, esp32, igre ...

const app = express(); // objekt aplikacije Express

const hostname = "192.168.1.219";
const port = 80; // standardni port za http strežnike (https pa ima port 443)

const server = http.createServer(app).listen(80); // express strežnik posluša na portu 80

const wss1 = new WebSocketServer({port: 8888}); // ko se bomo povezali z brskalnikom na strežnik, bo WebSocket povezava med brskalnikom in node.js strežnikom vzpostavljena preko porta 8888

app.get("/", (req, res) => {
    res.sendFile(join(_dirname, "pr05.html")); // pošlje datoteko pr05.html kot odgovor na zahtevo ("request - response")
});

// *********************************************************************
// Brskalnik na wss1 komunicira z node.js preko porta 8888
wss1.on("connection", function(ws, req){
    console.log("Klient - brskalnik se je povezal z dvosmerno povezavo prek vrat (porta) 8888.");
});
// konec wss1.on("connection"...)

console.log("Strežnik zagnan.");
console.log("IP=" + ip.address());
