# arduino-weather-station

This is a simple Weather Station project with Arduino for Arduino Nano.

It can measure temperature and humidity by DHT11 or DHT22 sensor.

Weather Station sends data to server over HTTP protocol.
Script over the server-side is written in PHP.

## Hardware setup
1. Arduino Nano 3.0 ATmega328P
2. ENC28J60 LAN Ethernet Network Module
3. DHT22 Digital Temperature and Humidity Sensor AM2302 Module

## Used libraries
1. https://github.com/markruys/arduino-DHT
2. https://github.com/ntruchsess/arduino_uip

## Wiring diagram
![Wiring diagram](http://blog.kacki.pl/wp-content/uploads/2016/02/ws1-1024x787.png)

## Couple of images
![Weather station](http://blog.kacki.pl/wp-content/uploads/2016/02/P1050615-001-1024x684.jpg)
![Serial monitor](http://blog.kacki.pl/wp-content/uploads/2016/02/ws-serial-4.png)
![amCharts](http://blog.kacki.pl/wp-content/uploads/2016/02/ws-graph.png)
