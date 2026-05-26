// Povezava_streznik_klient.js


const http = require("http");
// to je staro, po novem rabimo .json datoteko in brez 'require' tukaj 

const hostname = "192.168.1.219";
const port = 80;

const server = http.createServer((req, res) => {
    res.statusCode = 200;
    res.setHeader("Content-Type", "text/plain");
    res.end("Pozdravljen svet VSC v maju!")
});

server.listen(port, hostname, () => {
    console.log("Strežnik teče na http://" + hostname + ":" + port);
});