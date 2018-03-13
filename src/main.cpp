#include <Arduino.h>
#include <SoftwareSerial.h>
#include <UartStepperClient.h>
// #include <I2CStepperClient.h>
#include <MemoryFree.h>

SoftwareSerial serial(9, 10);
UartStepperClient client(&serial);

void setup()
{
    Serial.begin(9600);
    serial.begin(9600);

    while (serial.available())
        serial.read();

    if(client.connect()){
        Serial.println("Connection established.");
    }

    client.setAccel(34000);
    client.setI2cAddress(8);
    client.setMicrostepping(0);
    client.moveReference();
}

void loop()
{
    client.move(4312, 6544 , OPTS_SYNC);
    client.move(7800, 6699, OPTS_SYNC);
    client.move(8912, 3000, OPTS_SYNC);
    client.move(14012, 7000, OPTS_SYNC);
    client.move(11012, 4500, OPTS_SYNC);
    client.move(7500 , 7000 , OPTS_SYNC);
    // client.move(0 , 8000 , OPTS_SYNC);
    client.moveReference();
}