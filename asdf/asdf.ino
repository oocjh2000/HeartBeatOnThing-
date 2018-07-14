#include <Arduino.h>
#include <Ethernet2.h>
#include <Thingplus.h>
#include <TimeLib.h>
#include <Timer.h>
#include <Time.h>

#define RELESE 0

#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

int ledPin = 13;
int sensorPin = 0;
int period = 20;

double alpha = 0.75;
double change = 0.0;

byte mac[6] = {0x00, 0x08, 0xDC, 0x51, 0xA6, 0x2B};
const char *api = "eh_YAczhVICGGZnntSs8qiuNPi4=";
const char *light = "light-0008dc51a62b-0";
static EthernetClient ethernetClient;

Timer t;



void setup ()
{
  Serial.begin (9600);
  Serial.println("asdfg");
  Ethernet.begin(mac);

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
  static double oldValue = 0; // used for averaging.
  static double oldChange = 0; // not currently used
  int rawValue = analogRead (sensorPin); // This reads in the value from the analog pin.
  int value = alpha * oldValue + (1 - alpha) * rawValue;

  Serial.print (rawValue);
  Serial.print (",");
  Serial.println (value);
  oldValue = value;
  delay (period);            // Wait 20 mSec
  if (millis() - 20000 > ot) {
    ot = millis();
    Thingplus.gatewayStatusPublish(true, 30);
    Thingplus.sensorStatusPublish(light, true, 30);
    Thingplus.valuePublish(light, value);
  }
#endif

  if (Serial.available()) {
    Thingplus.gatewayStatusPublish(true, 30);
    int Ser = analogRead(A0);
    Serial.read();
    Thingplus.sensorStatusPublish(light, true, 30);
    Serial.println(Thingplus.valuePublish(light, Ser));
  }

}

