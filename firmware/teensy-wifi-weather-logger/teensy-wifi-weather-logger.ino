/*
   This program uses Adafruit.io to log data:
   https://learn.adafruit.com/adafruit-io/overview

   Adafruit.io group:
     https://io.adafruit.com/groups/79329

   Adafruit.io dashboard:
     https://io.adafruit.com/drewfustini/teensy-weather-wifi-logger

   This program is based on:
     ESP8266_Phant.ino
     SparkFun ESP8266 AT library - Phant Posting Example
     Jim Lindblom @ SparkFun Electronics
     https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

    GitHub repo:
      https://github.com/pdp7/teensy-wifi-weather-logger

    OSH Park shared park:
      https://oshpark.com/shared_projects/aCAtXvMP

    Developed with Arduino 1.6.8 with Teensyduino 1.28
    on Intel x88 64-bit desktop running Debian GNU/Linux

    Required libraries:
       - SparkFun_ESP8266_AT_Arduino_Library fork
           https://github.com/pdp7/SparkFun_ESP8266_AT_Arduino_Library
           [b54b7ef] Hard coded fix to use Serial2 as connection to ESP8266
       - Adafruit BME280 v1.0.4
       - Adafruit Unified Sensor v1.0.2
       - Adafruit SSD1306 v1.1.0
       - Adafruit GFX v1.1.5

*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <SparkFunESP8266WiFi.h>

#define REFRESH_DELAY_S 2
#define REFRESH_DELAY_MS (REFRESH_DELAY_S * 1000)

// based on SSD1306 example by Paul Stoffregen
// using software SPI (the default case):
// https://www.pjrc.com/teensy/td_libs_SSD1306.html
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

// WiFi Network Definitions
// Replace these two character strings with the name and
// password of your WiFi network.
const char officeSSID[] = "OSH Park - SweetB";
const char officePSK[] = "XXXXXXXXXX";

const char hotspotSSID[] = "teleshark";
const char hotspotPSK[] = "XXXXXXXXXX";

const char *mySSID = hotspotSSID;
const char *myPSK = hotspotPSK;


// ATTENTION: REPLACE WITH YOUR OWN adafruit.io KEY
// for additional adafruit.io info, visit:
// https://learn.adafruit.com/adafruit-io/overview
const String aio_key = "9197bce5b37f5dfc889f1e4b6a91298ed7f00261";
const String hostname = "io.adafruit.com";
const String path_prefix = "/api/feeds/";
const String path_suffix = "/data.json";

// TODO: create a power efficient way to sleep forever
void sleep_forever() {
  while (1) {
    delay(100000);
  }
}

void setup()
{

  // initialize with the I2C addr 0x3C (for the 128x32)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(F("TeensyWiFi"));
  display.print(F("WeatherLog"));
  display.display();
  delay(500);

  int status;
  Serial.begin(115200);
  Serial.println(F("Detecting BME280 sensor..."));
  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    Serial.println(F("BME280 not found"));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.print(F("  check  "));
    display.print(F("   BME280    "));
    display.display();
  }
  delay(500);

  Serial.println(F("BME280 OK"));
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(F("BME280 OK"));
  display.display();
  delay(500);

  // verify communication with ESP8266
  status = esp8266.begin(115200, ESP8266_HARDWARE_SERIAL);
  if (status <= 0)
  {
    Serial.println(F("Unable to communicate with ESP8266. Looping"));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.print(F("  check  "));
    display.print(F("  ESP8266   "));
    display.display();
    sleep_forever();
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("SSID:      ");
  display.setTextSize(1);
  display.print(mySSID);
  display.display();
  delay(500);
  esp8266.setMode(ESP8266_MODE_STA); // Set WiFi mode to station
  if (esp8266.status() <= 0) // If we're not already connected
  {
    if (esp8266.connect(mySSID, myPSK) < 0)
    {
      Serial.print(F("Error connecting to SSID:"));
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(BLACK, WHITE); // 'inverted' text
      display.print(F(" failed to join SSID  "));
      display.display();
      sleep_forever();
    }
  }

  // Get our assigned IP address and print it:
  Serial.print(F("My IP address is: "));
  Serial.println(esp8266.localIP());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(F("IP:        "));
  display.setTextSize(1);
  display.print(esp8266.localIP());
  display.display();
  delay(1000);
}

void loop()
{

  float temp_c_float = bme.readTemperature();
  int temp_c = round( temp_c_float );

  float temp_f_float = ( (temp_c_float * 9.0) / 5.0 ) + 32.0;
  int temp_f = round( temp_f_float );

  float humidity_float = bme.readHumidity();
  int humidity = round ( humidity_float );

  float hpa = bme.readPressure() / 100.0F;
  float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);

  // Autosize Temp *F value
  if ( temp_f < 100) {
    display.setTextSize(3);
  } else {
    display.setTextSize(2);
  }

  // Display Line 1
  display.print( temp_f );
  display.setTextSize(1);
  display.print("F ");
  display.setTextSize(2);
  display.print( temp_c );
  display.setTextSize(1);
  display.print("C ");
  display.setTextSize(2);
  display.print( humidity );
  display.setTextSize(1);
  display.print("%");

  // Display Line 2
  display.setCursor(0, 25);
  display.print( hpa );
  display.setTextSize(1);
  display.print(" hPa ");

  display.print( alt );
  display.setTextSize(1);
  display.println(" m");

  display.display();

  // Debug logging
  Serial.print(temp_f_float);
  Serial.print("*F\t");
  Serial.print(temp_c_float);
  Serial.print("*C\t");
  Serial.print(humidity_float);
  Serial.println("%");

  // Post to server
  postValue("temperature_celsius", temp_c_float);
  postValue("temperature_fahrenheit", temp_f_float);
  postValue("humidity", humidity_float);

  delay(REFRESH_DELAY_MS);

}

void postValue(String feed, float value)
{

  String httpHeader =
    "POST " + path_prefix + feed + path_suffix + " HTTP/1.1\n" +
    "Host: " + hostname + "\n" +
    "X-AIO-Key: " + aio_key + "\n" +
    "Content-Type: application/json\n" +
    "Connection: close\n";

  // Create a client, and initiate a connection
  ESP8266Client client;

  if (client.connect(hostname, 80) <= 0)
  {
    Serial.println(F("Failed to connect"));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.print(F(" failed to  connect  "));
    display.display();

    return;
  }
  Serial.println(F("Connected."));

  // Set up our post parameters:
  String params;
  params = "{\"value\": " + String(value) + "}";

  Serial.print(F("Posting: parmas="));
  Serial.println(params);

  client.print(httpHeader);
  Serial.println(httpHeader);
  client.print("Content-Length: "); client.println(params.length());
  client.println();
  client.print(params);

  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available())
    Serial.write(client.read()); // read() gets the FIFO char

  // connected() is a boolean return value - 1 if the
  // connection is active, 0 if it's closed.
  if (client.connected())
    client.stop(); // stop() closes a TCP connection.

  // avoid flooding the server
  delay(500);
}
