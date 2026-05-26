// Povezava_streznik_klient_websocket_gumb_in_vnosno_polje_za_posiljanje_na_streznik
// Tokrat pošiljanje JSON strukture

// pripravljeno za delo z esp32 modulom

import http from "http";
import express from "express";
import ip from "ip";
import path from "path"; // knjižnica za delo s potmi do datotek
import { fileURLToPath} from "url"; // knjižnica za pretvorbo URL v pot do datoteke
import { join, dirname} from "path"; // knjižnica za združevanje poti do datotek


const _dirname = path.dirname(fileURLToPath(import.meta.url));

import WebSocket, {WebSocketServer} from "ws"; // za permanentno dvosmerno povezavo med strežnikom in klientom

const app = express(); // objekt aplikacije Express

const hostname = "192.168.1.219";
const port = 80; // standardni port za http strežnike (https pa ima port 443)

const server = http.createServer(app).listen(80); // express strežnik posluša na portu 80

const wss1 = new WebSocketServer({port: 8888}); // ko se bomo povezali z brskalnikom na strežnik, bo WebSocket povezava med brskalnikom in node.js strežnikom vzpostavljena preko porta 8888

app.get("/", (req, res) => {
    res.sendFile(join(_dirname, "pr07.html")); // pošlje datoteko pr07.html kot odgovor na zahtevo ("request - response")
});

// *********************************************************************
// Brskalnik na wss1 komunicira z node.js preko porta 8888
wss1.on("connection", function(ws, req){
    console.log("Klient - brskalnik se je povezal z dvosmerno povezavo prek vrat (porta) 8888.");

    ws.on("message", function(msgString){
        console.log("Sporočilo iz brskalnika prejeto, vsebina sporočila: " + msgString);

        // izluščimo dele json sporočila in jih izpišemo
        let msg = JSON.parse(msgString); // msg je sedaj JSON objekt, s funkcijo parse razčlenimo string, ki smo ga poslali s HTML strani klienta
        console.log("Tip sporočila: " + msg.tipSporočila);
        console.log("Pin: " + msg.pin);
        console.log("Vrednost: " + msg.vrednost);
    }); // konec ws.on("message"...)
});
// konec wss1.on("connection"...)

console.log("Strežnik zagnan.");
console.log("IP=" + ip.address());
