<?php
// 1. Menangkap data dari URL (dikirim oleh ESP32 melalui metode GET)
// isset digunakan agar tidak muncul error jika file dibuka langsung di browser
$lumen    = isset($_GET['lumen']) ? $_GET['lumen'] : null;
$suhu     = isset($_GET['suhu']) ? $_GET['suhu'] : null;
$humidity = isset($_GET['humidity']) ? $_GET['humidity'] : null;

// Konfigurasi Database
$servername = "localhost";
$username   = "root";
$password   = "";
$dbname     = "eslolin";

// 2. Membuat Koneksi ke Database
$conn = mysqli_connect($servername, $username, $password, $dbname);

// Cek Koneksi
if (!$conn) {
    die("Koneksi gagal: " . mysqli_connect_error());
}

// 3. Validasi: Pastikan data tidak kosong sebelum disimpan
if ($lumen !== null && $suhu !== null && $humidity !== null) {
    
    // Query untuk memasukkan data ke tabel monitoring_ruangan
    // NOW() digunakan untuk mencatat waktu otomatis di kolom tglData
    $sql = "INSERT INTO datasensor (tglData, lumen, suhu, kelembapan) 
            VALUES (NOW(), '$lumen', '$suhu', '$humidity')";

    if (mysqli_query($conn, $sql)) {
        echo "Berhasil: Data tersimpan ke database";
    } else {
        echo "Error SQL: " . mysqli_error($conn);
    }

} else {
    echo "Gagal: Parameter data (lumen, suhu, humidity) tidak lengkap.";
}

// 4. Menutup Koneksi
mysqli_close($conn);
?>