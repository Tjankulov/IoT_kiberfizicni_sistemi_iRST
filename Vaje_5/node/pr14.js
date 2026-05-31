// Povezava_klient_strežnik_modul esp32_potenciometer

import * as http from 'http';
import express from 'express';
import ip from 'ip';
import path from 'path';
import { join, dirname } from 'path';
import { fileURLToPath } from 'url';
const __dirname = dirname(fileURLToPath(import.meta.url));

import WebSocket, { WebSocketServer } from 'ws'; // za permanentno povezavo med strežnikom in klientom

const app = express(); // deklariramo app kot aplikacijo

const wss1 = new WebSocketServer({port: 8888}); // port 8888 bo ves čas odprt, preko porta poteka dvosmerna povezava; povezava med Chrome - brskalnikom in node.js strežnikom
const wss2 = new WebSocketServer({port: 8811}); // esp32 modul se bo preko vtičnika 8811 povezal z node.js strežnikom, preko tega porta bo potekala dvosmerna povezava med esp32 modulom in node.js strežnikom

const server = http.createServer(app).listen(80);

app.get('/', function(req, res){
    res.sendFile(path.join(__dirname + '/pr14.html'));
});

// funkcija za posredovanje sporočil vsem aktivnim klientom na vtičniku wss1 - port 8888 - Chrome
wss1.broadcast = function broadcast(data){ // broadcast na Chrome kliente
    wss1.clients.forEach(function each(client){ // za vsakega klienta, ki je povezan na wss1, izvedi funkcijo
        if(client.readyState === WebSocket.OPEN){
            client.send(data); // pošlji sporočilo vsem klientom, ki so povezani na wss1, t.j. vsem Chrome klientom
        }
    }); 
};

// funkcija za posredovanje sporočil vsem aktivnim klientom na vtičniku wss2 - port 8811
wss2.broadcast = function broadcast(data){ // boradcast za esp32 kliente
    wss2.clients.forEach(function each(client){ // za vsakega klienta, ki je povezan na wss2, izvedi funkcijo
        if(client.readyState === WebSocket.OPEN){
            client.send(data); // pošlji sporočilo vsem klientom, ki so povezani na wss2, t.j. vsem esp32 modulom, ki so povezani na port 8811
        }
    }); 
};

// ***************************************************************
// Brskalnik Chrome (klient) priključen na wss1, vrata 8888
// ***************************************************************
wss1.on('connection', function(ws, req){
    console.log("Klient - Brskalnik se je povezal z dvosmerno povezavo prek vrat 8888.");
    ws.on('message', function(msgString){ // sporočilo dobimo na stran strežnika kot string, t.j. msgString
        console.log("Sporočilo prejeto, vsebina: " + msgString);

        let msg = JSON.parse(msgString); // sporočilo, ki je v obliki JSON stringa, pretvorimo v JSON objekt, msg je sedaj JSON objekt, ki ga lahko obdelujemo kot JavaScript objekt, s funkcijo JSON.parse() pretvorimo JSON string v JSON objekt
        
        switch(msg.tipSporočila){
            case "potenciometer":
                funkcijaPotenciometer(msg); // v primeru, da je tip sporočila iz Chrome klient "LED" poženemo funkcijo funkcijaLED
            break;
        }

    }); // konec ws.on("message...")
}); // konec wss1.on('connection'...)

// ***************************************************************
// esp32 priključen na wss2, vrata 8811
// ***************************************************************
wss2.on('connection', function(ws, req){
    console.log("esp32 se je povezal z dvosmerno povezavo prek vrat 8811.");
    ws.on('message', function(msgString){ // sporočilo dobimo na stran strežnika kot string, t.j. msgString
        console.log("Sporočilo iz esp32 prejeto, vsebina: " + msgString);

        let msg = JSON.parse(msgString); // sporočilo, ki je v obliki JSON stringa, pretvorimo v JSON objekt, msg je sedaj JSON objekt, ki ga lahko obdelujemo kot JavaScript objekt, s funkcijo JSON.parse() pretvorimo JSON string v JSON objekt
        
        switch(msg.tipSporočila){
            case "potenciometer":
                funkcijaPotenciometer(msg); // v primeru, da je tip sporočila iz Chrome klient "LED" poženemo funkcijo funkcijaLED
            break;
        }

    }); // konec ws.on("message...")
}); // konec wss2.on('connection'...)


console.log('Strežnik je zagnan');
console.log('IP=' + ip.address()); // IP naslov je določen s pomočjo knjižnice

function funkcijaPotenciometer(msg){ // če je tip sporočila "potenciometer"
    wss1.broadcast(JSON.stringify(msg)); // "broad-castamo" po vseh brskalnik klientih - JSON sporočilo posredujemo naprej, tokrat vsem brskalnik klientom   
}

