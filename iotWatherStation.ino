#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>

// ——— User Config ———
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";
#define DHT_PIN    D4         // GPIO2
#define DHT_TYPE   DHT22      // DHT22 (AM2302)
const uint16_t HTTP_PORT = 80;

// ——— Globals ———
DHT            dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp;          // I2C: SDA=D2, SCL=D1
BH1750         lightMeter;
ESP8266WebServer server(HTTP_PORT);

// ——— HTML Page ———
const char* HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>ESP8266 Weather Station</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script>
      function fetchData() {
        fetch('/data').then(resp=>resp.json()).then(js=>{
          document.getElementById('temp').innerText = js.temperature + ' °C';
          document.getElementById('hum').innerText  = js.humidity    + ' %';
          document.getElementById('press').innerText= js.pressure    + ' hPa';
          document.getElementById('lux').innerText  = js.lux         + ' lx';
        });
      }
      setInterval(fetchData, 5000);
      window.onload = fetchData;
    </script>
  </head>
  <body>
    <h2>ESP8266 Weather Station</h2>
    <ul>
      <li>Temperature: <span id="temp">--</span></li>
      <li>Humidity:    <span id="hum">--</span></li>
      <li>Pressure:    <span id="press">--</span></li>
      <li>Light:       <span id="lux">--</span></li>
    </ul>
  </body>
</html>
)rawliteral";

// ——— Handlers ———
void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleData() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float p = bmp.readPressure() / 100.0F; // convert to hPa
  float l = lightMeter.readLightLevel();

  String json = "{";
    json += "\"temperature\":" + String(t,1) + ",";
    json += "\"humidity\":"    + String(h,1) + ",";
    json += "\"pressure\":"    + String(p,1) + ",";
    json += "\"lux\":"         + String(l,0);
  json += "}";

  server.send(200, "application/json", json);
}

// ——— Setup & Loop ———
void setup() {
  Serial.begin(115200);
  dht.begin();
  bmp.begin();        // defaults to 0x76; pass address if 0x77
  lightMeter.begin();

  // Connect Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print('.');
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // HTTP server routes
  server.on("/",    handleRoot);
  server.on("/data",handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}
