#ifndef I2C_STEPPER_CLIENT_H
#define I2C_STEPPER_CLIENT_H

#include "MStepperClient.h"

class I2CStepperClient : public MStepperClient
{
    public:

        I2CStepperClient(int i2cAddr);
        virtual int     move(long pos , long speed , int opts = 0);
        virtual int     moveInfinitely(int dir , long speed = 0);
        virtual int     jog(long magnitude , long speed , int opts);
        virtual int     stop(int opts);
        virtual int     setAccel(long accel);
        virtual int     setPos(long pos);
        virtual int     setMicrostepping(int microstep);
        virtual int     setI2cAddress(int addr);
        virtual int     hardReset();
        virtual int     moveReference();
        virtual int     setSpeed(long speed);
        virtual long    position();
        virtual bool    isRunning();
        virtual bool    connect(long timeout = -1);
        void    update();
        bool    waitForMotionEnd(long timeout = -1);

    private:

        void    writeAndClear(char* str);
        void    write(char* str);
        int     m_i2cAddr;
        bool    m_running;
        long    m_position;
};

#endif