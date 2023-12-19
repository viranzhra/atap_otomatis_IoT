const mqtt = require("mqtt");
const mysql = require('mysql');

var connection = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: '',
  database: 'tubes_iot'
});

connection.connect();

const client = mqtt.connect("ws://broker.hivemq.com:8000/mqtt");

client.on("connect", () => {
  client.subscribe("sensor/atap", (err) => {
    if (!err) {
      console.log("Terhubung ke broker MQTT");
    }
  });
});

client.on("message", (topic, message) => {
  // Log pesan yang diterima
  console.log(message.toString());
  // Pisahkan pesan menjadi nilai hujan, cahaya, dan status atap
  let data = message.toString().split('#');
  console.log(data);

  // Periksa apakah format data sesuai dengan yang diharapkan
  if (data.length === 3) {
    let hujanValue = parseInt(data[0]); 
    let cahayaValue = parseInt(data[1]); 
    let statusAtap = data[2];

    // Konversi nilai numerik menjadi string deskriptif
    let hujan = hujanValue === 0 ? "Hujan" : "Tidak Hujan";
    let cahaya = cahayaValue === 0 ? "Terang" : "Gelap";

    // Masukkan data ke dalam database MySQL
    connection.query('INSERT INTO sensor (hujan, cahaya, status_atap) VALUES (?, ?, ?)', [hujan, cahaya, statusAtap], 
      function (error, results, fields) {
        if (error) throw error;
        console.log('Data dimasukkan ke dalam database MySQL');
      }
    );
  } else {
    console.log('Format data yang diterima tidak valid');
  }
});
