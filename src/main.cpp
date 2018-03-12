#include <Arduino.h>
#include <SoftwareSerial.h>
#include <UartStepperClient.h>
#include <MemoryFree.h>

SoftwareSerial serial(9, 10);
UartStepperClient client(&serial);

void setup()
{
    // put your setup code here, to run once:

    Serial.begin(9600);
    serial.begin(9600);

    while(serial.available())
        serial.read();

    //Reset mcu
    client.hardReset();

    //Connecting mstepper
    if(client.connect(-1)){
        Serial.println(F("Connection established."));
    } else {
        Serial.println(F("Baglanti failed."));
    }
}

void loop()
{
    client.jog(50000 , 12000);
    while(client.isRunning()){ }
    client.jog(-50000 , 15000);
    while(client.isRunning()) { }
}