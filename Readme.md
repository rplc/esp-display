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

# ESP8266 to ePaper
- https://www.az-delivery.de/en/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/e-paper-display-am-esp32-und-esp8266-teil1

# ioBroker connection via MQTT
- https://www.smarthome-tricks.de/esp8266/esp8266-oled-display-fuer-iobroker-ueber-mqtt/

# Arduino/ESP8266 development 
- install drivers (if needed) https://github.com/nodemcu/nodemcu-devkit/tree/master/Drivers