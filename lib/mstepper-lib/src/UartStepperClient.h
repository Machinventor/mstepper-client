#ifndef UART_STEPPER_CLIENT_H
#define UART_STEPPER_CLIENT_H

#include "MStepperClient.h"
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

#define DEFAULT_READ_LINE_TIMEOUT 100

class UartStepperClient : public MStepperClient
{
public:
  UartStepperClient(Stream *serial);
  virtual int move(long pos, long speed, int opts = 0);
  virtual int jog(long magnitude, long speed, int opts = 0);
  virtual int stop(int opts = 0);
  virtual int setAccel(long accel);
  virtual int setPos(long pos);
  virtual int setMicrostepping(int microstep);
  virtual int setI2cAddress(int addr);
  virtual int hardReset();
  virtual long position();
  virtual bool isRunning();
  virtual bool connect(long timeout = -1);

private:
  Stream *m_serial;

  void  write(char *buffer);
  bool  readLine(long timeout = -1L);
  bool  hasError(char *str);
  bool  succeeded(char *str);
  int   readErrorCode(char *str);
  bool  internalConnect();
  void  writeAndClear(char* str);
  int   extractResponseCode(char* str);

  static char m_buffer[50];
  static int m_idx;
};

#endif