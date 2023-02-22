/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
 *************************************************************
  Blynk.Edgent implements:
  - Blynk.Inject - Dynamic WiFi credentials provisioning
  - Blynk.Air    - Over The Air firmware updates
  - Device state indication using a physical LED
  - Credentials reset using a physical Button
 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
#define BLYNK_TEMPLATE_ID "TMPLWkarzZtM"
#define BLYNK_TEMPLATE_NAME "GG Ez"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
#define USE_ESP32_DEV_MODULE
//#define USE_ESP32C3_DEV_MODULE
//#define USE_ESP32S2_DEV_KIT
//#define USE_WROVER_BOARD
//#define USE_TTGO_T7
//#define USE_TTGO_T_OI

#include "BlynkEdgent.h"
#include "DHT.h"
#include <ESP32_Servo.h>

Servo myservo;

int pos = 0; 

#define DHTPIN 21
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)){
    Serial.println("failed to read sensor");
    return;
  }
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(19, value);
}

BLYNK_WRITE(V1) {
  int value2 = param.asInt();
  digitalWrite(18, value2);
}

void setup()
{
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  Serial.begin(115200);
  delay(100);

  dht.begin();

  BlynkEdgent.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  BlynkEdgent.run();
  timer.run();
}

