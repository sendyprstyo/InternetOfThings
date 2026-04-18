<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "eslolin";

$conn = mysqli_connect($servername, $username, $password, $dbname);
if (!$conn) {
    die("Koneksi gagal: " . mysqli_connect_error());
}

$data = new stdClass();

// 1. Ambil data LED 1 (idSensor = 1)
$sqlLampu = "SELECT idSensor, namaSensor, statusSensor FROM sensor WHERE idSensor=1";
$resultLampu = mysqli_query($conn, $sqlLampu);
$data->Lampu = mysqli_fetch_assoc($resultLampu);

// 2. Ambil data LED 2 (idSensor = 2)
$sqlLED2 = "SELECT idSensor, namaSensor, statusSensor FROM sensor WHERE idSensor=2";
$resultLED2 = mysqli_query($conn, $sqlLED2);
$data->LED2 = mysqli_fetch_assoc($resultLED2);

// 3. Tambahkan Nilai Sensor dummy sesuai soal
$data->NilaiSensor = array(
    "suhu" => 36,
    "kelembapan" => 40,
    "cahaya" => 120
);

mysqli_close($conn);

header('Content-Type: application/json');
echo json_encode($data, JSON_PRETTY_PRINT);