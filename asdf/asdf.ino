
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Ethernet.h>
#include <Thingplus.h>
#include <TimeLib.h>
#include <Timer.h>
#include <Time.h>

#define RELESE 1
#define DEBUGE 0

#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

const int PulseWire = A0;
const int Threshold = 550;


LiquidCrystal_I2C lcd(0x27, 16, 2);
int period = 20;

double alpha = 0.75;
double change = 0.0;

byte mac[6] = {0x00, 0x08, 0xDC, 0x51, 0xA6, 0x2B};
const char *api = "eh_YAczhVICGGZnntSs8qiuNPi4=";
const char *light = "light-0008dc51a62b-0";
static EthernetClient ethernetClient;

Timer t;

int ot = 0;

PulseSensorPlayground pulseSensor;

void setup ()
{
  Serial.begin (9600);
  Serial.println("asdfg");
  Ethernet.begin(mac);
  lcd.begin();
  lcd.backlight();

  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(13);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

  char st[64];
  IPAddress ip = Ethernet.localIP();
  Serial.print("[INFO] IP:");
  Serial.println(ip);
  Thingplus.begin(ethernetClient, mac, api);
  Thingplus.connect();
}
time_t current;
time_t nextReportInterval = now();

void loop ()
{
  Thingplus.loop();
  t.update();
#if RELESE
  int BPM = pulseSensor.getBeatsPerMinute();
  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("BPM: ");
    Serial.println(BPM);
  }
  delay (period);            // Wait 20 mSec
  lcd.print(BPM);
  if (millis() - 30000 > ot) {
    ot = millis();
    Thingplus.gatewayStatusPublish(true, 30);
    Thingplus.sensorStatusPublish(light, true, 30);
    Thingplus.valuePublish(light, BPM);
  }
#endif
#if DEBUGE
  if (Serial.available()) {
    Thingplus.gatewayStatusPublish(true, 30);
    int Ser = analogRead(A0);
    Serial.read();
    Thingplus.sensorStatusPublish(light, true, 30);
    Serial.println(Thingplus.valuePublish(light, Ser));
  }
#endif

}

