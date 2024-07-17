const express = require('express');
const open = require('open');
const path = require('path');
const addon = require('./build/Release/addon');

const app = express();
const PORT = 3000;

app.use(express.urlencoded({ extended: true }));
app.use(express.json());

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.post('/check', (req, res) => {
  const { username } = req.body;
  const privilege = addon.getPrivilege(username);
  if (privilege) {
    res.send(`Пользователь ${username} имеет привилегию ${privilege}`);
  } else {
    res.send('Пользователя нет');
  }
});

app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
  open(`http://localhost:${PORT}`);
});