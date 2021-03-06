#include "UartStepperClient.h"
#include <Stream.h>
#include <stdlib.h>
#include <string.h>

char UartStepperClient::m_buffer[50];
int UartStepperClient::m_idx = 0;

UartStepperClient::UartStepperClient(Stream *serial)
    : 
    m_serial(serial) ,
    m_microstepping(0) ,
    m_i2cAddr(0) ,
    m_acceleration(0)
{
}

bool UartStepperClient::connect(long timeout)
{
    unsigned long current = millis();

    if(internalConnect()){

        while(!retrieveSettings()){ 
            if(timeout >= 0 && millis() - current < timeout){
                return false;
            }
        }
        return true;
    }

    char* cmd = generateResetCmd(OPTS_HARD);
    bool connected = false;

    //Baglanti saglanana kadar, bekle
    while(1){
        write(cmd);
        delay(1000);
        if(internalConnect()){
            connected = true;
            break;
        }

        if(timeout >= 0 && millis() - current < timeout){
            connected = false;
            break;
        }
    }

    //Clear memory
    free(cmd);

    while(!retrieveSettings()){ 
        if(timeout >= 0 && millis() - current < timeout){
            connected = false;
            break;
        }
    }

    return connected;
}

int UartStepperClient::move(long pos, long speed, int opts)
{

    writeAndClear(generateMoveCmd(pos, speed, opts));

    int timeout = 10;

    if (opts & OPTS_SYNC)
    {
        timeout = (abs(pos) / (float)speed) * 10000;
    }

    if (!readLine(timeout))
    {
        return RES_ERR_TIMEDOUT;
    }

    return extractResponseCode(m_buffer);
}

int UartStepperClient::moveInfinitely(int dir , long speed)
{
    long num = 2000000000L;

    move(num , speed);
}

int UartStepperClient::jog(long magnitude, long speed, int opts)
{

    writeAndClear(generateJogCmd(magnitude, speed, opts));

    long timeout = DEFAULT_READ_LINE_TIMEOUT;

    if (opts & OPTS_SYNC)
    {
        timeout = (abs(magnitude) / (float)speed) * 10000;
    }

    if (!readLine(timeout))
    {
        return RES_ERR_TIMEDOUT;
    }

    return extractResponseCode(m_buffer);
}

int UartStepperClient::moveReference(){
    writeAndClear(generateMoveReferenceCmd());

    if(!readLine(-1)){
        return RES_ERR_TIMEDOUT;
    }

    return extractResponseCode(m_buffer);
}

int UartStepperClient::stop(int opts)
{
    writeAndClear(generateStopCmd(opts));

    long timeout = 20;

    if (opts & OPTS_SYNC && !(opts & OPTS_SOFT))
    {
        timeout = 40000;
    }

    if (!readLine())
    {
        return RES_ERR_TIMEDOUT;
    }

    return extractResponseCode(m_buffer);
}

int UartStepperClient::setAccel(long accel)
{
    //If accel is not valid
    if(accel < 0)
        return ERR_VALUE_SHOULD_BE_POSITIVE;

    //If accel is same
    if(accel == m_acceleration){
        return RES_SUCCESS;
    }

    writeAndClear(generateSetAccelCmd(accel));

    //If readline timedout
    if(!readLine(DEFAULT_READ_LINE_TIMEOUT)){
        return RES_ERR_TIMEDOUT;
    }

    int code = extractResponseCode(m_buffer);

    //Update settings
    if(code == RES_SUCCESS){
        retrieveSettings();
    }

    return code;
}

int UartStepperClient::setPos(long pos)
{
    writeAndClear(generateSetPosCmd(pos));

    //If readline timedout
    if(!readLine(DEFAULT_READ_LINE_TIMEOUT)){
        return RES_ERR_TIMEDOUT;
    }

    return extractResponseCode(m_buffer);
}

int UartStepperClient::setMicrostepping(int microstep)
{
    //If microstepping value is not valid
    if(microstep < 0)
        return ERR_VALUE_SHOULD_BE_POSITIVE;
    else if(microstep > 4)
        return ERR_MICROSTEP_OUTBOUNDARY;

    //If microstepping is same
    if(microstep == m_microstepping){
        return RES_SUCCESS;
    }

    writeAndClear(generateSetMicrostepCmd(microstep));

    //Read result
    if(!readLine(DEFAULT_READ_LINE_TIMEOUT)){
        return RES_ERR_TIMEDOUT;
    }

    int code = extractResponseCode(m_buffer);
    
    //Update settings
    if(code == RES_SUCCESS){
        retrieveSettings();
    }

    return code;
}

int UartStepperClient::setI2cAddress(int addr)
{
    //Check validity
    if(addr < 0 || addr > 255)
        return ERR_INVALID_I2C_ADDR;

    //If microstepping is same
    if(addr == m_i2cAddr){
        return RES_SUCCESS;
    }

    writeAndClear(generateSetI2CAddress(addr));

    //If there is no response
    if(!readLine(DEFAULT_READ_LINE_TIMEOUT)){
        return RES_ERR_TIMEDOUT;
    }

    int code = extractResponseCode(m_buffer);
    
    //Update settings
    if(code == RES_SUCCESS){
        retrieveSettings();
    }
    return code;
}

