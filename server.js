const express = require("express");
const mysql = require("mysql2");
const bodyParser = require("body-parser");

const app = express();
const port = 3000;

// Middleware agar server bisa membaca data JSON yang dikirim ESP32
app.use(bodyParser.json());

// 1. Konfigurasi Koneksi Database
const db = mysql.createConnection({
  host: "localhost",
  user: "root",
  password: "",
  database: "eslolin",
});

db.connect((err) => {
  if (err) throw err;
  console.log("Terhubung ke Database MySQL!");
});

// 2. Endpoint API untuk menerima data dari ESP32
app.post("/api/sensor", (req, res) => {
  // Mengambil data dari body request (yang dikirim ESP32)
  const { suhu, kelembapan, lumen } = req.body;

  const sql =
    "INSERT INTO datasensor (tglData, suhu, kelembapan, lumen) VALUES (NOW(), ?, ?, ?)";

  db.query(sql, [suhu, kelembapan, lumen], (err, result) => {
    if (err) {
      console.error(err);
      return res.status(500).send("Gagal menyimpan data");
    }
    console.log(`Data masuk: Suhu ${suhu}, Hum ${kelembapan}, LDR ${lumen}`);
    res.status(200).send("Data berhasil disimpan ke MySQL via Node.js!");
  });
});

// 3. Menjalankan Server
app.listen(port, () => {
  console.log(`Server berjalan di http://localhost:${port}`);
});