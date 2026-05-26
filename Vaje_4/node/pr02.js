// Povezava_streznik_klient_knjiznica_fs_za_delo_z_datotekami

import http from "http";
import fs from "fs"; // knjižnica za delo z datotekami

// Klient (brskalnik)
// pošlje request
// Node server
// fs.readFile ("pr02.html")
// vrne HTML
// Klient pokaže stran

const hostname = "192.168.1.219";
const port = 3000;

const server = http.createServer((req, res) => {

    fs.readFile("pr02.html", (err, data) => {

        if (err) {
            res.statusCode = 500;
            res.end("Napaka pri branju datoteke.");
            return;
        }

        res.setHeader("Content-Type", "text/html");
        res.end(data);

    });

});

server.listen(port, hostname, () => {
    console.log(`Strežnik teče na http://${hostname}:${port}`);
});