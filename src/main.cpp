#include <Arduino.h>
#include "define.h"
// #include <LittleFS.h> //this needs to be first, or it all crashes and burns...

#include "wifiMqttFun.h"

#include "buttons.h"
#include "flowSensor.h"
#include "analogInputs.h"

// needed for library
// #include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

// needed for OTA & MQTT
// #include <ArduinoOTA.h>
// #include <PubSubClient.h>

// #include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  // OUTPUT Definition !
  pinMode(PIN_LED_int, OUTPUT);
  pinMode(PIN_LED_ext, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
  pinMode(PIN_RELAY3, OUTPUT);

  digitalWrite(PIN_LED_int, HIGH);
  digitalWrite(PIN_LED_ext, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  digitalWrite(PIN_RELAY1, LOW); // OFF
  digitalWrite(PIN_RELAY1, LOW); // OFF
  digitalWrite(PIN_RELAY1, LOW); // OFF

  // beeb
  digitalWrite(PIN_BUZZER, HIGH);
  delay(30);
  digitalWrite(PIN_BUZZER, LOW);

  // INPUT Definition

  pinMode(PIN_DS18B20, INPUT);

  // 3 Buttons
  initButtons();

  // flow sensor
  initFlowSensor();

  // initi ADC channels > LDR / Suppy voltage
  initADCchannels();

  /*
   * When using lcd.print() (and almost everywhere you use string literals),
   * is a good idea to use the macro F(String literal).
   * This tells the compiler to store the string array in the flash memory
   * instead of the ram memory. Usually you have more spare flash than ram.
   * More info: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
   */

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("WaterControlV1.0"));

  wifiManagerSetup();

  // get IP address
  Serial.println("local ip");
  ip = ip2Str(WiFi.localIP());
  Serial.println(ip);

  // OTA starts here!
  otaSetup();

  lcd.setCursor(0, 1);
  lcd.print(F("WiFi connected"));
  delay(BOOT_TIMING);

  lcd.setCursor(0, 1);
  lcd.print(F("IP "));
  lcd.print(ip); // ip to LCD
  delay(BOOT_TIMING);

  // MQTT - Connection:
  mqttStartup();

  lcd.setCursor(0, 1);
  lcd.print(F("MQTT connected  "));
  delay(BOOT_TIMING);

  // Webserver!
  server.begin();

  lcd.setCursor(0, 1);
  lcd.print(F("WebServer done  "));
  delay(BOOT_TIMING);

  lcd.setCursor(0, 1);
  lcd.print(F("READY !!!!      "));
  delay(BOOT_TIMING / 2);

  lcd.setCursor(0, 1);
  lcd.print(F("                "));

  analogInputsTask_100();
  event = true;
  RelaisState = false;
  startup = true;

  // beeb
  digitalWrite(PIN_BUZZER, HIGH);
  delay(30);
  digitalWrite(PIN_BUZZER, LOW);
}

String lcd_text = "";

