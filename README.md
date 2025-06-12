# IoT Weather Station with NodeMCU (ESP8266)

A self-contained weather station that measures temperature, humidity, pressure and ambient light, then serves a live dashboard over Wi-Fi. Perfect for home automation or remote monitoring.

## 1. Concept
- **Sensors**  
  • DHT22 for temperature & relative humidity  
  • BMP280 for barometric pressure & temperature  
  • BH1750 for ambient light (lux)  
- **Controller**  
  • NodeMCU (ESP8266) runs all sensors on I²C (except DHT22) and hosts a small HTTP server  
- **Dashboard**  
  • Web page served by ESP8266, auto-refreshing every 5 s  
- **Power**  
  • 5 V USB supply (via micro-USB)  

## 2. Bill of Materials
- 1 × NodeMCU ESP8266 board  
- 1 × DHT22 sensor + 4.7 kΩ pull-up resistor  
- 1 × BMP280 breakout (I²C)  
- 1 × BH1750 breakout (I²C)  
- 1 × Breadboard + jumper wires  
- 1 × USB cable (for power & programming)

## 3. Wiring Diagram

         NodeMCU ESP8266
        ┌─────────────────┐
        │           D4 ───┤── DHT22 DATA  
        │           5V ───┤── DHT22 VCC
        │          GND ───┤── DHT22 GND
        │           5V ───┤── BMP280 VCC
        │          GND ───┤── BMP280 GND
        │           D1 ──┐┴─ SCL (I²C)
        │           D2 ──┤─ SDA (I²C)
        └─────────────────┘

• Place 4.7 kΩ between D4 and 5 V for the DHT22 pull-up.  
• BMP280 & BH1750 share the same SDA/SCL bus.

## 4. Software Setup
1. Install Arduino IDE ≥1.8.10 and add **ESP8266** board support via Board Manager.  
2. In Library Manager, install:
   - **DHT sensor library** by Adafruit  
   - **Adafruit BMP280**  
   - **BH1750** by Christopher Laws  
3. Create a new sketch named `WeatherStation.ino` and paste the code below.

## 5. Usage
- Edit `WIFI_SSID` and `WIFI_PASS` to match your network.  
- Upload to NodeMCU.  
- Open Serial Monitor at 115200 baud to see IP address.  
- Browse to `http://<ESP_IP>/` on any device.

Enjoy a live, auto-updating weather dashboard on your local network!

