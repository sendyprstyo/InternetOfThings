<?php
$conn = mysqli_connect("localhost", "root", "", "eslolin");

if (isset($_GET['ubah_status'])) {
    $id = $_GET['id'];
    $status_baru = $_GET['ubah_status'];
    mysqli_query($conn, "UPDATE sensor SET statusSensor='$status_baru' WHERE idSensor='$id'");
    header("Location: index.php");
}

$statusLED1 = mysqli_fetch_assoc(mysqli_query($conn, "SELECT statusSensor FROM sensor WHERE idSensor=1"))['statusSensor'];
$statusLED2 = mysqli_fetch_assoc(mysqli_query($conn, "SELECT statusSensor FROM sensor WHERE idSensor=2"))['statusSensor'];
?>

<!DOCTYPE html>
<html>

<head>
    <title>Kontrol IoT - LED</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 50px;
            background-color: #f4f4f9;
        }

        .card {
            display: inline-block;
            padding: 20px;
            margin: 15px;
            border-radius: 12px;
            width: 220px;
            background: white;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }

        .btn {
            padding: 10px 25px;
            text-decoration: none;
            color: white;
            border-radius: 5px;
            font-weight: bold;
            display: block;
            margin-top: 15px;
        }

        .btn-on {
            background-color: #28a745;
        }

        .btn-off {
            background-color: #dc3545;
        }

        h3 {
            color: #333;
        }
    </style>
</head>

<body>
    <h2>Panel Kontrol LED</h2>

    <div class="card">
        <h3>LED 1</h3>
        <p>Status: <strong><?php echo ($statusLED1 == 1) ? "MENYALA" : "MATI"; ?></strong></p>
        <a href="?id=1&ubah_status=<?php echo ($statusLED1 == 1) ? '0' : '1'; ?>"
            class="btn <?php echo ($statusLED1 == 1) ? 'btn-off' : 'btn-on'; ?>">
            <?php echo ($statusLED1 == 1) ? 'Matikan' : 'Nyalakan'; ?>
        </a>
    </div>

    <div class="card">
        <h3>LED 2</h3>
        <p>Status: <strong><?php echo ($statusLED2 == 1) ? "MENYALA" : "MATI"; ?></strong></p>
        <a href="?id=2&ubah_status=<?php echo ($statusLED2 == 1) ? '0' : '1'; ?>"
            class="btn <?php echo ($statusLED2 == 1) ? 'btn-off' : 'btn-on'; ?>">
            <?php echo ($statusLED2 == 1) ? 'Matikan' : 'Nyalakan'; ?>
        </a>
    </div>
</body>

</html>