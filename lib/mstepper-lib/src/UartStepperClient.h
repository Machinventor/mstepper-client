#ifndef UART_STEPPER_CLIENT_H
#define UART_STEPPER_CLIENT_H

#include "MStepperClient.h"
#include <Arduino.h>


#define DEFAULT_READ_LINE_TIMEOUT 100

class UartStepperClient : public MStepperClient
{

public:

  UartStepperClient(Stream *serial);
  virtual int move(long pos, long speed, int opts = 0);
  virtual int moveInfinitely(int dir , long speed);
  virtual int jog(long magnitude, long speed, int opts = 0);
  virtual int stop(int opts = 0);
  virtual int setPos(long pos);
  virtual int setAccel(long accel);
  virtual int setMicrostepping(int microstep);
  virtual int setI2cAddress(int addr);
  virtual int hardReset();
  virtual int moveReference();
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
  bool   retrieveSettings();

  int   m_microstepping;
  int   m_i2cAddr;
  long  m_acceleration;

  static char m_buffer[50];
  static int m_idx;
};

#endif