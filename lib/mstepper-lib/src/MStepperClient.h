#ifndef MStepperClient_H
#define MStepperClient_H

#include <Arduino.h>

#define OPTS_SYNC   (1 << 0)
#define OPTS_SOFT   (1 << 1)
#define OPTS_HARD   (1 << 2)

class MStepperClient
{
  public:

    virtual int     move(long pos , long speed , int opts) = 0;
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

//   private:

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