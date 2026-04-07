<?php
// Menangkap data dari ESP32 (metode GET)
$temp = isset($_GET['temp']) ? $_GET['temp'] : "0";

echo "<h1>Hasil Monitoring Suhu</h1>";
echo "<h3>Suhu Ruangan: <span style='color:red;'>$temp °C</span></h3>";
?>