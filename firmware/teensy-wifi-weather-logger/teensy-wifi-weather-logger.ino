/*
***********************************************************

  ESP8266_Phant.ino
  SparkFun ESP8266 AT library - Phant Posting Example
  Jim Lindblom @ SparkFun Electronics

  Original Creation Date: July 16, 2015
  https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

  This example demonstrates how to use the TCP client
  functionality of the SparkFun ESP8266 WiFi library to post
  sensor readings to a Phant stream on
  https://data.sparkfun.com

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
************************************************************/
#include <SparkFunESP8266WiFi.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>

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

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.
const char mySSID[] = "crocboss";
const char myPSK[] = "RespectsYourFreedom2014";

/////////////////////
// Phant Constants //
/////////////////////
// Phant detsination server:
const String phantServer = "data.sparkfun.com";
// Phant public key:
const String publicKey = "lz44lz7D61sGzD5K78EX";
// Phant private key:
const String privateKey = "Elxx7lW1p5hwZA2MKVnj";
/* String httpHeader = "POST /input/" + publicKey + ".txt HTTP/1.1\n" +
                    "Host: " + phantServer + "\n" +
                    "Phant-Private-Key: " + privateKey + "\n" +
                    "Connection: close\n" +
                    "Content-Type: application/x-www-form-urlencoded\n";
*/

const String aio_key = "9197bce5b37f5dfc889f1e4b6a91298ed7f00261";
const String hostname = "io.adafruit.com";
const String path_prefix = "/api/feeds/";
const String feed = "humidity";
const String path_suffix = "/data.json";
String httpHeader =
  "POST " + path_prefix + feed + path_suffix + " HTTP/1.1\n" +
  "Host: " + hostname + "\n" +
  "X-AIO-Key: " + aio_key + "\n" +
  "Content-Type: application/json\n" +
  "Connection: close\n";



void setup()
{

  // initialize with the I2C addr 0x3C (for the 128x32)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("TeensyWiFi");
  display.print("WeatherLog");
  display.display();
  delay(500);

  int status;
  Serial.begin(115200);
  Serial.println("TEST BEGIN");
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    Serial.println("BME280 not found");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.print(F("  check  "));
    display.print(F("   BME280    "));
    display.display();

    while (1) {
      delay(10000);
    }
  }
  Serial.println("BME280 OK");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("BME280 OK");
  display.display();
  delay(500);

  // To turn the MG2639 shield on, and verify communication
  // always begin a sketch by calling cell.begin().
  //status = esp8266.begin();
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
    delay(500);

    while (1) {
      delay(1000);
    }

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
      delay(500);

      while (1) {
        delay(1000);
      }

    }
  }

  // Get our assigned IP address and print it:
  Serial.print(F("My IP address is: "));
  Serial.println(esp8266.localIP());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("IP:        ");
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
  Serial.print(temp_c_float);
  Serial.print("*C\t");
  Serial.print(humidity_float);
  Serial.println("%");

  // Post to SparkFun Phant server
  postToPhant(humidity_float);

  delay(2000);

}

void postToPhant(float sample)
{
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

  // Set up our Phant post parameters:
  String params;
  params = "{\"value\": " + String(sample) + "}";
  //params += "temp_c=" + String(temp_c);
  //params += "analog2=" + String(analogRead(A2)) + "&";
  //params += "analog5=" + String(analogRead(A5));

  Serial.print(F("Posting to Phant: parmas="));
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
}
