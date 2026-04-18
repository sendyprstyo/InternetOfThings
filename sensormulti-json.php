<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "eslolin";

// koneksi
$conn = mysqli_connect($servername, $username, $password, $dbname);

// cek koneksi
if (!$conn) {
    die("Koneksi gagal: " . mysqli_connect_error());
}

// ambil data sensor id = 1 (LED 1)
$sql = "SELECT idSensor, namaSensor, statusSensor FROM sensor WHERE idSensor=1";
$result = mysqli_query($conn, $sql);

$objData = new stdClass();

if (mysqli_num_rows($result) > 0) {
    while($row = mysqli_fetch_assoc($result)) {
        $objData->idSensor = $row["idSensor"];
        $objData->namaSensor = $row["namaSensor"];
        $objData->statusSensor = $row["statusSensor"];
    }
} else {
    echo "0 results";
}

// tambahan data (boleh ada, tapi tidak dipakai LED)
$objData->add = [36,'40'];

mysqli_close($conn);

// convert ke JSON
$dataJSON = json_encode($objData, JSON_FORCE_OBJECT);
echo $dataJSON;
?>