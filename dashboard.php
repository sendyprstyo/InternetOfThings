<!DOCTYPE html>
<html>
<head>
<title>IoT Dashboard</title>

<style>
body {
  font-family: Arial;
  background: linear-gradient(135deg, #4facfe, #00f2fe);
  text-align: center;
  color: white;
}

h1 {
  margin-top: 30px;
}

.container {
  display: flex;
  justify-content: center;
  flex-wrap: wrap;
}

.card {
  background: white;
  color: black;
  padding: 20px;
  margin: 15px;
  border-radius: 15px;
  width: 200px;
  box-shadow: 0 10px 25px rgba(0,0,0,0.3);
}

.value {
  font-size: 30px;
  font-weight: bold;
}

.label {
  color: gray;
}
</style>
</head>

<body>

<h1>📊 Monitoring Sensor IoT</h1>

<div class="container">

  <div class="card">
    <div class="value" id="suhu">-</div>
    <div class="label">Suhu (°C)</div>
  </div>

  <div class="card">
    <div class="value" id="kelembapan">-</div>
    <div class="label">Kelembapan (%)</div>
  </div>

  <div class="card">
    <div class="value" id="lumen">-</div>
    <div class="label">Kecerahan (LDR)</div>
  </div>

</div>

<p id="waktu"></p>

<script>
function loadData() {
  fetch("get_data.php")
  .then(res => res.json())
  .then(data => {
    document.getElementById("suhu").innerHTML = data.suhu + " °C";
    document.getElementById("kelembapan").innerHTML = data.kelembapan + " %";
    document.getElementById("lumen").innerHTML = data.lumen;
    document.getElementById("waktu").innerHTML = "Update: " + data.tglData;
  });
}

setInterval(loadData, 2000);
</script>

</body>
</html>