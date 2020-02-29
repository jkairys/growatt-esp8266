#include "main.h"


#define PIN_LED_WIFI D2
#define PIN_LED_COMM D1
#define PIN_TX D5
#define PIN_RX D6
#define SLAVE_ADDRESS 1

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_SERVER;

GrowattInverter inverter(PIN_RX, PIN_TX, SLAVE_ADDRESS);
TickerScheduler ts(5);
IOThing iot(MQTT_HOSTNAME);

void publishStatus(uint16_t st){
  char buf[16];
  
  String str = "unknown";
  switch(st){
    case 0:
      str = "waiting";
      break;
    case 1:
      str = "normal";
      break;
    case 3: 
      str = "fault";
      break;
  }
  str.toCharArray(buf, 16);
  iot.publish("inverter/status", buf, false);
}

void hmi() {
  digitalWrite(PIN_LED_WIFI, iot.wifiConnected() ? 1 : 0);
  digitalWrite(PIN_LED_COMM, inverter.valid ? 1 : 0);
}

void report() {
  publishStatus(inverter.status);
  iot.publish("Ppv", inverter.Ppv);    
  iot.publish("Vpv1", inverter.Vpv1);
  iot.publish("Ipv1", inverter.Ipv1);
  iot.publish("Pac", inverter.Pac);
  iot.publish("Fac", inverter.Fac);
  iot.publish("Vac1", inverter.Vac1);
  iot.publish("Iac1", inverter.Iac1);
  iot.publish("Pac1", inverter.Pac1);
  iot.publish("Etoday", inverter.Etoday);
  iot.publish("Etotal", inverter.Etotal);
  iot.publish("ttotal", inverter.Etoday);
  iot.publish("Tinverter", inverter.Etotal);
}

void setup(){
  Serial.begin(9600);
  pinMode(PIN_LED_COMM, OUTPUT);
  pinMode(PIN_LED_WIFI, OUTPUT);
  digitalWrite(PIN_LED_COMM, 0);
  digitalWrite(PIN_LED_WIFI, 0);

  ts.add(0, 1000, [&](void *){
    if(!iot.wifiConnected()) return;
    Serial.println("Reading inverter");
    inverter.read();
  }, NULL, true);

  ts.add(1, 2000, [&](void *){
    if(!iot.wifiConnected() || !inverter.valid) return;
    Serial.println("Sending report");
    report();
  }, NULL, false);

  ts.add(2, 500, [&](void *){
    Serial.println("Updating HMI");
    hmi();
  }, NULL, false);

  Serial.println("Configuring WiFi");
  iot.useWiFi(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Configuring OTA");
  iot.useOTA();

  Serial.println("Configuring MQTT");
  iot.useMQTT(MQTT_SERVER, [](String topic, String message){
    Serial.println("Received setting: " + topic + " = " + message);
  });

  Serial.println("Starting loop");

}

void loop(){
  iot.loop();
  ts.update();
}