<?php
// 1. Konfigurasi Database
$host     = "localhost";
$username = "root";
$password = "";
$database = "eslolin";

// Membuat koneksi ke MySQL
$conn = mysqli_connect($host, $username, $password, $database);

// Cek Koneksi
if (!$conn) {
    die("Koneksi gagal: " . mysqli_connect_error());
}

// 2. Menangkap data POST dari ESP32
// Menggunakan isset agar tidak error jika file dibuka langsung di browser
if (isset($_POST['suhu']) && isset($_POST['cahaya'])) {
    
    $suhu    = $_POST['suhu'];
    $cahaya  = $_POST['cahaya'];
    $status  = isset($_POST['status']) ? $_POST['status'] : "-"; // Status cahaya (Gelap/Terang)

    // 3. Query SQL untuk memasukkan data ke tabel
    // Pastikan nama tabel kamu 'log_sensor'
    $sql = "INSERT INTO log_sensor (suhu, cahaya, keterangan) VALUES ('$suhu', '$cahaya', '$status')";

    if (mysqli_query($conn, $sql)) {
        echo "Berhasil: Data tersimpan ke database.";
    } else {
        echo "Error: " . $sql . "<br>" . mysqli_error($conn);
    }

} else {
    echo "Menunggu kiriman data dari ESP32...";
}

// Menutup koneksi
mysqli_close($conn);
?>