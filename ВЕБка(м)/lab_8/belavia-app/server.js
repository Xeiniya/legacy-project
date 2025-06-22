const express = require('express');
const setupRestApi = require('./rest');

const app = express();
const port = process.env.PORT || 3000;

setupRestApi(app);

app.listen(port, () => {
    console.log(`Сервер запущен по адресу http://localhost:${port}`);
});