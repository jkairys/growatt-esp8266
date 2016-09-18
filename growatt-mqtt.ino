
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "sneakypreen";
const char* password = "gumboots";
const char* mqtt_server = "192.168.0.3";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;










#include <ModbusMaster.h>


#include <ArduinoOTA.h>

#define LED 16

// instantiate ModbusMaster object
ModbusMaster node;


void setup(){
  // use Serial (port 0); initialize Modbus communication baud rate
  Serial.begin(9600);
  Serial.swap();
  
  // communicate with Modbus slave ID 2 over Serial (port 0)
  node.begin(1, Serial);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);
  
  digitalWrite(LED, HIGH);

  ArduinoOTA.begin();
  
}






void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("GrowattInverter")) {
      //Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("inverter/status", "init");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


float glueFloat(unsigned int d1, unsigned int d0){
  unsigned long t;
  t = d1 << 16;
  t += d0;

  float f;
  f = t;
  f = f / 10;
  return f;
}

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
  client.publish("inverter/status", buf, true);
  
}

void publishFloat(char * topic, float f){
  String value_str = String(f, 1);
  char value_char[32] = "";
  value_str.toCharArray(value_char, 40);

  String topic_str = "inverter/" + String(topic);
  char topic_char[128] = "";
  topic_str.toCharArray(topic_char, 128);
  
  client.publish(topic_char, value_char, true);
}

unsigned long next_poll = 0;
void loop()
{
  static uint32_t i;
  uint8_t j, result;
  uint16_t data[6];
  
  i++;

  String tmp;
  char topic[40] = "";
  char value[40] = "";


  if(millis() > next_poll){
    result = node.readInputRegisters(0, 33);
    // do something with data if read is successful
    if (result == node.ku8MBSuccess){
      /*
      for (j = 0; j < 33; j++){
        tmp = String("inverter/data/" + String(j));
        tmp.toCharArray(topic, 40);
        tmp = String(node.getResponseBuffer(j), DEC);
        tmp.toCharArray(value, 40);
        client.publish(topic, value);
      }*/

      publishStatus(node.getResponseBuffer(0));
      
      publishFloat("Ppv", glueFloat(node.getResponseBuffer(1), node.getResponseBuffer(2)));    
      publishFloat("Vpv1", glueFloat(0, node.getResponseBuffer(3)));    
      publishFloat("PV1Curr", glueFloat(0, node.getResponseBuffer(4)));    
      publishFloat("Pac", glueFloat(node.getResponseBuffer(11), node.getResponseBuffer(12)));
      publishFloat("Fac", glueFloat(0, node.getResponseBuffer(13))/10 );  

      publishFloat("Vac1", glueFloat(0, node.getResponseBuffer(14)));  
      publishFloat("Iac1", glueFloat(0, node.getResponseBuffer(15)));
      publishFloat("Pac1", glueFloat(node.getResponseBuffer(16), node.getResponseBuffer(17)));

      publishFloat("Etoday", glueFloat(node.getResponseBuffer(26), node.getResponseBuffer(27)));
      publishFloat("Etotal", glueFloat(node.getResponseBuffer(28), node.getResponseBuffer(29)));
      publishFloat("ttotal", glueFloat(node.getResponseBuffer(30), node.getResponseBuffer(31)));
      publishFloat("Tinverter", glueFloat(0, node.getResponseBuffer(32)));
    }else{
      tmp = String(result, HEX);
      tmp.toCharArray(value, 40);
      client.publish("inverter/error", value);
    }
    next_poll = millis() + 10000;
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  ArduinoOTA.handle();
  // set word 0 of TX buffer to least-sign;
  
  
}
