<?php
$host = "localhost"; // Biasanya cukup localhost
$user = "root";
$pass = "";
$db   = "db_monitoring";

$conn = new mysqli($host, $user, $pass, $db);

if ($conn->connect_error) {
    die("Koneksi Gagal: " . $conn->connect_error);
}

if (isset($_POST['lumen'])) {
    $lumen = $_POST['lumen'];

    // GUNAKAN UPDATE: Mengganti nilai di baris yang ID-nya 1
    $sql = "UPDATE kecerahan_ruangan SET nilai_lumen = '$lumen' WHERE id = 1";

    if ($conn->query($sql) === TRUE) {
        echo "Data Kecerahan Berhasil Diperbarui";
    } else {
        echo "Error: " . $conn->error;
    }
}
$conn->close();