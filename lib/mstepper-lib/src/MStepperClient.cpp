#include "MStepperClient.h"
#include <string.h>
#include <stdlib.h>

char *MStepperClient::generateMoveCmd(long pos, long speed, int opts)
{
    char *buffer = generateBuffer();

    char posBuffer[11];
    char speedBuffer[11];
    sprintf(posBuffer , "%ld" , pos);
    sprintf(speedBuffer , "%ld" , speed);

    strcat_P(buffer ,  PSTR("move "));
    appendOptions(buffer , opts);
    strcat(buffer , posBuffer);
    whitespace(buffer);
    strcat(buffer , speedBuffer);
    terminate(buffer);

    return buffer;
}

char *MStepperClient::generateJogCmd(long magnitude, long speed, int opts)
{
    char *buffer = generateBuffer();
    char posBuffer[11];
    char speedBuffer[11];

    sprintf(posBuffer , "%ld" , magnitude);
    sprintf(speedBuffer , "%ld" , speed);

    strcat_P(buffer , PSTR("jog "));
    appendOptions(buffer , opts);
    strcat(buffer , posBuffer);
    whitespace(buffer);
    strcat(buffer , speedBuffer);
    terminate(buffer);

    return buffer;
}

char *MStepperClient::generateStopCmd(int opts)
{
    char *buffer = generateBuffer();

    strcat_P(buffer , PSTR("stop "));
    appendOptions(buffer , opts);
    terminate(buffer);

    return buffer;
}

char* MStepperClient::generateGetPosCmd(){
    char* buffer = generateBuffer();

    strcat_P(buffer , PSTR("pos"));
    terminate(buffer);

    return buffer;
}

char *MStepperClient::generateSetAccelCmd(long accel)
{
    char *buffer = generateBuffer();
    char accelBuffer[12];
    strcat_P(buffer , PSTR("set --accel "));
    sprintf(accelBuffer , "%ld" , accel);
    strcat(buffer , accelBuffer);
    terminate(buffer);

    return buffer;
}

char *MStepperClient::generateSetPosCmd(long pos)
{
    char *buffer = generateBuffer();
    char posBuffer[12];

    strcat_P(buffer , PSTR("set --pos "));
    sprintf(posBuffer , "%ld" , pos);
    strcat(buffer , posBuffer);
    terminate(buffer);

    return buffer;
}

char *MStepperClient::generateIsRunningCmd()
{
    char *buffer = generateBuffer();

    strcat_P(buffer , PSTR("running\n"));

    return buffer;
}

char *MStepperClient::generateRetrieveSettingsCmd()
{
    char *buffer = generateBuffer();

    strcat_P(buffer , PSTR("settings --all\n"));

    return buffer;
}

char* MStepperClient::generateCheckCmd(){
    char* buffer = generateBuffer();

    strcat_P(buffer , PSTR("check"));
    terminate(buffer);

    return buffer;
}

char* MStepperClient::generateSetMicrostepCmd(int microstep){
    char* buffer = generateBuffer();
    char  microStepBuffer[12];

    strcat_P(buffer , PSTR("set --microstepping "));
    sprintf(microStepBuffer , "%d" , microstep);
    strcat(buffer , microStepBuffer);
    terminate(buffer);
    
    return buffer;
}

char* MStepperClient::generateSetI2CAddress(int addr){
    char* buffer = generateBuffer();
    char addrBuffer[12];
    strcat_P(buffer , PSTR("set --i2cAddress "));
    sprintf(addrBuffer , "%d" , addr);
    strcat(buffer , addrBuffer);
    terminate(buffer);

    return buffer;
}

char* MStepperClient::generateResetCmd(int opts){
    char* buffer = generateBuffer();
    //Append reset str
    strcat_P(buffer, PSTR("reset "));

    //Process options
    appendOptions(buffer , opts);

    //Terminate command
    terminate(buffer);

    return buffer;
}

char* MStepperClient::generateMoveReferenceCmd(){
    char* buffer = generateBuffer();

    strcat_P(buffer , PSTR("reference"));
    terminate(buffer);

    return buffer;
}

char* MStepperClient::generateSetSpeedCmd(long speed){
    //Generate buffer
    char* buffer = generateBuffer();
    char  speedBuffer[12];

    sprintf_P(speedBuffer , PSTR("%ld") , speed);
    strcat_P(buffer , PSTR("override --speed "));
    strcat(buffer , speedBuffer);
    terminate(buffer);
    return buffer;
}

char *MStepperClient::generateBuffer()
{
    char* c = (char *)malloc(sizeof(char) * 50);
    c[0] = '\0';
    return c;
}

void MStepperClient::whitespace(char* str){
    strcat_P(str, PSTR(" "));
}

void MStepperClient::terminate(char* str)
{
    strcat_P(str , PSTR("\n"));
}

void MStepperClient::appendOptions(char* str , int opts)
{
    if(opts & OPTS_SYNC){
        strcat_P(str , PSTR("--sync "));
    }

    if(opts & OPTS_SOFT){
        strcat_P(str , PSTR("--soft "));
    }

    if(opts & OPTS_HARD){
        strcat_P(str , PSTR("--hard "));
    }
}