bool UartStepperClient::isRunning()
{
    //Send command to mstepper
    writeAndClear(generateIsRunningCmd());
    if(!readLine(DEFAULT_READ_LINE_TIMEOUT)){
        return false;
    }

    if(strncmp_P(m_buffer , PSTR("no") , 2) == 0){
        return false;
    }
    else if(strncmp_P(m_buffer , PSTR("yes") , 3) == 0){
        return true;
    }

    return false;
}

void UartStepperClient::write(char *buffer)
{
    int size = strlen(buffer);
    Serial.print("Client::write(char*)\t");
    for (int i = 0; i < size; i++)
    {
        m_serial->write(buffer[i]);
        Serial.write(buffer[i]);
    }
}

bool UartStepperClient::readLine(long timeout)
{

    unsigned long start = millis();
    while (true)
    {
        while (m_serial->available())
        {
            int b = m_serial->read();
            if (b == '\n')
            {
                m_buffer[m_idx] = '\0';
                m_idx = 0;
                return true;
            }
            else
            {
                m_buffer[m_idx] = b;
                m_idx++;
            }
        }

        if (timeout >= 0 && (millis() - start) > ((unsigned long)timeout))
        {
            return false;
        }
    }

    return false;
}

bool UartStepperClient::hasError(char *str)
{
    return (strstr_P(str, PSTR("ERR:")) != NULL);
}

int UartStepperClient::readErrorCode(char *str)
{
    char numStr[3];
    int len = strlen(str);
    memcpy(numStr, &str[4], len - 3);
    numStr[len] = '\0';
    int num = atoi(numStr);

    return num;
}

long UartStepperClient::position(){
    writeAndClear(generateGetPosCmd());

    //If timedout
    if(!readLine(DEFAULT_READ_LINE_TIMEOUT)){
        return RES_ERR_TIMEDOUT;
    }

    char numStr[12];
    char *dup, *dup_orig;
    dup = dup_orig = strdup(m_buffer);
    int len = strlen(dup);
    memcpy(numStr, &dup[4], len - 3);
    numStr[len] = '\0';
    // Serial.println(numStr);
    long pos = atol(numStr);
    free(dup_orig);
    return pos;
}

bool UartStepperClient::succeeded(char *str)
{
    return (strstr_P(str, PSTR(";")) != NULL);
}

bool UartStepperClient::internalConnect(){

    char *cmd = generateCheckCmd();

    write(cmd);

        if (!readLine(1000))
        {
            return false;
        }

        if (succeeded(m_buffer))
        {
            return true;
        }
    
    while(m_serial->available()){
        if (!readLine(1000))
        {
            continue;
        }

        if (succeeded(m_buffer))
        {
            return true;
        }
    }

    return false;
}

void UartStepperClient::writeAndClear(char* cmd)
{
    //Writing command
    write(cmd);
    //Free command
    free(cmd);
}

int UartStepperClient::extractResponseCode(char* str){
    char *dup, *dup_org;

    //Duplicates buffer to prevent corruption
    dup_org = dup = strdup(str);

    int resCode = RES_ERR_TIMEDOUT;

    if (succeeded(dup))
    {
        resCode = RES_SUCCESS;
    }
    else if (hasError(dup))
    {
        resCode = readErrorCode(dup);
    }

    free(dup_org);

    return resCode;
}

int UartStepperClient::hardReset(){
    writeAndClear(generateResetCmd(OPTS_HARD));

    return connect();
}

bool UartStepperClient::retrieveSettings(){
    writeAndClear(generateRetrieveSettingsCmd());

    if(!readLine(DEFAULT_READ_LINE_TIMEOUT)){
        return false;
    }

    bool accelReceived , i2cReceived , microstepReceived;
    accelReceived = i2cReceived = microstepReceived = false;

    do {
        char *dup, *dup_orig;
        dup = dup_orig = strdup(m_buffer);
        if(strncmp_P(dup , PSTR("accel") , 5) == 0){
            char accelStr[12];
            memcpy(accelStr, &dup[6], strlen(dup) - 5);
            m_acceleration = atol(accelStr);
            accelReceived = true;
        } else if(strncmp_P(m_buffer , PSTR("i2cAddr") , 7) == 0) {
            char i2cAddrStr[8];
            memcpy(i2cAddrStr , &dup[8] , strlen(dup) - 7);
            m_i2cAddr = atoi(i2cAddrStr);
            i2cReceived = true;
        } else if(strncmp_P(m_buffer , PSTR("microstepping") , 13) == 0) {
            char microsteppingStr[8];
            memcpy(microsteppingStr , &dup[14] , strlen(dup) - 13);
            m_microstepping = atoi(microsteppingStr);
            microstepReceived = true;
        }

        free(dup_orig);
    } while(readLine(DEFAULT_READ_LINE_TIMEOUT) != RES_ERR_TIMEDOUT);

    return accelReceived && i2cReceived && microstepReceived;
}