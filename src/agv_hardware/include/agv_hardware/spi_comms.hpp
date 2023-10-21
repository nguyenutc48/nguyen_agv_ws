#ifndef AGV_HARDWARE_SPI_COMMS_HPP
#define AGV_HARDWARE_SPI_COMMS_HPP

// #include <cstring>
#include <sstream>
// #include <cstdlib>
#include <wiringPi.h>
#include <iostream>

#define g_pi_LAT_MT1 12
#define g_pi_LAT_MT2 6
#define g_pi_SCK_ANALOG  4
#define g_pi_DATA_ANALOG  5
static      bool        _g_bInitial = false;
static      bool        _g_bGPIOState[230];
static      int         _g_nAddressMax = 227;



class SPIComms
{

public:

  SPIComms() = default;

  void setup(){
    wiringPiSetup();
    digitalWrite(g_pi_LAT_MT1, false);
    digitalWrite(g_pi_LAT_MT2, false);
  }

void WriteDAC(bool bDriverWheel, int nSpeed)
{
    if(nSpeed >= 0)
    {
        int nPinAddress = 0;
        if(bDriverWheel == true)
        {
            nPinAddress = g_pi_LAT_MT1;
        }
        else /*if(bDriverWheel == Right_Wheel)*/
        {
            nPinAddress = g_pi_LAT_MT2;
        }
        // initiate data transfer with 4921
        SetOutput(nPinAddress,false);
        // send 4 bit header
        SetSendSPIHeader();
        // send data
        for(int i = 11 ; i >= 0; i--)
        {
            SetOutput(g_pi_DATA_ANALOG, ((nSpeed & (1<<i))) >> i);
            SetSendSPIClock();
        }
        // finish data transfer
        SetOutput(nPinAddress,true);
    }
}
void  SetSendSPIClock()
{
    SetOutput(g_pi_SCK_ANALOG,true);
    SetOutput(g_pi_SCK_ANALOG,false);
}
bool  SetOutput(int nPinAddress, bool bON)
{
    if(!_g_bInitial || abs(nPinAddress) > _g_nAddressMax)
    {
        return false;
    }
    else
    {
        digitalWrite(abs(nPinAddress),bON);
        _g_bGPIOState[abs(nPinAddress)] = bON;
        return true;
    }
}
void    SetSendSPIHeader()
{
    // bit 15
    // 0 write to DAC *
    // 1 ignore command
    SetOutput(g_pi_DATA_ANALOG,false);
    SetSendSPIClock();
    // bit 14 Vref input buffer control
    // 0 unbuffered *
    // 1 buffered
    SetOutput(g_pi_DATA_ANALOG,false);
    SetSendSPIClock();
    // bit 13 Output Gain selection
    // 0 2x
    // 1 1x *
    SetOutput(g_pi_DATA_ANALOG,true);
    SetSendSPIClock();
    // bit 12 Output shutdown control bit
    // 0 Shutdown the device
    // 1 Active mode operation *
    SetOutput(g_pi_DATA_ANALOG,true);
    SetSendSPIClock();
}


private:

};

#endif // DIFFDRIVE_ARDUINO_ARDUINO_COMMS_HPP