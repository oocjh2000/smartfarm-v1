#include <Arduino.h>
#include <Ethernet2.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#include <Time.h>
#include <TimeLib.h>
#include <Timer.h>
#include <Thingplus.h>
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
#include <SimpleDHT.h>
SimpleDHT11 dht11;
#define CONNECT_LED_GPIO  LED_BUILTIN // Thing+ connection indicator LED
#define BUZZER_SOUND      700

#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif
int ledOffTimer = 0;
int ledBlinkTimer = 0;

int BUTTON_GPIO = 2;
int LED_GPIO = 9;
int LIGHT_GPIO = A1;
int BUZZER_GPIO = 5;
int DHT11_GPIO = 4;
int angle_GPIO = A3; 
int reportIntervalSec = 10;

byte mac[6] = {0x00, 0x08, 0xDC, 0x51, 0xA6, 0x4B};
const char *apikey = "K7yCsxtjHJWgXUdqE6D6OrXy32k=";                                //FIXME APIKEY
const char *ledId = "led-0008dc51a64b-0";                 //FIXME LED ID
const char *temperatureId = "temperature-0008dc51a64b-0"; //FIXME TEMPERATURE ID
const char *percentId = "percent-0008dc51a64b-0";         //FIXME HUMIDITY ID
const char *buzzerId = "buzzer-0008dc51a64b-0";           //FIXME BUZZER ID
const char *lightId = "light-0008dc51a64b-0";             //FIXME LIGHT ID
const char *buttonId = "onoff-0008dc51a64b-0";            //FIXME BUTTON ID
const char *angleid = "rotaryAngle-0008dc51a64b-0";
Timer t;
static EthernetClient ethernetClient;

byte temperature = 0, humidity = 0;
bool button_on = false, button_off = false;
void setup() {

  // put your setup code here, to run once:
  _serialInit();
  _ethernetInit();
  _gpioInit();

  Thingplus.begin(ethernetClient, mac, apikey);
  Thingplus.actuatorCallbackSet(actuatingCallback);

  Thingplus.connect();

  digitalWrite(CONNECT_LED_GPIO, HIGH);
}

time_t current;

time_t nextReportInterval = now();

void DHT11Get() {
  dht11.read(DHT11_GPIO, &temperature, &humidity, NULL);
}
int angleget() {
  int angle = analogRead(angle_GPIO);
  return angle;
}
int lightGet() {
  int light = analogRead(LIGHT_GPIO);
  return light;
}
void loop() {
  // put your main code here, to run repeatedly:
  Thingplus.loop();
  t.update();
  u8x8.begin();
  u8x8.setPowerSave(0);
  
  current = now();

  if (current > nextReportInterval) {

    DHT11Get();

    Thingplus.gatewayStatusPublish(true, reportIntervalSec * 3);

    Thingplus.sensorStatusPublish(ledId, true, reportIntervalSec * 3);
    Thingplus.sensorStatusPublish(buzzerId, true, reportIntervalSec * 3);
    Thingplus.sensorStatusPublish(buttonId, true, reportIntervalSec * 3);

    Thingplus.sensorStatusPublish(temperatureId, true, reportIntervalSec * 3);
    Thingplus.valuePublish(temperatureId, temperature);

    Thingplus.sensorStatusPublish(percentId, true, reportIntervalSec * 3);
    Thingplus.valuePublish(percentId, humidity);

    Thingplus.sensorStatusPublish(lightId, true, reportIntervalSec * 3);
    Thingplus.valuePublish(lightId, lightGet());

    Thingplus.sensorStatusPublish(angleid, true, reportIntervalSec * 3);
    Thingplus.valuePublish(angleid, angleget());
  u8x8.setFont(u8x8_font_chroma48medium8_r);
//  u8x8.drawString(0,1,"Thing plus conneted");
/*char st[64] = "";
   u8x8.drawString(1,1,"192.168.0.1");
   Ethernet.localIP();
   //u8x8.drawString(1,1,);
   for(;;);
  u8x8.refreshDisplay();    // for SSD1606/7  */
    nextReportInterval = current + reportIntervalSec;
  }

  if (button_on || button_off) {
    if (button_on == true) {
      Thingplus.valuePublish(buttonId, 0);
      button_on = false;
    }

    if (button_off == true) {
      Thingplus.valuePublish(buttonId, 1);
      button_off = false;
    }
  }
}
