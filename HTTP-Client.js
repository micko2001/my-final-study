const http = require("http");

let count = 0;

// Alamat IP ESP32 dan port server HTTP
const esp32Url = "http:192.168.0.100/";

function requestData() {
  // Buat permintaan HTTP GET ke server ESP32
  http
    .get(esp32Url, (response) => {
      let data = "";

      // Mengumpulkan data yang diterima dari server
      response.on("data", (chunk) => {
        data += chunk;
      });

      // Menampilkan data setelah semua data diterima
      response.on("end", () => {
        console.log("Data dari ESP32:", data);
        const objData = JSON.parse(data.toString());

        console.log("Suhu: " + objData.temperature + " \u00B0C");
        console.log("Kelembapan: " + objData.humidity + " %");
        console.log("MQ-Digital Value:" + objData.adcMq);
        console.log(`Data ke- ${count}`);
        count += 1;
      });
    })
    .on("error", (error) => {
      console.error("Error:", error.message);
    });
}

const interValid = setInterval(() => {
  requestData();
  console.log("Jalan");

  if (count > 70) {
    clearInterval(interValid);
    console.log("Stop");
  }
}, 15000);
