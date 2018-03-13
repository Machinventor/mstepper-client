#include "I2CStepperClient.h"
#include <Wire.h>

I2CStepperClient::I2CStepperClient(int i2cAddr) 
: 
    m_i2cAddr(i2cAddr) ,
    m_running(false) ,
    m_position(0)
{   }

int I2CStepperClient::move(long pos , long speed , int opts)
{

    bool sync = false;

    if(opts & OPTS_SYNC)
        sync = true;

    //Remove sync options
    //It will be handled with hacky way
    opts &= ~(OPTS_SYNC);

    writeAndClear(generateMoveCmd(pos , speed , opts));

    if(sync){
        waitForMotionEnd();
    }

    return RES_SUCCESS;
}

int I2CStepperClient::moveInfinitely(int dir , long speed)
{
    long num = 2000000000L;

    move(num , speed);

    return RES_SUCCESS;
}

int I2CStepperClient::jog(long magnitude , long speed , int opts)
{
    bool sync = false;
    if(opts & OPTS_SYNC){
        sync = true;
    }

    //Remove sync options
    opts &= ~(OPTS_SYNC);

    writeAndClear(generateJogCmd(magnitude , speed , opts));

    //If sync required
    if(sync){
        //Waitign for motion finish
        waitForMotionEnd();
    }

    return RES_SUCCESS;
}

int I2CStepperClient::stop(int opts)
{

    bool sync = false;

    //If sync option is specified
    if(opts & OPTS_SYNC)
        sync = true;

    //Remove sync options
    opts &= ~OPTS_SYNC;

    writeAndClear(generateStopCmd(opts));

    //If sync is specified
    //Wait for motion end
    if(sync){
        waitForMotionEnd();
    }

    return RES_SUCCESS;
}

int I2CStepperClient::setAccel(long accel)
{
    writeAndClear(generateSetAccelCmd(accel));
    return RES_SUCCESS;
}

int I2CStepperClient::setPos(long pos)
{   
    writeAndClear(generateSetPosCmd(pos));
    return RES_SUCCESS;
}

int I2CStepperClient::setMicrostepping(int microstep)
{   
    writeAndClear(generateSetMicrostepCmd(microstep));
    return RES_SUCCESS;
}

int I2CStepperClient::setI2cAddress(int addr)
{   
    return RES_ERR_TIMEDOUT;
}

int I2CStepperClient::hardReset(){
    writeAndClear(generateResetCmd(OPTS_HARD));
    delay(1000);
    return RES_SUCCESS;
}

long I2CStepperClient::position(){
    return m_position;
}

bool I2CStepperClient::isRunning(){
    return m_running;
}

bool I2CStepperClient::connect(long timeout)
{
    Wire.begin();
    update();
    return RES_SUCCESS;
}

void I2CStepperClient::writeAndClear(char* str)
{
    write(str);

    free(str);
}

void I2CStepperClient::write(char* str){
    Wire.beginTransmission(m_i2cAddr);
    int len = strlen(str);
    Serial.print("::write =>");
    for(int i = 0; i < len; i++){
        Wire.write(str[i]);
        Serial.write(str[i]);
    }

    Wire.endTransmission();
}

bool I2CStepperClient::waitForMotionEnd(long timeout){
    long start = millis();
    do{
        update();
        delay(1);
        //Check for timeout
        if(timeout >= 0 && millis() - start < timeout)
            return false;
    } while(isRunning());

    return true;
}

void I2CStepperClient::update(){
    Wire.requestFrom(m_i2cAddr , 5);

  union {
    byte b[4];
    long num;
  } data;

  if (Wire.available()) {
    uint8_t c = Wire.read();
    data.b[0] = Wire.read();
    data.b[1] = Wire.read();
    data.b[2] = Wire.read();
    data.b[3] = Wire.read();
    m_running = c > 0;
    m_position = data.num;
  }
}