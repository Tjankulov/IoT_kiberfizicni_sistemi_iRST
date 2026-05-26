// Povezava_streznik_klient_knjiznici_ip_in_express


import http from "http";
import express from "express"; // knjižnica za izdelavo spletnega strežnika (v tej verziji sicer ni potrebna, Node to naredi že sam)
import ip from "ip"; // dodamo izpis IP naslova
import path from "path"; // knjižnica za delo s potmi do datotek
import { fileURLToPath} from "url"; // knjižnica za pretvorbo URL v pot do datoteke
import { join, dirname} from "path"; // knjižnica za združevanje poti do datotek


// Browser
// Express
// sendFile()
// HTML


const _dirname = path.dirname(fileURLToPath(import.meta.url));

const app = express(); // objekt aplikacije Express

const hostname = "192.168.1.219";
const port = 80; // standardni port za http strežnike (https pa ima port 443)

const server = http.createServer(app).listen(80); // express strežnik posluša na portu 80 (app.listen(80))

app.get("/", (req, res) => {
    res.sendFile(join(_dirname, "pr04.html")); // pošlje datoteko pr04.html kot odgovor na zahtevo ("request - response")
});

console.log("Strežnik zagnan.");
console.log("IP=" + ip.address());
