#include <xc.h>
#include "MasterInitialization.h"
#include "IMU.h"

void i2cStart(void)
{
    I2C2CONbits.SEN = 1;       //initiate a START on the bus
    Nop();
    while(I2C2CONbits.SEN);    //wait for START to complete
}

void i2cRestart(void)
{
    I2C2CONbits.RSEN = 1;       //initiate a RESTART on the bus
    Nop();
    while(I2C2CONbits.RSEN);    //wait for RESTART to complete
}

void i2cStop(void)
{
    I2C2CONbits.PEN = 1;       //initiate a STOP on the bus
    Nop();
    while(I2C2CONbits.PEN);    //wait for STOP to complete
}

void i2cNACK(void)
{
    I2C2CONbits.ACKDT = 1;       //NACK is high
    Nop();
    I2C2CONbits.ACKEN = 1;       //initiate a NACK on the bus
    Nop();
    while(I2C2CONbits.ACKEN);    //wait for NACK to complete
}

void i2cACK(void)
{
    I2C2CONbits.ACKDT = 0;       //ACK is low
    Nop();
    I2C2CONbits.ACKEN = 1;       //initiate a ACK on the bus
    Nop();
    while(I2C2CONbits.ACKEN);    //wait for ACK to complete
}

void i2cWrite(unsigned char data)
{
    while(I2C2STATbits.TBF);        //wait for buffer to be available
    I2C2TRN = (unsigned int)data;   //send out data
    while(I2C2STATbits.TRSTAT);     //wait for data to be sent
    //could check that slave sent an ACK...
}

unsigned char getIMUStatus(void)
{
    unsigned char temp;
    
    i2cStart();
    i2cWrite(0x50);                 //IMU device "write" address
    i2cWrite(0x37);                 //IMU status register address
    i2cRestart();
    i2cWrite(0x51);                 //IMU device "read" address
    I2C2CONbits.RCEN = 1;          //initiate a READ
    while(!I2C2STATbits.RBF);       //wait for READ to complete
    temp = (unsigned char)I2C2RCV;  //get the data
    i2cNACK();
    i2cStop();
    return(temp);
    
}

void startIMU(void)
{

    i2cStart();
    i2cWrite(0x50);               //IMU device "write" address
    i2cWrite(0x55);               //IMU MagRate register address NOTE: register addresses auto-increment with additional writes
    i2cWrite(50);                 //IMU MagRate = 50Hz
    i2cWrite(20);                 //IMU AccelRate = 200Hz (register value is 1/10th of data rate)
    i2cWrite(20);                 //IMU GyroRate = 200Hz (register value is 1/10th of data rate)
    i2cNACK();
    i2cStop();

    i2cStart();
    i2cWrite(0x50);               //IMU device "write" address
    i2cWrite(0x32);               //IMU QRateDivisor register address
    i2cWrite(0x02);               //IMU Qrate = 100Hz (output rate is GyroRate/register value)
    i2cNACK();
    i2cStop();

    i2cStart();
    i2cWrite(0x50);               //IMU device "write" address
    i2cWrite(0x54);               //IMU AlgorithmControl register address
    i2cWrite(0x04);               //IMU disable standby state, Scaled sensor data, heading/pitch/roll output
    i2cNACK();
    i2cStop();

    //enabling Quaternion Result interrupt to scope output rate
    /*i2cStart();
    i2cWrite(0x50);               //IMU device "write" address
    i2cWrite(0x33);               //IMU EnableEvents register address
    i2cWrite(0x04);               //IMU enable Quaternion Result event on host interrupt line
    i2cNACK();
    i2cStop();*/
    
    i2cStart();
    i2cWrite(0x50);               //IMU device "write" address
    i2cWrite(0x34);               //IMU HostControl register address
    i2cWrite(0x01);               //IMU Run enable
    i2cNACK();
    i2cStop();

}

void IMUStandby(void)
{
    i2cStart();
    i2cWrite(0x50);               //IMU device "write" address
    i2cWrite(0x54);               //IMU AlgorithmControl register address
    i2cWrite(0x05);         //IMU enable standby state, Scaled sensor data, heading/pitch/roll output
    i2cNACK();
    i2cStop();
}

void IMUEnable(void)
{
    i2cStart();
    i2cWrite(0x50);               //IMU device "write" address
    i2cWrite(0x54);               //IMU AlgorithmControl register address
    i2cWrite(0x04);         //IMU disable standby state, Scaled sensor data, heading/pitch/roll output
    i2cNACK();
    i2cStop(); 
}

unsigned char IMUDataReady(void)
{
    unsigned char temp;
    
    i2cStart();
    i2cWrite(0x50);                 //IMU device "write" address
    i2cWrite(0x35);                 //IMU EventStatus register address
    i2cRestart();
    i2cWrite(0x51);                 //IMU device "read" address
    I2C2CONbits.RCEN = 1;          //initiate a READ
    while(!I2C2STATbits.RBF);       //wait for READ to complete
    temp = (unsigned char)I2C2RCV;  //get the data
    i2cNACK();
    i2cStop();
    return(temp);
}

unsigned char IMUDataBuffer[42];

// getIMUData reads all 42 bytes from IMU registers and places
// heading, pitch, roll, linear acceleration, and rotational velocity
// into data structure
// This function takes about 900uSec to run
void getIMUData(data_union *data)
{
    unsigned char i, j;
    
    i2cStart();
    i2cWrite(0x50);                 //IMU device "write" address
    i2cWrite(0x00);                 //IMU start of data addresses
    i2cRestart();
    i2cWrite(0x51);                 //IMU device "read" address
    for (i = 0; i < 42; i++)        //loop to read all 42 data values
    {    
        I2C2CONbits.RCEN = 1;          //initiate a READ
        while(!I2C2STATbits.RBF);       //wait for READ to complete
        IMUDataBuffer[i] = (unsigned char)I2C2RCV;  //get the data
        if (i == 41)
            i2cNACK();
        else
            i2cACK();
    }
    i2cStop();
    /*** rearrange data into proper spots ***/
    //heading, pitch, roll
    for (j = 2, i = 0; i < 0x0C; j++, i++)
        data->justBytes[j] = IMUDataBuffer[i];
    //linear acceleration x,y,z
    for (i = 0x1A, j = 14; i < 0x20; i++, j++)
        data->justBytes[j] = IMUDataBuffer[i];
    //rotational velocity x,y,z
     for (i = 0x22, j = 20; i < 0x28; i++, j++)
        data->justBytes[j] = IMUDataBuffer[i];
}
