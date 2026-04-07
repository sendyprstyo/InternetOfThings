<?php
    /**
     * Nama File : sensorsingle.php
     * Deskripsi : Hanya membaca SATU statusSensor (id 1)
     */

    $servername = "localhost";
    $username   = "root";
    $password   = ""; 
    $dbname     = "eslolin";

    $conn = mysqli_connect($servername, $username, $password, $dbname);

    if (!$conn) {
        die("Connection failed: " . mysqli_connect_error());
    }

    // Query diubah: Tambahkan WHERE idSensor = 1
    $sql    = "SELECT statusSensor FROM sensor WHERE idSensor = 1";
    $result = mysqli_query($conn, $sql);

    if (mysqli_num_rows($result) > 0) {
        // Karena hanya satu data, kita tidak butuh perulangan while yang panjang
        $row = mysqli_fetch_assoc($result);
        echo $row["statusSensor"];
    } else {
        echo "0 results";
    }

    mysqli_close($conn);
?>