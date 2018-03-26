#include <Arduino.h>
#include <SoftwareSerial.h>
#include <UartStepperClient.h>
#include <I2CStepperClient.h>
#include <MemoryFree.h>

SoftwareSerial serial(9, 10);
// UartStepperClient client(&serial);
I2CStepperClient client(8);

void setup()
{
    Serial.begin(9600);
    serial.begin(9600);

    while (serial.available())
        serial.read();

    if(client.connect()){
        Serial.println("Connection established.");
    }

    // client.setAccel(34000);
    // client.setI2cAddress(8);
    client.setMicrostepping(1);
    // client.setSpeed()
    // client.moveReference();
}

void loop()
{
    client.move(50000, 2544);
    delay(2000);
    client.setSpeed(7000);
    client.waitForMotionEnd();
    client.move(0 , 7000);
    delay(2500);
    client.setSpeed(2000);
    client.waitForMotionEnd();
    // client.move(7800, 3699, OPTS_SYNC);
    // client.move(8912, 4000, OPTS_SYNC);
    // client.move(50012, 10000, OPTS_SYNC);
    // client.move(11012, 5500, OPTS_SYNC);
    // client.move(7500 , 3000 , OPTS_SYNC);
}