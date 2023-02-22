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

#define BLYNK_TEMPLATE_ID "TMPLZkRhZ6I_"
#define BLYNK_TEMPLATE_NAME "JNutIot"

#define BLYNK_FIRMWARE_VERSION "0.1.0"

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

#include <Wire.h>
#include "BlynkEdgent.h"
#include "Adafruit_HTU21DF.h"
#include <LiquidCrystal_I2C.h>
#include <ESP32_Servo.h>
//#include <Adafruit_Sensor.h>
//#include "DHT.h"

//#define DHTPIN 2
//
//#define DHTTYPE DHT11
//
//DHT dht(DHTPIN, DHTTYPE);

Servo myservo;

int servoPin = 15;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trig = 13;
const int echo = 12;

long duration;
int distance;

BlynkTimer timer;



byte termometru[8] =  //icon for termometer
{
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte picatura[8] =  //icon for water droplet
{
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
};

BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(15, value);
}

void sendSensor() {


  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}
void setup() {

  myservo.attach(servoPin);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  // lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SWEET HOME");
  lcd.createChar(1, termometru);
  lcd.createChar(2, picatura);
  Serial.begin(115200);
  delay(100);

  dht.begin();
  BlynkEdgent.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop() {



  BlynkEdgent.run();
  timer.run();
  Door();
  LCD_TEMP();
}


void Door() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);

  distance = duration * 0.034 / 2;

  Serial.println(distance);
  if (distance <= 5) {
    myservo.write(90);
  } else {
    myservo.write(0);
  }
  delay(10);
}

void LCD_TEMP() {
  lcd.setCursor(1, 1);
  lcd.write(1);
  lcd.setCursor(3, 1);
  lcd.print(t);
  lcd.setCursor(5, 1);
  lcd.print((char)223);  //degree sign
  lcd.print("C");

  lcd.setCursor(9, 1);
  lcd.write(2);
  lcd.setCursor(11, 1);
  lcd.print(h);
  lcd.print("%");
}
