# Teensy WiFi weather logging badge
Weather WiFi logging badge with Teensy 3.2, ESP8266, 128x32 OLED and BME280 sensor

## Bill of Materials
* [Teensy 3.2 _(OSH Park Edition)_](https://oshpark.com/teensy)
* [ESP-01 module _(ESP8266 SoC)_](https://www.amazon.com/Diymall%C2%AE-Esp8266-Serial-Wireless-Transceiver/dp/B00O34AGSU)
* [Adafruit I2C OLED _(128x32)_](https://www.adafruit.com/product/931)
* [Adafruit I2C BME280 _(temp/hum/press)_](https://www.adafruit.com/product/2652)
* 10K Ohm through-hole resistor

## OSH Park Shared Project
* **[Teensy WiFi weather logging badge](https://oshpark.com/projects/aCAtXvMP)**
   * This is what was ordered and tested
   * File: [teensyi2c.kicad_pcb](https://github.com/pdp7/teensy-wifi-weather-logger/blob/7003174a60241e1a554e16e218b749da8fd9b785/hardware/teensyi2c.kicad_pcb)
   * Commit: [[7003174]](https://github.com/pdp7/teensy-wifi-weather-logger/commit/7003174a60241e1a554e16e218b749da8fd9b785)

## Firmware
* Developed with [Arduino 1.6.8](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous) with [Teensyduino 1.28](https://www.pjrc.com/teensy/td_download.html) on Intel x88 64-bit desktop running Debian GNU/Linux
* Libraries
   * [SparkFun_ESP8266_AT_Arduino_Library fork](https://github.com/pdp7/SparkFun_ESP8266_AT_Arduino_Library)
     * [[b54b7ef]](https://github.com/pdp7/SparkFun_ESP8266_AT_Arduino_Library/commit/b54b7ef9adb190625479ca260df3bd32e37d1230) Hard coded fix to use Serial2 as connection to ESP8266
     * _TODO: submit a pull request to SparkFun with more general fix_
   * Adafruit BME280 v1.0.4: `Adafruit_BME280_Library` 
   * Adafruit Unified Sensor v1.0.2: `Adafruit_Unified_Sensor`
   * Adafruit SSD1306 v1.1.0: `Adafruit_SSD1306` 
   * Adafruit GFX v1.1.5: `Adafruit_GFX`

## Data:
* [Adafruit.io dashboard](https://io.adafruit.com/drewfustini/teensy-weather-wifi-logger#)
* [Adafruit.io group](https://io.adafruit.com/groups/79329)

## Videos
* [Teensy WiFi Weather Logger](https://www.youtube.com/watch?v=4_19no4auhY)
* [Teensy WiFi Weather Logger (short demo) ](https://www.youtube.com/watch?v=Mvm6nQrg_Dw)

## Photos
* [Google Photos album](https://goo.gl/photos/Le2FGBFNvm8KNdKC9)
* [images](/images)
![PCB preview top](https://github.com/pdp7/teensy-wifi-weather-logger/blob/master/images/pcb-preview-top.png)

## License
This project released under the CERN Open Hardware v1.2 License

**Note:** [teensy-wifi-weather-logger.ino](https://github.com/pdp7/teensy-wifi-weather-logger/blob/master/firmware/teensy-wifi-weather-logger/teensy-wifi-weather-logger.ino) contains code based on:

* [Capacitive sensors with the Teensy 3.0](http://njhurst.com/blog/01356576041)
* [SSD1306 display code based on example by Paul Stoffregen](https://www.pjrc.com/teensy/td_libs_SSD1306.html)
* [SparkFun ESP8266 AT library - Phant Posting Example](https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library) by Jim Lindblom @ SparkFun Electronics

## Contact
* Drew Fustini &lt;drew@oshpark.com&gt;
