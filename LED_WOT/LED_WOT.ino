/**
 * Simple server compliant with Mozilla's proposed WoT API
 * Originally based on the HelloServer example
 * Tested on ESP8266, ESP32, Arduino boards with WINC1500 modules (shields or
 * MKR1000)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <Arduino.h>
#include <Thing.h>
#include <WebThingAdapter.h>

// TODO: Hardcode your wifi credentials here (and keep it private)
const char *ssid = "LESERC_Wifi";
const char *password = "LESERC2020#$";

#if defined(LED_BUILTIN)
const int ledPin = LED_BUILTIN;
#else
const int ledPin = 13; // manually configure LED pin
#endif

WebThingAdapter *adapter;

const char *ledTypes[] = {"OnOffSwitch", "Light", nullptr};
ThingDevice led("led", "Built-in LED", ledTypes);
ThingProperty ledOn("on", "", BOOLEAN, "OnOffProperty");

bool lastOn = false;

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("PASSOU AQUI");

  // Wait for connection
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, blink ? LOW : HIGH); // active low led
    delay(2000);                       // wait for a second

    blink = !blink;
  }
  digitalWrite(LED_BUILTIN, HIGH); // active low led
  delay(1000);                       // wait for a second

  Serial.println("");
  Serial.print("Connected to ");
  //Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  adapter = new WebThingAdapter("w25", WiFi.localIP());

  led.addProperty(&ledOn);
  adapter->addDevice(&led);
  adapter->begin();
  Serial.write("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led.id);
}
void loop(void) {
  Serial.println("Passou aqui tbm");

  adapter->update();
  bool on = ledOn.getValue().boolean;
  digitalWrite(LED_BUILTIN, on ? LOW : HIGH); // active low led
  if (on != lastOn) {
    Serial.println(led.id);
    Serial.println(": ");
    Serial.println(on);
  }
  lastOn = on;
}
