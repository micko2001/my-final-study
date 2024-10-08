const coap = require("coap");
let count = 0;
// Alamat IP ESP32 dan port server CoAP
const esp32Url = {
  host: "192.168.0.100", // Ganti dengan alamat CoAP Anda
  port: 5683,
};

function requestData() {
  // Buat permintaan CoAP GET ke server ESP32
  const req = coap.request(esp32Url);

  req.on("response", (response) => {
    let data = "";

    // Mengumpulkan data yang diterima dari server
    response.on("data", (chunk) => {
      data += chunk;
    });

    // Menampilkan data setelah semua data diterima
    response.on("end", () => {
      console.log("Data dari ESP32:", data);
      const objData = JSON.parse(data.toString());
      try {
        console.log("Suhu: " + objData.temperature + " \u00B0C");
        console.log("Kelembapan: " + objData.humidity + " %");
        console.log("MQ-Digital Value:" + objData.adcMq);
        console.log(`Data ke-${count}`);
        count += 1;
      } catch (error) {
        console.error("Error parsing JSON:", error.message);
      }
    });
  });

  req.on("error", (error) => {
    console.error("Error:", error.message);
  });

  req.end();
}

const interValid = setInterval(() => {
  requestData();
  console.log("Jalan");

  if (count > 70) {
    clearInterval(interValid);
    console.log("Stop");
  }
}, 15000);
