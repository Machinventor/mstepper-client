#include <Arduino.h>
#include <SoftwareSerial.h>
#include <UartStepperClient.h>
#include <I2CStepperClient.h>
#include <MemoryFree.h>

SoftwareSerial serial(9, 10);
UartStepperClient client(&serial);
I2CStepperClient i2cClient(8);

void setup()
{
    // put your setup code here, to run once:

    Serial.begin(9600);
    serial.begin(9600);

    while(serial.available())
        serial.read();

    // //Reset mcu
    // client.hardReset();

    // //Connecting mstepper
    // if(client.connect(-1)){
    //     Serial.println(F("Connection established."));
    // } else {
    //     Serial.println(F("Baglanti failed."));
    // }

    if(i2cClient.connect()){
        Serial.println("I2C Connection established");
    }

    // i2cClient.hardReset();
    i2cClient.setAccel(25000);
}

void loop()
{
    // i2cClient.moveInfinitely(1 , 14000);    // delay(500);
    // i2cClient.move(2000 , 1500 , OPTS_SYNC);
    // i2cClient.move(2000 , 1500 , OPTS_SYNC);
    // i2cClient.move(-2000 , 1500 , OPTS_SYNC);
    // i2cClient.move(-2000 , 1500 , OPTS_SYNC);
    i2cClient.move(4312 , 9444 , OPTS_SYNC);
    i2cClient.move(7800 , 9999 , OPTS_SYNC);
    i2cClient.move(8912 , 3000 , OPTS_SYNC);
    i2cClient.move(12012 , 12000 , OPTS_SYNC);
    i2cClient.move(11012 , 3000 , OPTS_SYNC);
    i2cClient.move(6012 , 3000 , OPTS_SYNC);
    // while(1)  { }
    // i2cClient.jog(-20000 , 10000 , OPTS_SYNC);
    // i2cClient.move(8312 , 3455 , OPTS_SYNC);
}