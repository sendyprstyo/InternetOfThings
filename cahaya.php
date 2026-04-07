<?php
// Menangkap data dari ESP32 (metode GET)
$status = isset($_GET['status']) ? $_GET['status'] : "Belum ada data";

echo "<h1>Hasil Monitoring Cahaya</h1>";
echo "<h3>Kondisi Ruangan saat ini: <span style='color:blue;'>$status</span></h3>";
?>