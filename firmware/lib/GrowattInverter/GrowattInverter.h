#ifndef _GrowattInverter_h
#define _GrowattInverter_h

#include <Arduino.h>
#include <stdint.h>
#include <functional>
#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#define SLAVE_ADDRESS 1


class GrowattInverter
{
  private:

    //
    // publishFloat("Ppv", glueFloat(node.getResponseBuffer(1), node.getResponseBuffer(2)));
    // publishFloat("Vpv1", glueFloat(0, node.getResponseBuffer(3)));
    // publishFloat("PV1Curr", glueFloat(0, node.getResponseBuffer(4)));
    // publishFloat("Pac", glueFloat(node.getResponseBuffer(11), node.getResponseBuffer(12)));
    // publishFloat("Fac", glueFloat(0, node.getResponseBuffer(13))/10 );
    //
    // publishFloat("Vac1", glueFloat(0, node.getResponseBuffer(14)));
    // publishFloat("Iac1", glueFloat(0, node.getResponseBuffer(15)));
    // publishFloat("Pac1", glueFloat(node.getResponseBuffer(16), node.getResponseBuffer(17)));
    //
    // publishFloat("Etoday", glueFloat(node.getResponseBuffer(26), node.getResponseBuffer(27)));
    // publishFloat("Etotal", glueFloat(node.getResponseBuffer(28), node.getResponseBuffer(29)));
    // publishFloat("ttotal", glueFloat(node.getResponseBuffer(30), node.getResponseBuffer(31)));
    // publishFloat("Tinverter", glueFloat(0, node.getResponseBuffer(32)));
    float glueFloat(uint16_t w1, uint16_t w2);
    SoftwareSerial * ser;
    ModbusMaster * node;

  public:
    GrowattInverter(uint8_t txPin, uint8_t rxPin, uint8_t slaveAddress);
    uint8_t read();

    float Ppv1;
    float Vpv1;
    float Ipv1;

    float Ppv2;
    float Vpv2;
    float Ipv2;

    float Ppv3;
    float Vpv3;
    float Ipv3;

    float Pac1;
    float Vac1;
    float Iac1;

    float Pac2;
    float Vac2;
    float Iac2;

    float Pac3;
    float Vac3;
    float Iac3;

    float Etoday;
    float Etotal;
    float ttotal;
    float Tinverter;

    float Fac;
    float Pac;
    float Ppv;
    bool valid;

    uint8_t status;
};

#endif