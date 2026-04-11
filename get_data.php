    <?php
    $conn = mysqli_connect("localhost", "root", "", "eslolin");

    $sql = "SELECT * FROM datasensor ORDER BY id DESC LIMIT 1";
    $result = mysqli_query($conn, $sql);

    $data = mysqli_fetch_assoc($result);

    echo json_encode([
    "suhu" => $data['suhu'],
    "kelembapan" => $data['kelembapan'],
    "lumen" => $data['lumen'],
    "tglData" => $data['tglData']
    ]);
    ?>