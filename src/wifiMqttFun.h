/*
 * webServer.h
 *
 *  Created on: 23.10.2024
 *      Author: A.Huth
 */

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <Arduino.h>
#include "define.h"
#include <LittleFS.h> //this needs to be first, or it all crashes and burns...

#ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
#include <time.h>
#endif

#define FORMAT_LITTLEFS_IF_FAILED true

// #include "buttons.h"
// #include "flowSensor.h"
// #include "analogInputs.h"
// #include <LCD_I2C.h>

// needed for library
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

// needed for OTA & MQTT
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

// define your default values here, if there are different values in config.json, they are overwritten.
char mqtt_server[40] = MQTT_SERVER;
char mqtt_port[6] = MQTT_PORT;

// Project name
char WifiApName[40] = WIFI_AP_NAME;
char MQTTClientName[40] = MQTT_CLIENT_NAME;
char OtaClientName[40] = OTA_CLIENT_NAME;

// MQTT
const char *pup_alive = "/watering/WaterControlActive";
const char *pup_Volume = "/watering/volume";
const char *pup_RelaisStatus = "/watering/relaisStatus";

const char *sub_RelaisControl = "/watering/relaisControl";
const char *sub_value2 = "/watering/value2";
const char *sub_value3 = "/watering/value3";

// WIFI
WiFiClient espClient;
PubSubClient client(espClient);

WiFiServer server(80);

String ip = "";
bool startup = true;

bool RelaisState = false;
bool event = false;
float volume = 0.00000001;

// flag for saving data
bool shouldSaveConfig = false;

// callback notifying us of the need to save config
void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

String ip2Str(IPAddress ip)
{
  String s = "";
  for (int i = 0; i < 4; i++)
  {
    s += i ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}

// OTA setup function:
void otaSetup(void)
{
  // Ergänzung OTA
  // Port defaults to 3232
  ArduinoOTA.setPort(3232);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("WEMOS_ESP32");

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"admin2water");

  ArduinoOTA.onStart([]()
                     {
                       Serial.println("Start");
                       // analogWrite(14, 0);
                       // digitalWrite(2, LOW);
                     });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
  ArduinoOTA.begin();
}

// MQTT callback function:
void MQTTcallback(char *watering, byte *payload, unsigned int length)
{
  Serial.print("Message arrived @ PUB [");
  Serial.print(watering);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp(watering, sub_RelaisControl) == 0)
  {
    event = true;
    if ((char)payload[0] == '0')
    {
      RelaisState = false;
      Serial.println("false");
    }
    else
    {
      RelaisState = true;
      Serial.println("true");
    }
  }

  if (strcmp(watering, sub_value2) == 0)
  {
  }

  if (strcmp(watering, sub_value3) == 0)
  {
  }
}

// RECONNECT MQTT Server
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTTClientName))
    {

      client.subscribe(sub_RelaisControl);
      client.loop();
      client.subscribe(sub_value2);
      client.loop();
      client.subscribe(sub_value3);
      client.loop();

      Serial.println("connected ...");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage;
  htmlPage.reserve(1024); // prevent ram fragmentation
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n" // the connection will be closed after completion of the response
               //"Refresh: 5\r\n"         // refresh the page automatically every 5 sec
               "\r\n"
               "<!DOCTYPE HTML>"
               "<html>");
  htmlPage += "<body><h1>ESP8266 Web Server</h1>";
  htmlPage += "<br>";

  htmlPage += "Device Name      : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += DEVICE_NAME;           // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br> <br>";

  htmlPage += "SW-Version       : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += SW_BASE_VERSION;       // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br> <br>";

  htmlPage += "IP Adresse       : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += ip;                    // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br> <br>";

  htmlPage += "MQTT Server      : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += mqtt_server;           // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br>";                // "<br>" erschafft eine Leerzeile (bzw. definiert das Ende einer Zeile)

  htmlPage += "MQTT Port        : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += mqtt_port;             // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br>";                // "<br>" erschafft eine Leerzeile (bzw. definiert das Ende einer Zeile)

  htmlPage += "MQTT Client Name : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += MQTT_CLIENT_NAME;      // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br>";

  htmlPage += "MQTT Client : ";
  if (!client.connected())
  {
    htmlPage += "NOT ";
  }
  htmlPage += "connected ";
  htmlPage += "<br> <br>";

  htmlPage += "Relais status        : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += RelaisState;               // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br> <br>";

  htmlPage += "Wassermenge       : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += volume;                 // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br> <br>";

  htmlPage += "timer            : "; // Damit wir auf unserer Website später auch etwas ablesen können, müssen wir diese Füllen.
  htmlPage += millis();              // Dies erreichen wir mit dem Befehl "client.println" , ähnlich wie "Serial.println"
  htmlPage += "<br>";

  htmlPage += F("</html>\r\n");
  return htmlPage;
}

void webServer(void)
{
  // WiFiClient client = server.available();

  WiFiClient client = server.accept();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
      }
    }

    while (client.available())
    {
      // but first, let client finish its request
      // that's diplomatic compliance to protocols
      // (and otherwise some clients may complain, like curl)
      // (that is an example, prefer using a proper webserver library)
      client.read();
    }

    // close the connection:
    client.stop();
    Serial.println("[Client disconnected]");
  }
}

void wifiManagerSetup(void)
{

  // clean FS, for testing
  // LittleFS.format();

  // read configuration from FS json
  Serial.println("mounting FS...");

  if (LittleFS.begin())
  {
    Serial.println("mounted file system");
    if (LittleFS.exists("/config.json"))
    {
      // file exists, reading and loading
      Serial.println("reading config file");
      File configFile = LittleFS.open("/config.json", "r");
      if (configFile)
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success())
        {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
        }
        else
        {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  }
  else
  {
    Serial.println("failed to mount FS");
  }
  // end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  // set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  // set static ip
  // wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);

  // reset settings - for testing
  // wifiManager.resetSettings();

  // set minimu quality of signal so it ignores AP's under that quality
  // defaults to 8%
  // wifiManager.setMinimumSignalQuality();

  // sets timeout until configuration portal gets turned off
  // useful to make it all retry or go to sleep
  // in seconds
  wifiManager.setTimeout(120);

  // fetches ssid and pass and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  // if (!wifiManager.autoConnect("WEMOS D1", "password")) {
  if (!wifiManager.autoConnect(WifiApName))
  {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    // reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  // if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  // read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());

  // save the custom parameters to FS
  if (shouldSaveConfig)
  {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;

    File configFile = LittleFS.open("/config.json", "w");
    if (!configFile)
    {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    // end save
  }
}

void mqttCheck(void)
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

void mqttStartup(void)
{

  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(MQTTcallback);
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

#endif /* WEBSERVER_H_ */