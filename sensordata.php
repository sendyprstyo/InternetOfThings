<?php
// Cek apakah data sudah dikirim atau belum agar tidak error "Undefined key"
$dataHumidity = isset($_GET['humidity']) ? $_GET['humidity'] : 0;
$dataSuhu = isset($_GET['suhu']) ? $_GET['suhu'] : 0;

echo "Data diterima - Kelembapan: " . $dataHumidity . " | Suhu: " . $dataSuhu;

$servername = "localhost";
$username = "root";
$password = "";
$dbname = "eslolin";

// Membuat koneksi
$conn = mysqli_connect($servername, $username, $password, $dbname);

// Cek koneksi
if (!$conn) {
    // Menampilkan pesan error yang lebih detail
    die("<br>Koneksi ke Database Gagal: " . mysqli_connect_error());
}

// Hanya simpan ke database jika data bukan nol (mencegah data kosong masuk)
if($dataSuhu != 0){
    $sql = "INSERT INTO datasensor (tglData, suhu, kelembapan) VALUES (NOW(), $dataSuhu, $dataHumidity);";

    if (mysqli_query($conn, $sql)) {
        echo "<br>Berhasil simpan ke database!";
    } else {
        echo "<br>Error SQL: " . mysqli_error($conn);
    }
}

mysqli_close($conn);
?>