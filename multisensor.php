<?php
    //echo $_GET['lumen'].$_GET['suhu'];
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "eslolin";

    // Create connection
    $conn = mysqli_connect($servername, $username, $password, $dbname);
    
    // Check connection
    if (!$conn) {
        die("Koneksi gagal: " . mysqli_connect_error());
    }

    $sql = "SELECT * FROM sensor order by idSensor";
    $result = mysqli_query($conn, $sql);

    if (mysqli_num_rows($result) > 0) {
        // output data of each row
        while($row = mysqli_fetch_assoc($result)) {
            echo $row["statusSensor"].",";
        }
    } else {
        echo "0 results";
    }

    mysqli_close($conn);
?>