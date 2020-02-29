#include "Arduino.h"
#include "user_interface.h"
#include <IOThing.h>
#include <TickerScheduler.h>
#include <GrowattInverter.h>

// Either define your WIFI and MQTT settings here, or:
#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"
#define MQTT_SERVER "your-mqtt-server-ip"
#define MQTT_HOSTNAME "growatt"

// Put your wifi and mqtt settings in settings.h to avoid them being committed to Git Hub
#include "settings.h"
