#include "GrowattInverter.h"

// SoftwareSerial ser(D2, D3);

float GrowattInverter::glueFloat(uint16_t w1, uint16_t w0){
  unsigned long t;
  t = w1 << 16;
  t += w0;

  float f;
  f = t;
  f = f / 10;
  return f;
}

uint8_t GrowattInverter::read(){
  // this->ser->println("hello");
  uint8_t result = this->node->readInputRegisters(0, 33);
  // do something with data if read is successful

  if(result == this->node->ku8MBSuccess){
    /*
    for (j = 0; j < 33; j++){
      tmp = String("inverter/data/" + String(j));
      tmp.toCharArray(topic, 40);
      tmp = String(node.getResponseBuffer(j), DEC);
      tmp.toCharArray(value, 40);
      client.publish(topic, value);
    }*/

    // publishStatus(node.getResponseBuffer(0));
    this->valid = true;
    this->status = this->node->getResponseBuffer(0);
    this->Ppv = this->glueFloat(this->node->getResponseBuffer(1), this->node->getResponseBuffer(2));
    this->Vpv1 = this->glueFloat(0, this->node->getResponseBuffer(3));
    this->Ipv1 = this->glueFloat(0, this->node->getResponseBuffer(4));
    this->Pac = this->glueFloat(this->node->getResponseBuffer(11), this->node->getResponseBuffer(12));
    this->Fac = this->glueFloat(0, this->node->getResponseBuffer(13))/10;

    this->Vac1 = this->glueFloat(0, this->node->getResponseBuffer(14));
    this->Iac1 = this->glueFloat(0, this->node->getResponseBuffer(15));
    this->Pac1 = this->glueFloat(this->node->getResponseBuffer(16), this->node->getResponseBuffer(17));

    this->Etoday = this->glueFloat(this->node->getResponseBuffer(26), this->node->getResponseBuffer(27));
    this->Etotal = this->glueFloat(this->node->getResponseBuffer(28), this->node->getResponseBuffer(29));
    this->ttotal = this->glueFloat(this->node->getResponseBuffer(30), this->node->getResponseBuffer(31));
    this->Tinverter = this->glueFloat(0, this->node->getResponseBuffer(32));
  }else{
    String tmp = String(result, HEX);
    this->valid = false;
    // iot.publish("inverter/error", "1");
  }
}


GrowattInverter::GrowattInverter(uint8_t pinRx, uint8_t pinTx, uint8_t slaveAddress){
  // rx, tx, invert
  // this->ser.begin(pinRx, pinTx);
  // ser.begin(9600);
  this->ser = new SoftwareSerial(pinRx, pinTx);
  this->ser->begin(9600);
  this->node = new ModbusMaster();
  this->node->begin(slaveAddress, (Stream &)(*(this->ser)));
  this->valid = false;
  this->status = 0;
  this->Ppv1 = -1.0;
  this->Vpv1 = -1.0;
  this->Ipv1 = -1.0;

  this->Ppv2 = -1.0;
  this->Vpv2 = -1.0;
  this->Ipv2 = -1.0;

  this->Ppv3 = -1.0;
  this->Vpv3 = -1.0;
  this->Ipv3 = -1.0;

  this->Pac1 = -1.0;
  this->Vac1 = -1.0;
  this->Iac1 = -1.0;

  this->Pac2 = -1.0;
  this->Vac2 = -1.0;
  this->Iac2 = -1.0;

  this->Pac3 = -1.0;
  this->Vac3 = -1.0;
  this->Iac3 = -1.0;

  this->Etoday = -1.0;
  this->Etotal = -1.0;
  this->ttotal = -1.0;
  this->Tinverter = -1.0;

  this->Fac = -1.0;
  this->Pac = -1.0;
  this->Ppv = -1.0;
}
