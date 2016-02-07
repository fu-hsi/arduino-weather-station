/*
  Arduino Weather Station 1.0.0 -  Measuring Humidity and Temperature.
  Copyright (c) 2016 Mariusz Kacki <mariusz@kacki.pl>

  Permission is hereby granted, free of charge, to any person obtaining 
  a copy of this software and associated documentation files (the "Software"), 
  to deal in the Software without restriction, including without limitation 
  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
  and/or sell copies of the Software, and to permit persons to whom the Software 
  is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in 
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
  OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <DHT.h>
#include <UIPEthernet.h>

#define WS_HOST_NAME "ws.example.com"
#define WS_HOST_PORT 80

const static uint8_t REQUEST_BUFFER_SIZE = 250;
const static uint8_t mac[] = { 0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F };
const static uint16_t timerInterval = 1000 * 60; //max 65 535

const static char wsHost[] PROGMEM = WS_HOST_NAME;
const static char wsUri[] PROGMEM = "/ws/ping.php";
const static char wsAgent[] PROGMEM = "WeatherStation/1.0";
const static char wsApiKey[] PROGMEM = "4Jrt39dj";

const static char wsRequestHeaders[] PROGMEM = "POST %S HTTP/1.1\r\nUser-Agent: %S\r\nHost: %S\r\nAccept: */*\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\nConnection: keep-alive\r\nX-ApiKey: %S\r\n\r\n";

static char requestBuffer[REQUEST_BUFFER_SIZE] = "";
static uint16_t connectRetryCount = 0;
static uint16_t timerLastMeasurement = 0;

DHT outSensor;
EthernetClient client;

void setup() {
  Serial.begin(9600);
  outSensor.setup(A0);

  do
  {
    if (connectRetryCount++ > 0) Serial.println("FAIL");
    Serial.print(F("Collect DHCP information..."));
  } while (Ethernet.begin(mac) == 0);

  Serial.println("DONE");

  Serial.print(F("IP Address: "));
  Serial.println(Ethernet.localIP());
  Serial.print(F("Subnet Mask: "));
  Serial.println(Ethernet.subnetMask());
  Serial.print(F("Default Gateway: "));
  Serial.println(Ethernet.gatewayIP());
  Serial.print(F("DNS Server: "));
  Serial.println(Ethernet.dnsServerIP());
}

void loop() {
  uint16_t timerNow = millis();
  if (timerNow - timerLastMeasurement >= timerInterval || timerLastMeasurement == 0) {
    timerLastMeasurement = timerNow;
    timerCallback();
  }
}

void timerCallback() {
  //delay(outSensor.getMinimumSamplingPeriod());

  Serial.print(F("Reading data from DHT..."));

  float humidity = outSensor.getHumidity();
  float temperature = outSensor.getTemperature();

  Serial.println(outSensor.getStatusString());

  if (outSensor.getStatus() != DHT::ERROR_NONE) return;

  Serial.print("DHT22");
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("%\t");
  Serial.print(temperature, 1);
  Serial.write(176);
  Serial.println("C");
  //Serial.print("\t");
  //Serial.println(outSensor.toFahrenheit(temperature), 1);

  Serial.print(F("Connecting to server..."));

  if (Ethernet.maintain() % 2 == 0 && client.connect(WS_HOST_NAME, WS_HOST_PORT) && client.connected()) {
    Serial.println("DONE");

    String data;
    data.reserve(40);

    data.concat("Temperature=");
    data.concat(temperature);
    data.concat("&Humidity=");
    data.concat(humidity);

    uint8_t len = data.length();
    bool written;

    sprintf_P(requestBuffer, wsRequestHeaders, wsUri, wsAgent, wsHost, len, wsApiKey);
    
    Serial.print(F("Sending data..."));
    written = client.write((const uint8_t *)requestBuffer, strlen(requestBuffer)) && client.write((const uint8_t *)data.c_str(), len);
    Serial.println(written ? "DONE" : "FAIL");

    client.stop();
  } else {
    Serial.println("FAIL(" + String(connectRetryCount++) + ")");
  }
}

