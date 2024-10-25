// MQTT settings
#define MQTT_SERVER         "192.168.0.192"
#define MQTT_SERVER_TEST    "192.168.1.0"
#define MQTT_PORT           "1883"

// WIFI settings
#define WIFI_AP_NAME        "Wifi_WATERING"
#define MQTT_CLIENT_NAME    "Wifi_MQTT_WATERING_SYSTEM"
#define OTA_CLIENT_NAME     "Wifi_OTA_WATERING_SYSTEM"

#define DEVICE_NAME         "ESP32 - Watering System"
#define SW_BASE_VERSION     "V1.1"

// Parameters 

#define LOWBATTERY      10.0

// Pin definition

// Inputs
#define PIN_SENSOR      16 
#define PIN_BUTTON_1    19
#define PIN_BUTTON_2    18
#define PIN_BUTTON_3    17

#define PIN_LDR         25
#define PIN_DS18B20     33
#define PIN_UIN         32

#define PIN_IN1         34
#define PIN_IN2         39
#define PIN_IN3         36

//Outputs
#define PIN_BUZZER      23
#define PIN_LED_int     2 
#define PIN_LED_ext     4

#define PIN_RELAY1      13      // on?
#define PIN_RELAY2      27      // off?
#define PIN_RELAY3      26      // Saftey Switch


