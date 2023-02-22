#define BLYNK_TEMPLATE_ID "TMPLZkRhZ6I_"
#define BLYNK_TEMPLATE_NAME "JNutIot"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI


#include "BlynkEdgent.h"
#include "Adafruit_HTU21DF.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//Servo servo;
//
LiquidCrystal_I2C lcd(0x27, 16, 2);
//
const int trig = D6;
const int echo = D7;

long duration;
int distance;

Adafruit_HTU21DF htu = Adafruit_HTU21DF();
BlynkTimer timer;

byte termometru[8] = //icon for termometer
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

byte picatura[8] = //icon for water droplet
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
  float temp = htu.readTemperature();
  float rel_hum = htu.readHumidity();

  Blynk.virtualWrite(V5, temp);
  Blynk.virtualWrite(V6, rel_hum);
}

void setup()
{
  //  servo.attach(18);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  //  //  lcd.init();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("SWEET HOME");
    lcd.createChar(1, termometru);
    lcd.createChar(2, picatura);
  Serial.begin(115200);
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
  delay(100);

  BlynkEdgent.begin() ;
  delay(2000);
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Door();
    LCD_TEMP();

  BlynkEdgent.run();
  timer.run();

}

void Door() {
  //  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);

  distance = duration * 0.034 / 2;

  Serial.println(distance);
  //  if (distance <= 5) {
  //    servo.write(90);
  //  } else {
  //    servo.write(0);
  //  }
  //  delay(10);
}

void LCD_TEMP() {
  float temp = htu.readTemperature();
  float rel_hum = htu.readHumidity();

  lcd.setCursor(0, 1);
  lcd.write(1);
  lcd.setCursor(1, 1);
  lcd.print(temp);
  lcd.setCursor(6, 1);
  lcd.print((char)223); //degree sign
  lcd.print("C");

  lcd.setCursor(9, 1);
  lcd.write(2);
  lcd.setCursor(10, 1);
  lcd.print(rel_hum);
  lcd.print("%");
}