void loop()
{
  static unsigned long lastTransferTime = 0;
  static unsigned long lastTransferMQTTTime = 0;
  static unsigned long lastLedChangeTime = 0;
  static unsigned long AnalogTimer = 0;
  static unsigned long SensorTimer = 0; // Same type as millis()

  static bool ledState = false;

  bool buttonState = true;
  static bool LastbuttonState = true;

  // MQTT connect
  mqttCheck();

  // OTA handler !
  ArduinoOTA.handle();

  // WebServer
  webServer();

  // put your main code here, to run repeatedly:
  /*
    if (millis() - lastTransferTime > (1000000))
    { // tbd sec
      lastTransferTime = millis();

      client.publish(pup_alive, "Hello World!");
    }

    if (millis() - lastTransferMQTTTime > (10000)) // 10 Seconds
    {
      lastTransferMQTTTime = millis();
      client.publish(pup_RelaisStatus, String(RelaisState).c_str(), 1);
      client.publish(pup_Volume, String(volume).c_str(), 1);
    }
  */

  if (millis() - lastLedChangeTime > (1000)) // 1 Second
  {
    lastLedChangeTime = millis();
    if (ledState)
    {
      ledState = false;
      digitalWrite(PIN_LED_int, HIGH);
      digitalWrite(PIN_LED_ext, HIGH);
    }
    else
    {
      ledState = true;
      digitalWrite(PIN_LED_int, LOW);
      digitalWrite(PIN_LED_ext, LOW);
    }
  }

  if (event)
  {
    // Update LCD Display
    lcd.setCursor(0, 1);

    if (RelaisState)
    {
      digitalWrite(PIN_RELAY1, LOW);
      digitalWrite(PIN_RELAY2, HIGH);
      digitalWrite(PIN_RELAY3, HIGH); // safety switch on
      Serial.println("R=ON ");
      lcd.print(F("R=ON "));
    }
    else
    {
      digitalWrite(PIN_RELAY1, HIGH);
      digitalWrite(PIN_RELAY2, LOW);
      digitalWrite(PIN_RELAY3, LOW); // safety switch off
      lcd.print(F("R=OFF"));
      Serial.println("R=OFF");
    }
    event = false;
  }

  static bool relay1 = false;
  static bool relay2 = false;
  static bool relay3 = false;

  if (button1event || button2event || button3event)
  {

    if (button1event)
    {
      Serial.println("Taste 1");
      button1event = false;
      if (relay1)
      {
        digitalWrite(PIN_RELAY1, LOW);
        relay1 = false;
      }
      else
      {
        digitalWrite(PIN_RELAY1, HIGH);
        relay1 = true;
      }

      lcd_text = "R=ON";
    }

    if (button2event)
    {
      Serial.println("Taste 2");
      button2event = false;
      if (relay2)
      {
        digitalWrite(PIN_RELAY2, LOW);
        relay2 = false;
      }
      else
      {
        digitalWrite(PIN_RELAY2, HIGH);
        relay2 = true;
      }
      lcd_text = "R=OFF";
    }

    if (button3event)
    {
      Serial.println("Taste 3");
      button3event = false;
      if (relay3)
      {
        digitalWrite(PIN_RELAY3, LOW);
        relay3 = false;
      }
      else
      {
        digitalWrite(PIN_RELAY3, HIGH);
        relay3 = true;
      }
      lcd_text = "R=OFF";
    }
  }

  if (millis() - SensorTimer >= 1000)
  {
    SensorTimer = millis();
    Serial.print("TIMER : ");
    Serial.println(SensorTimer);

    buttonStatus();

    Serial.print("Suppyl : ");
    Serial.println(readSuppyVoltage());

    flowSensorRead(0);
    Serial.print("Flow rate (L/minute) : ");
    Serial.println(getFlowRate_m());

    Serial.print("Volume (liter) : ");
    volume = getSensorVolume();
    Serial.println(volume);

    Serial.println();

    lcd.setCursor(8, 1);
    lcd.print(volume);

    lcd.setCursor(0, 1);
    lcd.print(lcd_text);
  }

  if (millis() - AnalogTimer >= 100)
  {
    AnalogTimer = millis();
    analogInputsTask_100();
  }

  // Reset Volume
  // if (millis() - reset >= 60000)
  //{
  //  // Sensor.resetVolume();
  //  reset = millis();
  //}
}

void help(void)
{
  if (button1event)
  {
    Serial.println("Taste 1");
    button1event = false;
    event = true;
    RelaisState = true;
    lcd_text = "R=ON";
  }

  if (button2event)
  {
    Serial.println("Taste 2");
    button2event = false;
    event = true;
    RelaisState = false;
    lcd_text = "R=OFF";
  }

  if (button3event)
  {
    Serial.println("Taste 3");
    button3event = false;
    event = true;
    RelaisState = false;

    lcd_text = "R=OFF";
  }
}
