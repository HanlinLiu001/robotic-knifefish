/* 
 * File:   Mastercontrolmain.c
 * Author: HarleyLiu
 *
 * Created on May 28, 2016, 10:36 AM
 */                

#include <stdio.h>
#include <xc.h>
#include <string.h>
#include <p33EP512GM304.h>
#include <stdlib.h>
#include "MasterInitialization.h"
#include <libpic30.h>
#include "flash.h"
#include "IMU.h"

/*** DSPIC33EP512GM304 Configuration Bit settings ***/
// FICD
#pragma config ICS = PGD1         // Communicate on PGEC1 and PGED1
#pragma config JTAGEN = OFF       // JTAG Enable bit disabled

// FPOR
#pragma config BOREN = OFF         // BOR is disabled
#pragma config ALTI2C1 = OFF       // I2C1 mapped to SDA1/SCL1 pins
#pragma config ALTI2C2 = OFF       // I2C2 mapped to SDA2/SCL2 pins

// FWDT
#pragma config FWDTEN = OFF       // Watchdog timer enabled/disabled by the user 
#pragma config PLLKEN = ON        // Clock switch to PLL source will wait until the PLL lock signal is valid

// FOSCSEL
#pragma config FNOSC = PRIPLL     // Primary Oscillator (XT + PLL, HS + PLL, EC + PLL)
#pragma config PWMLOCK = OFF      // Unlock the PWMLOCK
#pragma config IESO = OFF         // Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = XT                    // XT Crystal Oscillator Mode
#pragma config OSCIOFNC = OFF                 // OSC2 is clock output
#pragma config FCKSM = CSECMD                 // Clock switching is enabled,Fail-safe Clock Monitor is disabled
#pragma config IOL1WAY = OFF                   // Allow multiple reconfiguration

// FGS
#pragma config GWRP = OFF                     // General Segment may be written
#pragma config GCP = OFF                      // General Segment Code protect is Disabled

