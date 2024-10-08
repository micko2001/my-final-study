#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>//pin 21 and 22
#include "DHT.h"


#define DHTPIN 4    
#define DHTTYPE DHT11   // DHT 11

const char* ssid = "**********";
const char* password = "************";
const int HTTPPort = 80;

//deklarasi server
WebServer server(HTTPPort);

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
int analogSens =34;

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//
int counter=0;

// Fungsi untuk mengirimkan respons CoAP
void handleRequest() {
  

  const char* data = dataOutput();
  Serial.print("data: ");
  Serial.println(data);
  Serial.println("dikirim!");
  //counter+=1;
  server.sendHeader("Content-Type", "text/plain");
  server.send(200, "text/plain", data);
}

void printLCD (float temp, float hum, float vol){
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.setCursor(2, 1);
  lcd.print(temp);
  lcd.setCursor(4, 1);
  lcd.print("\337C");
  lcd.setCursor(6, 1);
  lcd.print("H:");
  lcd.setCursor(8, 1);
  lcd.print(hum);
  lcd.setCursor(10, 1);
  lcd.print("%");
  lcd.setCursor(11, 1);
  lcd.print("V:");
  lcd.setCursor(13, 1);
  lcd.print(vol);
}

const char* dataOutput (){
   // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float analogValue = analogRead(analogSens);

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["temperature"]=t;
  jsonDoc["humidity"]=h;
  jsonDoc["adcMq"]=analogValue;
 // jsonDoc["count"]=counter;
  
    // Mengonversi objek JSON menjadi string
  static String jsonString;
  jsonString = "";
  serializeJson(jsonDoc, jsonString);
  return jsonString.c_str();
}

void setup() {
  Serial.begin(9600);

    // Menghubungkan ke jaringan WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.printf("http://%s:%d\n", WiFi.localIP().toString().c_str(), HTTPPort);


  Serial.println(F("DHT11 test!"));

  dht.begin();
  pinMode(analogSens,INPUT);

// initialize the lcd
  lcd.init();                       
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  String httpAddress = "";
  httpAddress += WiFi.localIP().toString();
  httpAddress += ":";
  httpAddress += String(HTTPPort);
  lcd.print(httpAddress);
 // Mengatur route untuk menangani permintaan ke root path
  server.on("/", handleRequest);

  // Memulai server HTTP
  server.begin();;

}

void loop() {


  server.handleClient();


  const char* data = dataOutput();

  // Create a JSON document
  StaticJsonDocument<200> doc;

  // Parse the JSON string
  DeserializationError error = deserializeJson(doc, data);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
  int temp = doc["temperature"];
  int hum = doc["humidity"];
  int adcMq = doc["adcMq"];

  // Serial.print(F("Humidity: "));
  // Serial.print(hum);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(temp);
  // Serial.print(F("°C "));
  // Serial.print("Analog:");
  // Serial.println(adcMq);

  // printLCD(temp,hum,adcMq);
  
  // Wait a few seconds between measurements.
  //delay(3000);
}
