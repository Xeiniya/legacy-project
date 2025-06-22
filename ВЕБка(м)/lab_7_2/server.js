const express = require('express');
const path = require('path');
const app = express();
const port = 3000;

app.use(express.static(path.join(__dirname)));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/download/flights', (req, res) => {
    res.download(path.join(__dirname, 'flights.json'), 'belavia-flights.json');
});

app.listen(port, () => {
    console.log(`Сервер запущен по адресу http://localhost:${port}`);
});