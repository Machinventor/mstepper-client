#ifndef MStepperClient_H
#define MStepperClient_H

#include <Arduino.h>

#define RES_ERR_TIMEDOUT 0
#define RES_SUCCESS 1
#define ERR_POS_NOT_SPECIFIED 2
#define ERR_SPEED_NOT_SPECIFIED 3
#define ERR_VALUE_SHOULD_BE_POSITIVE 4
#define ERR_ACCEL_NOT_SPECIFIED 5
#define ERR_VALUE_NOT_SPECIFIED 6
#define ERR_MICROSTEP_OUTBOUNDARY 7
#define ERR_INVALID_I2C_ADDR 8

#define OPTS_SYNC   (1 << 0)
#define OPTS_SOFT   (1 << 1)
#define OPTS_HARD   (1 << 2)

class MStepperClient
{
  public:

    virtual int     move(long pos , long speed , int opts) = 0;
    virtual int     moveInfinitely(int dir , long speed) = 0;
    virtual int     jog(long magnitude , long speed , int opts) = 0;
    virtual int     stop(int opts = 0) = 0;
    virtual int     setAccel(long accel) = 0;
    virtual int     setPos(long pos) = 0;
    virtual int     setMicrostepping(int microstep) = 0;
    virtual int     setI2cAddress(int addr) = 0;
    virtual int     hardReset() = 0;
    virtual long    position() = 0;
    virtual bool    isRunning() = 0;
    virtual bool    connect(long timeout) = 0;

  protected:

    static char *generateMoveCmd(long pos, long speed, int opts = 0);
    static char *generateJogCmd(long magnitude, long speed, int opts = 0);
    static char *generateStopCmd(int opts = 0);
    static char *generateGetPosCmd();
    static char *generateSetAccelCmd(long accel);
    static char *generateSetPosCmd(long pos);
    static char *generateSetMicrostepCmd(int microstep);
    static char *generateSetI2CAddress(int addr);
    static char *generateIsRunningCmd();
    static char *generateRetrieveSettingsCmd();
    static char *generateResetCmd(int opts = 0);
    static char *generateCheckCmd();
    static char *generateBuffer();
    static void  appendOptions(char* buffer , int opts);
    static void  whitespace(char* str);
    static void  terminate(char* str);

};

#endif