# growatt-esp8266
A library to monitor a Growatt MTL-5000 using an ESP8266 (Wemos D1) via a modbus over RS232 interface.

## Software
The library uses a software serial port to connect to the inverter to retrieve data using its Modbus interface. Modbus provides a set of predefined parameters available at certain addresses, which can be queried using a "read register" commmand. This library will:
* Create a software serial port on the pins you specify
* Create a modbus master using this serial port
* Read data from the modbus client (the inverter) registers, decode it and make it available via the library

### Using the library
```
#include <GrowattInverter.h>
#define PIN_TX D5
#define PIN_RX D6
#define SLAVE_ADDRESS 1
GrowattInverter inverter(PIN_RX, PIN_TX, SLAVE_ADDRESS);

void setup(){
  Serial.begin(9600);
}

void loop(){
  inverter.read();
  if(inverter.valid){
    // Power output from the PV panels
    Serial.println(inverter.Ppv);
    // Power output from the inverter
    Serial.println(inverter.Pac1);
  }else{
    Serial.println("Inverter read failed");
  }
  delay(1000);
}

```

## Hardware
See the `hardware/` directory for EAGLE schematic and circuit board design. 
To simplify board design and fabrication, the board uses a module for the esp8266 and all through hole components beyond this.

The board itself is comprised of:
* Wemos D1 Mini
* LM7805 voltage regulator
* 1x330uF capacitor (for main power)
* MAX232 level converter
* 3x 1uF capacitors (for MAX232)
* 3x 3mm leds + 1/4W resistors (optional)

### Pinout
The growatt inverter I have has a DB9 connector for an RS232 connection, which has the pinout:
```
    FROM INVERTER
-----------------------   2 = TX
\ (5) (4) (3) (2) (1) /   3 = RX
 \  (9) (8) (7) (6)  /    5 = GND
  \_________________/     9 = V+ (about 8 volts)


  Inverter DB9           Our DB9       PCB         Wemos
    (female)              (male)       (J4)        (pins)
       2 ---(TX)----(RX)--- 2 --------- 3 ---(RX)--- D6
       3 ---(RX)----(TX)--- 3 --------- 4 ---(TX)--- D5
       5 ---(GND)---------- 5 --------- 1 ---------- GND
       9 ---(V+)----------- 9 --------- 2 -- VREG -- VCC 

```

## Sample project
I've included a complete sample project using the board and software library if you just want to get this running on your own inverter. It uses an MQTT + WiFi wrapper library I put together a while ago called _iot-thing_ but you should be easily able to rip this out and use your own code for the internet / wifi connectivity side of things.

## Enclosure
Coming soon...

## Todo
* Example project should use self configuring captive portal for wifi setup
* Add a simple web UI to show present values from the inverter
* WiFi Serial debugging using socket