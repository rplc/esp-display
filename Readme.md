# ESP8266 to SPI Display
- SPI synonyms
    - MOSI = SDA / SID
    - CLK = SCL / SCLK
    - DC = RS
    - CS = SS / ST-
    - RESET = RES / RST
- current connection scheme:
  - GND - GND
  - VCC - 3.3V
  - SCK - D3 (Arduino PIN 0)
  - SDA - D2 (Arduino PIN 4)
  - DC - D6 (Arduino PIN 12)
  - CS - D5 (Arduino PIN 14)
  - RES - D7 (Arduino PIN 13)

## u8x8 vs u8g2
- u8x8: limited functionality but faster/less memory consumption, https://github.com/olikraus/u8g2/wiki/u8x8reference
- u8g2: choice for this projekt due to ability to rotate the display, https://github.com/olikraus/u8g2/wiki/u8g2reference

## Learnings
- careful when choosing the font, choose an alphanumerical one!

## ESP8266 to ePaper
- https://www.az-delivery.de/en/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/e-paper-display-am-esp32-und-esp8266-teil1

# ioBroker connection via MQTT
- https://www.smarthome-tricks.de/esp8266/esp8266-oled-display-fuer-iobroker-ueber-mqtt/

# Arduino/ESP8266 development 
- install drivers (if needed) https://github.com/nodemcu/nodemcu-devkit/tree/master/Drivers

# Arduino Button
- https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
- 4 PIN Buttons: https://de.wikipedia.org/wiki/Widerstand_(Bauelement)#Farbkodierung_auf_Widerst%C3%A4nden
- 2 PIN Buttons: https://www.the-diy-life.com/wp-content/uploads/2019/05/Single-Push-Button-On-Single-Arduino-Digital-Input.jpg

# Libaries needed
- U8g2  (https://github.com/olikraus/u8g2)
- PubSubClient (https://pubsubclient.knolleary.net/)