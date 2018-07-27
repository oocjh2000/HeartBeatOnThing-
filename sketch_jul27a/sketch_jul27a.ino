#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#include <TimeLib.h>
#include <Timer.h>
#include <Time.h>

#define RELESE 1

#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

const int PulseWire = A0;
const int Threshold = 510;


LiquidCrystal_I2C lcd(0x3F, 16, 2);
int period = 20;

double alpha = 0.75;
double change = 0.0;

byte mac[6] = {0x00, 0x08, 0xDC, 0x51, 0xA6, 0x2B};
const char *api = "eh_YAczhVICGGZnntSs8qiuNPi4=";
const char *light = "light-0008dc51a62b-0";

Timer t;

unsigned int ot = 0;

PulseSensorPlayground pulseSensor;

void setup ()
{
  Serial.begin (9600);
  Serial.println("asdfg");


  lcd.begin();
  lcd.backlight();
  lcd.print("BPM");

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(13);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

  char st[64];

}
time_t current;
time_t nextReportInterval = now();

void loop ()
{
  Thingplus.loop();
  unsigned int dt = millis();
  t.update();
#if RELESE

  int BPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("BPM: ");
    Serial.println(BPM);
    lcd.setCursor(0, 1);
    lcd.print(BPM);
  }
  delay (period);            // Wait 20 mSec
  if (dt - ot >= 3000) {
    ot = dt;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BPM");

  }
#endif
}

