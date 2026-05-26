// Povezava_streznik_klient_knjiznica_path


import http from "http";
import fs from "fs"; // knjižnica za delo z datotekami (file system)
import path from "path"; // knjižnica za delo s potmi do datotek
import { fileURLToPath} from "url"; // knjižnica za pretvorbo URL v pot do datoteke

// Klient (brskalnik)
// pošlje request --> preko URL
// Node HTTP server   
// fs.readFile ("pr02.html")
// path najde datoteko       --> omogoča pravilno delo s potmi do datotek ne glede na OS
// red.end()
// HTML nazaj brskalniku


// nadomestek za ____dirname
// pretvori URL -> v normalno pot v operacijskem sistemu
// npr. file://C:/projekti/server/app.js
// v C:\projekti\server\app.js
const _filename = fileURLToPath(import.meta.url);
// iz poti odstrani ime datoteke, vrne smo mapo
// iz _filename = "C:\projekti\server\app.js"
// po path.dirname(_filename)
// dobimo samo C:\projekti\server" - dobimo mapo v kateri je trenutna datoteka
const _dirname = path.dirname(_filename);


const hostname = "192.168.1.219";
const port = 8080;

const server = http.createServer((req, res) => {

    if (req.url === "/"){ // pogledamo, ali je 'request' enak "/"
        fs.readFile(path.join(_dirname, "pr03.html"), (err, data) => {
            if(err){
                res.writeHead(500, {"Content-Typte": "text/plain; charset=utf-8"}); // response write
                res.end("Napaka pri branju datoteke s html stranjo. Branje datoteke pr03.html ni uspelo"); 
            } else {
                res.writeHead(200, {"Content-Typte": "text/html; charset=utf-8"}); // response write
                res.end(data); 
            }
        });
    }
    else if (req.url === "/stranx"){ // pogledamo, ali je 'request' enak "/"
        fs.readFile(path.join(_dirname, "pr03x.html"), (err, data) => {
            if(err){
                res.writeHead(500, {"Content-Typte": "text/plain; charset=utf-8"}); // response write
                res.end("Napaka pri branju datoteke s html stranjo. Branje datoteke pr03x.html ni uspelo"); 
            } else {
                res.writeHead(200, {"Content-Typte": "text/html; charset=utf-8"}); // response write
                res.end(data); 
            }
        });
    }
    else if (req.url === "/strany"){ // pogledamo, ali je 'request' enak "/"
        fs.readFile(path.join(_dirname, "pr03y.html"), (err, data) => {
            if(err){
                res.writeHead(500, {"Content-Typte": "text/plain; charset=utf-8"}); // response write
                res.end("Napaka pri branju datoteke s html stranjo. Branje datoteke pr03y.html ni uspelo"); 
            } else {
                res.writeHead(200, {"Content-Typte": "text/html; charset=utf-8"}); // response write
                res.end(data); 
            }
        });
    }
});

server.listen(port, hostname, () => {
    // v oklepaju zapišemo narekovaje z Alt+Gt + 7
    console.log(`Strežnik teče na http://${hostname}:${port}`);
});