void TransmitMessage(char *message);
void Download(void);
void Test(void);
char Presinfo[50];
char Tempinfo[50];
char Message[10];
char Smessage[50];
char Userin;
char slavein;
int MessageIndex;
int MessageStarted = 0;
int MessageReady = 0;              //Check if Message is ready or not
int SmessageStarted = 0;
int SmessageReady = 0;
int SmessageIndex;
double Vtemp;
double Vpres;
double Vbus;
double scale1 = 0.00322265625;         // Multiplication of 1/1024 instead of dividing
double scale2 = 33.67003367;  // Multiplication of 1/(0.09*3.3)
int ADCValues[4] = {0, 0, 0, 0};
int k;
int MeasureIndicator = 0;
int PowerIndicator = 0;
int PowerIF = 0;
int timer;
double TEMP;
double PRES;
double Vleak;
char Sleak;
unsigned char rv1;
unsigned char rv2;
unsigned int Meastimer;
data_union dread;
data_union d;
data_union TestData;
unsigned int address;
/**********************************************************************************************/
/*************************************** Main Function ****************************************/
/**********************************************************************************************/
int main(void) 
{  
    /****Initialization****/
    Init_Oscillator();           //Initialize Oscillator
    Init_Pins_Master();          //Initialize Pins
    Init_UART();                 //Initialize UART for communication between radio, masterboard and slaveboards
    Init_TMR1();                 //Initialize Timer1
    Init_TMR2();                 //Initialize Timer2    
    Init_ADC();                  //Initialize ADC to read values from each sensor
    Init_I2C();                  //Initialize I2C for communication between IMU and master board
    Init_SPI();                  //Initialize SPI for communication between flash and master board
    TXENABLE = 0;                //Listen to serial port only set this IO when transmitMessage

    startIMU();
    __delay_ms(1000);
    IMUStandby();
    
  while(1)                     //Loop forever 
  { 
      if (U3STAbits.OERR)
        {
          U3STAbits.OERR = 0; //clear any overruns or port will stop receiving. If getting overruns data is coming in too fast
        }
        if (U3STAbits.URXDA)
        {
            Userin = (char)U3RXREG;
            if(MessageStarted)
          {
            if(MessageIndex>9)
            {
                MessageStarted = 0;
                MessageReady = 0;
            }
            else
            {
              if (Userin == '\n')
              {
                MessageStarted = 0;
                MessageReady = 1;
                Message[MessageIndex] = Userin;
                Message[MessageIndex+1] = 0;
              }
              else
              {
                Message[MessageIndex] = Userin;
                MessageIndex++;
              }
            }
          }
          else
          {
            if(Userin == '*')
            {
             MessageIndex = 0;
             Message[MessageIndex] = Userin;
             MessageReady = 0;
             MessageStarted = 1;
             MessageIndex++;
            }
          } 
        }
    if(MessageReady)
    {
        switch(Message[1])
                {
                  case '2':
                    PowerIndicator = 1;
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b00;    // Select 1:1 Prescaler
                    TMR1 = 0x00;               // Clear timer register
                    if(Message[3] == '5')
                    {
                     PR1 = 31680;
                    }
                    else{
                     PR1 = 39600;
                    }
                    timer = -32;
                    T1CONbits.TON = 1;         // Start Timer
                    TransmitMessage(Message);
                  break;
                  case '3':
                    PowerIndicator = 1;
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b00;    // Select 1:1 Prescaler
                    TMR1 = 0x00;               // Clear timer register
                    if(Message[3] == '5')
                    {
                     PR1 = 22000;
                    }
                    else{
                     PR1 = 26400;
                    }
                    timer = -32;
                    T1CONbits.TON = 1;         // Start Timer
                    TransmitMessage(Message);
                  break;
                  case '4':
                    PowerIndicator = 1;
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b00;    // Select 1:1 Prescaler
                    TMR1 = 0x00;               // Clear timer register
                    PR1 = 19800;
                    timer = -32;
                    T1CONbits.TON = 1;         // Start Timer
                    TransmitMessage(Message);
                  break;
                  case '1':
                    PowerIndicator = 1;
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b01;     // Select 1:8 Prescaler
                    TMR1 = 0x00;               // Clear timer register
                    if(Message[3] == '5')
                    {
                     PR1 = 6600;
                    }
                    else{
                     PR1 = 9900;
                    }
                    timer = -32;
                    T1CONbits.TON = 1;         // Start Timer
                    TransmitMessage(Message);
                  break;
                  case 'R':
                  case 'I':
                  case 'W':
                  case 'O':
                    PowerIndicator = 1;
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS = 0b01;       // Select 1:8 Prescaler
                    TMR1 = 0x00;                  // Clear timer register
                    if(Message[2] == '1'){
                     PR1 = 9900;
                     }
                    else{
                     PR1 = 4950;
                     }
                    timer = -32;
                    T1CONbits.TON = 1;
                    TransmitMessage(Message);
                  break;
                  case 'M':
                  case '0':
                  case 'S':
                    if(Message[1] == '0') 
                    {
                        PowerIndicator = 1;
                        timer = -32;
                    }
                    else
                    {
                        PowerIndicator = 0;
                        IMUStandby();
                    }
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b01;    // Select 1:8 Prescaler
                    TMR1 = 0x00;               // Clear timer register
                    PR1 = 19800;
                    T1CONbits.TON = 1;         // Start Timer 
                    if(Message[1] == 'S')
                    {
                      sprintf(Message, "*S\n");
                    }
                    TransmitMessage(Message);
                  break;
                  case 'e':
                    PowerIndicator = 0;
                    MeasureIndicator = 1;
                    Meastimer = 0;
                  break;
                  case 'd':
                    PowerIndicator = 0;
                    MeasureIndicator = 0;
                  break;
                  case 'D':
                   PowerIndicator = 0;
                   Download();   
                  break;
                  case 'T':
                   PowerIndicator = 0;
                   Test();
                  break;
                  case 'C':
                     PowerIndicator = 0;
                     IMUEnable();
                     for(k = 0; k<DATA_SIZE; k++) dread.justBytes[k] = 0;   //Clear out previous data
                     address = 0;
                     while (readSampleNumberFromFlash(address) != 0xFFFF) 
                      {
                        address++;                                         //Find next empty address in the flash
                      }
                     sprintf(Message, "*C\n");
                     TransmitMessage(Message);
                     putsUSER("Done");
                  break;
                }
        MessageReady = 0;
    }
        if(SmessageReady)
        {
            switch (Smessage[0])
            {
                case '0':
                sscanf(Smessage, "0,%s,%s", dread.myData.Power0, dread.myData.Track0);               
                break;
                case '1':
                sscanf(Smessage, "1,%s,%s", dread.myData.Power1, dread.myData.Track1);
                break; 
                case '2':
                sscanf(Smessage, "2,%s,%s", dread.myData.Power2, dread.myData.Track2);
                break;
                case '3':
                sscanf(Smessage, "3,%s,%s", dread.myData.Power3, dread.myData.Track3);
                break;
                case '4':
                sscanf(Smessage, "4,%s,%s", dread.myData.Power4, dread.myData.Track4);
                break;
                case '5':
                sscanf(Smessage, "5,%s,%s", dread.myData.Power5, dread.myData.Track5);
                break;
                case '6':
                sscanf(Smessage, "6,%s,%s", dread.myData.Power6, dread.myData.Track6);
                break;
                case '7':
                sscanf(Smessage, "7,%s,%s", dread.myData.Power7, dread.myData.Track7);
                getIMUData(&dread);
                writeDataToFlash(address, &dread);
                address++;
                break;
            }
        SmessageReady = 0;
        }
  }                 //End of while loop
  return 0;
}                   //End of main loop

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{    
  IFS0bits.T1IF = 0;         //Clear Timer1 interrupt flag 
  SYNC = !SYNC;              //Toggle SYNC pin
  if(PowerIndicator)
  {
   if(timer == 0)
   {
    TransmitMessage("*d\n");
   }
   timer++;
   if (timer>=32) 
   {
    timer = 0;
   }
  }
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{    
  IFS0bits.T2IF = 0;         //Clear Timer2 interrupt flag 
 if(MeasureIndicator)
 {
  Meastimer++;
  if (Meastimer == 7680 || Meastimer == 12800)
  {
    if(PRES>103)
    {
     rv1 = sprintf(Presinfo,"PRES HI %.3f kp!\r\n", PRES);
     putsUSER(Presinfo);
    }
   if (Vleak<2.85 || Sleak == '!')
   {
       putsUSER("LEAK!");
   }
  }
  if (Meastimer == 20480)
  {
    if(PRES>103)
    {
     rv1 = sprintf(Presinfo,"PRES HI %.3f kp!\r\n", PRES); 
    }
    else
    {
     rv1 = sprintf(Presinfo,"PRES %.3f kp\r\n", PRES);  
    }
    
    if(TEMP>26)
    {
     rv2 = sprintf(Tempinfo,"TEMP HI %.3f!\r\n", TEMP);  
    }
    else
    {
     rv2 = sprintf(Tempinfo,"TEMP %.3f\r\n", TEMP);  
    }
    putsUSER(Presinfo);
    putsUSER(Tempinfo);
    if (Vleak<2.85 || Sleak == '!')
   {
       putsUSER("LEAK!");
   }
  }
  if(Meastimer == 20481)
  {
   Meastimer=0;
  }
 }
  
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
        IFS0bits.U1RXIF = 0;
        if (U1STAbits.OERR)
        {
          U1STAbits.OERR = 0;                            //clear any overruns or port will stop receiving. If getting overruns data is coming in too fast
        }
        while(U1STAbits.URXDA)
        {
            slavein = U1RXREG;
            if(SmessageStarted)
            {
            if(SmessageIndex>49)
            {
                SmessageStarted = 0;
                SmessageReady = 0;
            }
            else
            {
              if (slavein == '\n')
              {
                SmessageStarted = 0;
                SmessageReady = 1;
                Smessage[SmessageIndex] = 0;
                //Smessage[SmessageIndex+1] = 0;     // Terminate message
              }
              else
              {
                Smessage[SmessageIndex] = slavein;
                SmessageIndex++;
              }
            }
          }
          else
          {
            if(slavein == '#')
            {
             SmessageIndex = 0;
             SmessageReady = 0;
             SmessageStarted = 1;
            }
          } 
        }
}
void __attribute__((__interrupt__, no_auto_psv)) _AD1Interrupt(void)
{
        IFS0bits.AD1IF = 0;         //Clear AD1 Flag
        ADCValues[0] = ADC1BUF0;  //Read AN6 conversion Leak sensor
        ADCValues[1] = ADC1BUF1;  //Read AN0 conversion Pressure 
        ADCValues[2] = ADC1BUF2;  //Read AN1 conversion Temperature
        ADCValues[3] = ADC1BUF3;  //Read AN2 conversion bus
        Vleak = ADCValues[0]*scale1;  //Leak output voltage
        Vpres = ADCValues[1]*scale1;  //Pressure output voltage
        Vtemp = ADCValues[2]*scale1;  //Temperature output voltage
        Vbus = ADCValues[3]*scale1;   //Bus voltage
        PRES = Vpres*scale2+10.5555556; //Get pressure from output voltage
        TEMP = 100*Vtemp-50;         //Get temperature from output voltage
}
void TransmitMessage(char* message)
{
  TXENABLE = 1;               //Enable line driver
  while(*message != '\0')
  {
    while(U1STAbits.UTXBF);   //Check transmit buffer full or not
    U1TXREG = (unsigned int)*message++;  
  }
  Nop();
  while(!U1STAbits.TRMT);     //Wait for empty transmit shift register 
  TXENABLE = 0;
}

enum downloadStates {DOWNMENU, DOWNWAIT, QUERY, DUMP, ERASEFLASH, AREYOUSURE, AREYOUREALLYSURE, EXITDOWN};
void printDownloadMenu(void)
{
    putsUSER("\r\nDOWNLOAD MENU:\r\n");
    putsUSER("1: Query number of data points saved to flash\r\n");
    putsUSER("2: Dump ALL data to USER port\r\n");
    putsUSER("!: Erase flash - BE CAREFUL\r\n");
    putsUSER("E: Exit download mode\r\n");
    putsUSER("\r\nEnter command:\r\n");
}
void Download(void)
{
enum downloadStates state = DOWNMENU;
    unsigned char inChar;
    int endOfFlash;
    unsigned int numberOfSamples, flashAddress;
    while (state != EXITDOWN)
    {
        switch(state)
        {
            case DOWNMENU:
                printDownloadMenu();
                state = DOWNWAIT;
                break;
            case DOWNWAIT:
                //check for received byte
                if(U3STAbits.OERR)
                {
                    U3STAbits.OERR = 0; //clear overrun error          
                }
                if (U3STAbits.URXDA)
                {
                    inChar = (unsigned char) U3RXREG;               
                    switch(inChar)
                    {
                        case '1':
                            state = QUERY;
                            break;
                        case '2':
                            state = DUMP;
                            break;
                        case '!':
                            state = AREYOUSURE;
                            break;
                        case 'E':
                            state = EXITDOWN;
                            break;
                        default:
                            state = DOWNMENU;
                            break;
                    }                    
                }
                break; //case WAIT
            case QUERY:                
                endOfFlash = 0;
                numberOfSamples = 0;
                while(!endOfFlash)
                {
                    if (readSampleNumberFromFlash(numberOfSamples) == 0xFFFF)
                        endOfFlash = 1;
                    else
                        numberOfSamples++;                    
                }
                rv = sprintf(message,"\r\nNumber of samples saved to flash = %d\r\n", numberOfSamples);
                putsUSER(message);
                state = DOWNMENU;
                break; //case QUERY
            case DUMP:
                putsUSER("DUMPING DATA. Press any key to continue\r\n");
                if(U3STAbits.OERR)
                {
                    U3STAbits.OERR = 0; //clear overrun error          
                }
                while(!U3STAbits.URXDA); //wait for key to be pressed
                inChar = (unsigned char) U3RXREG; //dummy read
                //start data dump
                endOfFlash = 0;
                flashAddress = 0;
                //print comma separated header
                putsUSER("Sample,Power0,Power1,Power2,Power3,Power4,Power5,Power6,Power7,");
                putsUSER("IMU Heading,IMU Pitch,IMU Roll,");
                putsUSER("IMU Linear Acceleration X,IMU Linear Acceleration Y,IMU Linear Acceleration Z,");
                putsUSER("IMU Rotational Velocity X,IMU Rotational Velocity Y,IMU Rotational Velocity Z,");
                putsUSER("Track0,Track1,Track2,Track3,Track4,Track5,Track6,Track7\r\n");
                        
                while(!endOfFlash)
                {
                    readDataFromFlash(flashAddress, &dread);
                    if (dread.myData.sampleNumber == 0xFFFF)
                    {
                        endOfFlash = 1;
                        putsUSER("*** DATA DUMP DONE ***\r\n");
                    }
                    else
                    {
                        printDataCSV(&dread);
                        flashAddress++;
                        if (flashAddress == 0xFFFF) 
                        {
                            endOfFlash = 1;
                        }
                    }
                }
                state = DOWNMENU;
                break; //case Dump
            case AREYOUSURE:
                putsUSER("ERASING FLASH DESTROYS ALL SAVED DATA!!\r\n");
                putsUSER("ARE YOU SURE YOU WANT TO DO THIS?\r\n");
                putsUSER("Press 'Y' to continue erase process, any other key to exit\r\n");
                if(U3STAbits.OERR)
                {
                    U3STAbits.OERR = 0; //clear overrun error          
                }
                while(!U3STAbits.URXDA); //wait for key to be pressed
                inChar = (unsigned char) U3RXREG;
                if (inChar == 'Y') 
                {
                    state = AREYOUREALLYSURE;
                }
                else{
                    state = DOWNMENU;
                }
                break;             
            case AREYOUREALLYSURE:
                putsUSER("\r\n**** READY TO ERASE ENTIRE FLASH ****\r\n");
                putsUSER("ARE YOU REALLY SURE YOU WANT TO DO THIS?\r\n");
                putsUSER("Press 'P' to start the erase process, any other key to exit\r\n");
                if(U3STAbits.OERR)
                {
                    U3STAbits.OERR = 0;    //clear overrun error          
                }
                while(!U3STAbits.URXDA);   //wait for key to be pressed
                inChar = (unsigned char) U3RXREG;
                if (inChar == 'P') 
                {
                    state = ERASEFLASH;
                }
                else{
                    state = DOWNMENU;
                }
                break;
            case ERASEFLASH:
                putsUSER("ERASING FLASH NOW");
                eraseFlash(0, _ALL);
                state = DOWNMENU;
                break; 
            case EXITDOWN:
                state = EXITDOWN;
                break;
            default:
                state = DOWNWAIT;
                break;                
        }
    }
}

   enum testStates {TESTMENU, TESTWAIT, TESTIMU, TESTFLASH, EXITTEST};

   void printTestMenu(void)
   {
    putsUSER("\r\nTEST MENU:\r\n");
    putsUSER("1: TEST IMU  -> USER port (press any key to return to test menu)\r\n");
    putsUSER("2: Test Flash\r\n");
    putsUSER("E: Exit test mode\r\n");
   }

   void Test(void)
  {
    
    enum testStates state = TESTMENU;
    unsigned char inChar;
    IMUEnable();
    while (state != EXITTEST)
    {
        switch(state)
        {
            case TESTMENU:
                printTestMenu();
                state = TESTWAIT;
                break;
            case TESTWAIT:
                //check for received byte
                if(U3STAbits.OERR)
                {
                    U3STAbits.OERR = 0; //clear overrun error          
                }
                if (U3STAbits.URXDA)
                {
                    inChar = (unsigned char) U3RXREG;               
                    switch(inChar)
                    {
                        case '1':
                            state = TESTIMU;
                            break;
                        case '2':
                            state = TESTFLASH;
                            break;
                        case 'E':
                            state = EXITTEST;
                            break;
                        default:
                            state = TESTMENU;
                    }
                }
                break;
            case TESTIMU:
                //check if user pressed a key
                if(U3STAbits.OERR)
                {
                    U3STAbits.OERR = 0; //clear overrun error          
                }
                if (U3STAbits.URXDA)
                {
                    inChar = (unsigned char) U3RXREG; 
                    state = TESTMENU;
                }
                //read & print out IMU data
                //if (IMUDataReady() & 0x04)
                //{
                    getIMUData(&TestData);
                    printData(&TestData);
                //}
                break; //case TESTIMU
            case TESTFLASH:
                if (checkFlashComms())
                    putsUSER("\r\n*** FLASH ID GOOD ***\r\n");
                else
                    putsUSER("\r\n*** FLASH ID BAD ***\r\n");
                state = TESTMENU;
                break; //case TESTFLASH
            case EXITTEST:
                state = EXITTEST;
                break;
            default:
                state = TESTWAIT;
                break; 
        }
    }
    IMUStandby();
  }
