/* 
 * File:   motorMechanicalStopmain.c
 * Author: HarleyLiu
 *
 * Created on May 30, 2016, 9:58 AM
 */

/********************************************/
/**********Define Globe Variables************/
/********************************************/
#define MaxDuty 4000
#define MinDuty -4000
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <p33EP512GM304.h>
#include <dsp.h>
#include <qei32.h>
#include <string.h>
#include "InitPinsSlave.h"
#include "SlaveInitializationAdd0.h"

/****************************************************/
/*** DSPIC33EP512GM304 Configuration Bit settings ***/
/****************************************************/
// FICD
#pragma config ICS = PGD1         // Communicate on PGEC1 and PGED1
#pragma config JTAGEN = OFF       // JTAG Enable bit disabled

// FPOR
#pragma config BOREN = OFF         // BOR is enabled
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

enum processState {MechanicalStop, RunOscMotion, Reverse, Inamp, Weamp, Opposed, Zero, StandBy};
enum processState state = StandBy;
char ReadAddress(void);
void TransmitMessage(char* message);
int address = 0;
int Pos1_value;                               // Motor1 position
int Pos2_value;                               // Motor2 position
int timer = 0;
unsigned int DelayCounter = 0;                // Delay counter in Mechanical stop
int MSCounter = 0;
int GBCounter = 0;                            // Go back to zero counter
int SineCounter1;
int SineCounter2;
int RevCounter1;
int RevCounter2;
int InampCounter0;
int InampCounter1;
int WeampCounter0;
int WeampCounter1;
int OpposedCounter0;
int OpposedCounter1;
unsigned int Revzerocounter = 0;              // Reversed motion delay counter
unsigned int Inampzerocounter = 0;            // Increasing amplitude motion delay counter
unsigned int Weampzerocounter = 0;            // Decreasing amplitude motion delay counter
unsigned int Opposedzerocounter = 0;          // Opposed amplitude motion delay counter
float MCurrent1;                              // Motor 1 current value
float MCurrent2;
float MVoltage;
double MPower1;
double MPower2;
double LeakVolt;
char myAddress;
char Slavemessage[30];
char SLeak;
double SPower; 
float scale256 = 0.00390625;                // 1/1024
float scale5 = 0.01611328125;               // Multiplication of 5*3.3/1024
float scale1 = 0.00322265625;               // Multiplication of 3.3/1024 instead of dividing
float scale05 = 0.001611328125;             // Multiplication of 3.3/2048
char temp;
int MessageReady = 0;
int MessageStarted = 0;
char Message[12];
int MessageIndex;
int ADCValues[4] = {0,0,0,0};
int dataIF = 0;
float SPower1;
float SPower2;
/**********************************************************************************************/
/*************************************** Main Function ****************************************/
/**********************************************************************************************/
int main(void) 
{
   
/****Initialization****/
    Init_Oscillator();         //Initialize Oscillator
    Init_Pins_Slave();         //Initialize Pins
    Init_TMR1();               //Initialize Timer1: power transmission
    Init_UART();               //Initialize UART1: communication with master board
    Init_PWM();                //Initialize PWM
    Init_Encoder();            //Initialize Encoder
    Init_PID();                //Initialize PID
    Init_Int1();               //Initialize External Interrupt1
    Init_ADC();                //Initialize ADC
    TXENABLE = 0;              //Listen to Comm bus
    myAddress = ReadAddress();
    while(1)                       //Loop forever 
    { 
        if(MessageReady)
        {
            switch(Message[0])
            {
                case 'M':
                  Write32bitQEI1GreaterEqual(&Max_encounter);
                  Write32bitQEI1PositionCounter(&Ini_encounter);
                  Write32bitQEI1LesserEqual (&Min_encounter);
                  Write32bitQEI2GreaterEqual(&Max_encounter);
                  Write32bitQEI2PositionCounter(&Ini_encounter);
                  Write32bitQEI2LesserEqual (&Min_encounter);
                  Open32bitQEI1(QEICON, QEIIOC, QEISTAT);
                  Open32bitQEI2(QEICON, QEIIOC, QEISTAT);
                 state = MechanicalStop;
                 break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4': 
                 SineCounter1 = -16*(int)(Message[3]-0x30)*2*address;
                 SineCounter2 = -16*(int)(Message[3]-0x30)*(2*address+1);
                 Write32bitQEI1GreaterEqual(&Max_encounter);
                 Write32bitQEI1LesserEqual (&Min_encounter);
                 Write32bitQEI2GreaterEqual(&Max_encounter);
                 Write32bitQEI2LesserEqual (&Min_encounter);
                 Open32bitQEI1(QEICON, QEIIOC, QEISTAT);
                 Open32bitQEI2(QEICON, QEIIOC, QEISTAT);
                 switch(Message[0])
                 {
                   case '0':  
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b01;    // Select 1:8 Prescaler
                    TMR1 = 0x00;               // Clear timer register
                    PR1 = 39600;
                    kCoeffs2[0] = Q15(0.8124);    
                    kCoeffs4[0] = Q15(0.8124);
                   break;
                   case '1':
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b01;    // Select 1:8 Prescaler
                    if(Message[2] == '5')
                    {
                       TMR1 = 0x00;               // Clear timer register
                       PR1 = 13200;
					   kCoeffs2[0] = Q15(0.8143);    
                       kCoeffs4[0] = Q15(0.8143); 
                    }
                    else 
                    {
                       TMR1 = 0x00;               // Clear timer register
                       PR1 = 19800;
					   kCoeffs2[0] = Q15(0.8124);    
                       kCoeffs4[0] = Q15(0.8124); 
                    }   
                   break;
                   case '2':
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b01;    // Select 1:8 Prescaler
                    if(Message[2] == '5')
                    {
                       TMR1 = 0x00;               // Clear timer register
                       PR1 = 7920;
					   kCoeffs2[0] = Q15(0.818);    
                       kCoeffs4[0] = Q15(0.818);  
                    }
                    else 
                    {
                       TMR1 = 0x00;               // Clear timer register
                       PR1 = 9900;
					   kCoeffs2[0] = Q15(0.8163);    
                       kCoeffs4[0] = Q15(0.8163);  
                    }  
                   break;
                   case '3':
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b00;    // Select 1:1 Prescaler
                    if(Message[2] == '5')
                    {
                       TMR1 = 0x00;               // Clear timer register
                       PR1 = 44000;
                    }
                    else 
                    {
                       TMR1 = 0x00;               // Clear timer register
                       PR1 = 52800;
                    } 
                    kCoeffs2[0] = Q15(0.8236);    
                    kCoeffs4[0] = Q15(0.8236);    
                   break;
                   case '4':
                    T1CONbits.TON = 0;
                    T1CONbits.TCKPS= 0b00;    // Select 1:1 Prescaler
                       TMR1 = 0x00;               // Clear timer register
                       PR1 = 39600;
                    kCoeffs2[0] = Q15(0.826);    
                    kCoeffs4[0] = Q15(0.826);    
                   break;
                  }
                 kCoeffs2[1] = Q15(0.0023);    
                 kCoeffs2[2] = Q15(0.1);   
                 kCoeffs4[1] = Q15(0.0023);    
                 kCoeffs4[2] = Q15(0.1);
                 PIDCoeffCalc(&kCoeffs2[0], &OMPID1);
                 PIDCoeffCalc(&kCoeffs4[0], &OMPID2);
                 state = RunOscMotion;
                 break;
                case 'R':
                 RevCounter1 = -16*(int)(Message[2]-0x30)*(2*(7-address)+1);
                 RevCounter2 = -16*(int)(Message[2]-0x30)*2*(7-address);
                 Write32bitQEI1GreaterEqual(&Max_encounter);
                 Write32bitQEI1LesserEqual (&Min_encounter);
                 Write32bitQEI2GreaterEqual(&Max_encounter);
                 Write32bitQEI2LesserEqual (&Min_encounter);
                 Open32bitQEI1(QEICON, QEIIOC, QEISTAT);
                 Open32bitQEI2(QEICON, QEIIOC, QEISTAT);
                 switch(Message[1])
                 {
                     case '1':
                      T1CONbits.TON = 0;
                      T1CONbits.TCKPS= 0b01;    // Select 1:8 Prescaler
                      TMR1 = 0x00;               // Clear timer register
                      PR1 = 19800;
                      kCoeffs2[0] = Q15(0.8124);    
                      kCoeffs4[0] = Q15(0.8124);    
                      break;
                     case '2':
                      T1CONbits.TON = 0;
                      T1CONbits.TCKPS= 0b01;    // Select 1:8 Prescaler
                      TMR1 = 0x00;               // Clear timer register
                      PR1 = 9900;
                      kCoeffs2[0] = Q15(0.8163);    
                      kCoeffs4[0] = Q15(0.8163);  
                      break;
                 }
                 kCoeffs2[1] = Q15(0.0023);    
                 kCoeffs2[2] = Q15(0.1);   
                 kCoeffs4[1] = Q15(0.0023);    
                 kCoeffs4[2] = Q15(0.1);
                 PIDCoeffCalc(&kCoeffs2[0], &OMPID1);
                 PIDCoeffCalc(&kCoeffs4[0], &OMPID2);     
                 state = Reverse;
                 break;
                case 'I':
                 InampCounter0 = -16*(int)(Message[2]-0x30)*2*address;
                 InampCounter1 = -16*(int)(Message[2]-0x30)*(2*address+1);
                 Write32bitQEI1GreaterEqual(&Max_encounter);
                 Write32bitQEI1LesserEqual (&Min_encounter);
                 Write32bitQEI2GreaterEqual(&Max_encounter);
                 Write32bitQEI2LesserEqual (&Min_encounter);
                 Open32bitQEI1(QEICON, QEIIOC, QEISTAT);
                 Open32bitQEI2(QEICON, QEIIOC, QEISTAT);
                 switch(Message[1])
                 {
                    case '1':
                     T1CONbits.TON =0;         
					 T1CONbits.TCKPS= 0b01;           // Select 1:8 Prescaler
                     TMR1 = 0x00;
                     PR1 = 19800;
                     kCoeffs2[0] = Q15(0.8124);
                     kCoeffs4[0] = Q15(0.8124);
                     break;
                    case '2':
                     T1CONbits.TON = 0;
					 T1CONbits.TCKPS= 0b01;
                     TMR1 = 0x00;
                     PR1 = 9900;
                     kCoeffs2[0] = Q15(0.8163);
                     kCoeffs4[0] = Q15(0.8163);
                     break;
                 }
                 kCoeffs2[1] = Q15(0.0023);
                 kCoeffs2[2] = Q15(0.1);
                 kCoeffs4[1] = Q15(0.0023);
                 kCoeffs4[2] = Q15(0.1);
                 PIDCoeffCalc(&kCoeffs2[0], &OMPID1);
                 PIDCoeffCalc(&kCoeffs4[0], &OMPID2);
                 state = Inamp;
                 break;
                case 'W':
                 WeampCounter0 = -16*(int)(Message[2]-0x30)*2*address;
                 WeampCounter1 = -16*(int)(Message[2]-0x30)*(2*address+1);
                 Write32bitQEI1GreaterEqual(&Max_encounter);
                 Write32bitQEI1LesserEqual (&Min_encounter);
                 Write32bitQEI2GreaterEqual(&Max_encounter);
                 Write32bitQEI2LesserEqual (&Min_encounter);
                 Open32bitQEI1(QEICON, QEIIOC, QEISTAT);
                 Open32bitQEI2(QEICON, QEIIOC, QEISTAT);
                 switch(Message[1])
                 {
                    case '1':
                     T1CONbits.TON =0;         
					 T1CONbits.TCKPS= 0b01;           // Select 1:8 Prescaler
                     TMR1 = 0x00;
                     PR1 = 19800;
                     kCoeffs2[0] = Q15(0.8124);
                     kCoeffs4[0] = Q15(0.8124);
                     break;
                    case '2':
                     T1CONbits.TON = 0;
					 T1CONbits.TCKPS= 0b01;
                     TMR1 = 0x00;
                     PR1 = 9900;
                     kCoeffs2[0] = Q15(0.8163);
                     kCoeffs4[0] = Q15(0.8163);
                     break;
                 }
                 kCoeffs2[1] = Q15(0.0023);
                 kCoeffs2[2] = Q15(0.1);
                 kCoeffs4[1] = Q15(0.0023);
                 kCoeffs4[2] = Q15(0.1);
                 PIDCoeffCalc(&kCoeffs2[0], &OMPID1);
                 PIDCoeffCalc(&kCoeffs4[0], &OMPID2);
                 state = Weamp;
                 break;
                case 'O':
                 OpposedCounter0 = -16*(int)(Message[2]-0x30)*2*address;
                 OpposedCounter1 = -16*(int)(Message[2]-0x30)*(2*address+1);
                 Write32bitQEI1GreaterEqual(&Max_encounter);
                 Write32bitQEI1LesserEqual (&Min_encounter);
                 Write32bitQEI2GreaterEqual(&Max_encounter);
                 Write32bitQEI2LesserEqual (&Min_encounter);
                 Open32bitQEI1(QEICON, QEIIOC, QEISTAT);
                 Open32bitQEI2(QEICON, QEIIOC, QEISTAT);
                 switch(Message[1])
                 {
                    case '1':
                     T1CONbits.TON =0;        
					 T1CONbits.TCKPS= 0b01;             // Select 1:8 Prescaler
                     TMR1 = 0x00;
                     PR1 = 19800;
                     kCoeffs2[0] = Q15(0.8124);
                     kCoeffs4[0] = Q15(0.8124);
                     break;
                    case '2':
                     T1CONbits.TON = 0;
					 T1CONbits.TCKPS= 0b01;
                     TMR1 = 0x00;
                     PR1 = 9900;
                     kCoeffs2[0] = Q15(0.8163);
                     kCoeffs4[0] = Q15(0.8163);
                     break;
                 }
                 kCoeffs2[1] = Q15(0.0023);
                 kCoeffs2[2] = Q15(0.1);
                 kCoeffs4[1] = Q15(0.0023);
                 kCoeffs4[2] = Q15(0.1);
                 PIDCoeffCalc(&kCoeffs2[0], &OMPID1);
                 PIDCoeffCalc(&kCoeffs4[0], &OMPID2);
                 state = Opposed;
                 break;
                case 'S':
                 AD1CON1bits.ADON = 0;                //Disable AD1
                 dataIF = 0;
                 state = Zero;
                 break;
                case 'C':
                 IFS0bits.AD1IF = 0;                  //Clear AD1 Flag
                 IEC0bits.AD1IE = 1;                  //Enable AD1 Interrupt
                 AD1CON1bits.ADON = 1;                //Enable AD1
                 break;
                case 'd':
                 timer = 0;
                 IPC0bits.T1IP = 4;      // Set Timer 1 Interrupt Priority Level
                 IFS0bits.T1IF = 0;         // Clear Timer 1 Interrupt Flag
                 IEC0bits.T1IE = 1;         // Enable Timer1 interrupt
                 T1CONbits.TON = 1;         // Enable Timer
                 break;
                default:
                 state = StandBy;
                 break;
            }
            MessageReady = 0;
        }
        if(dataIF)
        {
            sprintf(Slavemessage, "#%c,%.4f,%5d\n", myAddress, SPower, Pos2_value);
            TransmitMessage(Slavemessage);
            dataIF = 0;
        }
    }                          //End of while loop
    return 0;
}                              // End of main function

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{ 
    IFS0bits.T1IF = 0;         // Clear Timer 1 Interrupt Flag
    if (timer == address+1)
    {
          dataIF = 1;
          T1CONbits.TON = 0;         // Close Timer
    }
    timer++;
}

void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void)
{
  IFS1bits.INT1IF = 0;                                      // Clear Interrupt1 flag 
  switch(state)
 {
  case MechanicalStop:
       if(DelayCounter<=256)
        {                  
          PDC2 = 4000;                                      // Let two motors rotate slowly  
          SDC2 = 3650; 
          
          PDC6 = 4000;
          SDC6 = 3650;                                       
          DelayCounter++;                                    // Wait for 3 sec
        }
       else if (DelayCounter>256 && DelayCounter<=448)
        {
          PDC2 = 4000;                                       // After long enough time let two motors stop
          SDC2 = 4000;
          
          PDC6 = 4000;
          SDC6 = 4000;
          if(DelayCounter == 448)
          {
          Write32bitQEI1PositionCounter(&Ini_encounter);      // Set as new zero position
          Write32bitQEI2PositionCounter(&Ini_encounter); 
          }
          DelayCounter++;                                     // Wait for 1 sec  
        }
       else
       {
       ++MSCounter;
       Pos1_value = Read32bitQEI1PositionCounter()<<4;        // Read the Encoder1 Position
       Pos1_ref = MStable[MSCounter];
       
       MSPID1.controlReference =  Pos1_ref;                   //Set the Reference Input for MSPID1 controller
       MSPID1.measuredOutput = Pos1_value;                    //Measured output from Encoder1
       PID(&MSPID1);                                          //Compute MSPID1                                   
       PID1_output = Fract2Float(MSPID1.controlOutput);       //MSPID1 output
       PWM2_set = (int)(4000*PID1_output);

       PDC2 = 4000+PWM2_set;                                  //PWM2<0 since the motor turn back 90 degree
       SDC2 = 4000;
       
       Pos2_value = Read32bitQEI2PositionCounter()<<4;        // Read the Encoder2 Position
       Pos2_ref = MStable[MSCounter];
       
       MSPID2.controlReference =  Pos2_ref;                   //Set the Reference Input for MSPID2 controller
       MSPID2.measuredOutput = Pos2_value;                    //Measured output from Encoder1
       PID(&MSPID2);                                          //Compute MSPID2                                   
       PID2_output = Fract2Float(MSPID2.controlOutput);       //MSPID2 output
       PWM6_set = (int)(4000*PID2_output);

       PDC6 = 4000+PWM6_set;                                  //PWM6<0 since the motor turn back 90 degree
       SDC6 = 4000;

       if(MSCounter == 256)                                  //MSCounter=1024, then the motor will be at absolute 0 position
        {  
        MSCounter = 0;   
        DelayCounter = 0;     
        Write32bitQEI1PositionCounter(&Ini_encounter);
        Write32bitQEI2PositionCounter(&Ini_encounter);
        state = StandBy;                                           //Stop two motors
        }
      }      
   break;
  case RunOscMotion:                                          //Case OscillatingMotion
        Pos1_value = Read32bitQEI1PositionCounter()<<4;       //Read the Encoder1 Position
        if (SineCounter1<0)
           {
              Pos1_ref = 0;
           }
        else{
        Pos1_ref = Sinetable[SineCounter1]-5461;              //Reference position1 
        }
        OMPID1.controlReference = Pos1_ref;                   //Set the Reference Input for OMPID1 controller
        OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
        PID(&OMPID1);                                         //Compute OMPID1                                   
        PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
        PWM2_set = (int)(4000*PID1_output);
        ++SineCounter1;                                       //Sinetable indicator update
        
        Pos2_value = Read32bitQEI2PositionCounter()<<4;        //Read the Encoder2 Position
        if (SineCounter2<0)
           {
              Pos2_ref = 0;
           }
        else{
            Pos2_ref = Sinetable[SineCounter2]-5461;         // Reference position2 
           }
        OMPID2.controlReference = Pos2_ref;                    //Set the Reference Input for OMPID2 controller
        OMPID2.measuredOutput = Pos2_value;                    //Measured output from Encoder2
        PID(&OMPID2);                                          //Compute OMPID2                                   
        PID2_output = Fract2Float(OMPID2.controlOutput);       //OMPID2 output
        PWM6_set = (int)(4000*PID2_output);
        ++SineCounter2;
        if (PWM2_set > MaxDuty)
           {
              PWM2_set = MaxDuty;
           }                  
        if (PWM2_set < MinDuty)
           {
              PWM2_set = MinDuty;
           }
        if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
        else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
        if (PWM6_set >MaxDuty)                                 //Set PWM upper/lower limit
           {
              PWM6_set = MaxDuty;
           }
        if (PWM6_set < MinDuty)
           {
              PWM6_set = MinDuty;
           }
        if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
        else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
        MPower1 = MCurrent1*MVoltage*fabsf(PID1_output);
        MPower2 = MCurrent2*MVoltage*fabsf(PID2_output);
		SPower = MPower1 + MPower2;                                                            // Total power of two motors
        if(SineCounter1 == 256)
           {
              SineCounter1 = 0;
           }            
        if(SineCounter2 == 256)
           {
              SineCounter2 = 0;
           }
   break;
  case Reverse:
      if(Revzerocounter<=384)
      {
        Revzerocounter++;
        Pos1_value = Read32bitQEI1PositionCounter()<<4;       //Read the Encoder1 Position
        OMPID1.controlReference = 0;                          //Set the Reference Input as 0
        OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
        PID(&OMPID1);                                         //Compute OMPID1                                   
        PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
        PWM2_set = (int)(4000*PID1_output);
        
        Pos2_value = Read32bitQEI2PositionCounter()<<4;
        OMPID2.controlReference = 0;
        OMPID2.measuredOutput = Pos2_value;
        PID(&OMPID2);
        PID2_output = Fract2Float(OMPID2.controlOutput);
        PWM6_set = (int)(4000*PID2_output);
        if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                 
        if (PWM2_set < MinDuty) PWM2_set = MinDuty;
        if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
        else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
        if (PWM6_set > MaxDuty) PWM6_set = MaxDuty;             //Set PWM upper/lower limit 
        if (PWM6_set < MinDuty) PWM6_set = MinDuty;
        if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
        else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
      }
      else{
        Pos1_value = Read32bitQEI1PositionCounter()<<4;        //Read the Encoder2 Position
        if (RevCounter1<0)
           {
              Pos1_ref = 0;
           }
        else{
        Pos1_ref = Sinetable[RevCounter1]-5461;              //Reference position1 
        }
        OMPID1.controlReference = Pos1_ref;                   //Set the Reference Input for OMPID1 controller
        OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
        PID(&OMPID1);                                         //Compute OMPID1                                   
        PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
        PWM2_set = (int)(4000*PID1_output);
        ++RevCounter1;                                       //Sinetable indicator update
        
        Pos2_value = Read32bitQEI2PositionCounter()<<4;        //Read the Encoder2 Position
        if (RevCounter2<0)
           {
              Pos2_ref = 0;
           }
        else{
        Pos2_ref = Sinetable[RevCounter2]-5461;         // Reference position2 
        }
        OMPID2.controlReference = Pos2_ref;                    //Set the Reference Input for OMPID2 controller
        OMPID2.measuredOutput = Pos2_value;                    //Measured output from Encoder2
        PID(&OMPID2);                                          //Compute OMPID2                                   
        PID2_output = Fract2Float(OMPID2.controlOutput);       //OMPID2 output
        PWM6_set = (int)(4000*PID2_output);
        ++RevCounter2;
        if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                
        if (PWM2_set < MinDuty) PWM2_set = MinDuty;
        if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
        else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
        if (PWM6_set > MaxDuty) PWM6_set = MaxDuty;
        if (PWM6_set < MinDuty) PWM6_set = MinDuty;
        if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
        else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
        MPower1 = MCurrent1*MVoltage*fabsf(PID1_output);
        MPower2 = MCurrent2*MVoltage*fabsf(PID2_output);
		SPower = MPower1 + MPower2;                                                            // Total power of two motors
        if(RevCounter1 == 256)
           {
              RevCounter1 = 0;
           }            
        if(RevCounter2 == 256)
           {
              RevCounter2 = 0;
           }
      }
   break;
  case Inamp:
      if(Inampzerocounter<=384)
      {
        Inampzerocounter++;
        Pos1_value = Read32bitQEI1PositionCounter()<<4;       //Read the Encoder1 Position
        OMPID1.controlReference = 0;                          //Set the Reference Input as 0
        OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
        PID(&OMPID1);                                         //Compute OMPID1                                   
        PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
        PWM2_set = (int)(4000*PID1_output);
        
        Pos2_value = Read32bitQEI2PositionCounter()<<4;
        OMPID2.controlReference = 0;
        OMPID2.measuredOutput = Pos2_value;
        PID(&OMPID2);
        PID2_output = Fract2Float(OMPID2.controlOutput);
        PWM6_set = (int)(4000*PID2_output);

        if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                 
        if (PWM2_set < MinDuty) PWM2_set = MinDuty;
        if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
        else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
        if (PWM6_set > MaxDuty) PWM6_set = MaxDuty;             //Set PWM upper/lower limit 
        if (PWM6_set < MinDuty) PWM6_set = MinDuty;
        if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
        else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
      }
      else{
        Pos1_value = Read32bitQEI1PositionCounter()<<4;        //Read the Encoder2 Position
        if (InampCounter0<0)
           {
              Pos1_ref = 0;
           }
        else{
        Pos1_ref = Inamptable0[InampCounter0]-2003;              //Reference position1 
        }
        OMPID1.controlReference = Pos1_ref;                   //Set the Reference Input for OMPID1 controller
        OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
        PID(&OMPID1);                                         //Compute OMPID1                                   
        PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
        PWM2_set = (int)(4000*PID1_output);
        ++InampCounter0;                                       //Sinetable indicator update
        
        Pos2_value = Read32bitQEI2PositionCounter()<<4;        //Read the Encoder2 Position
        if (InampCounter1<0)
           {
              Pos2_ref = 0;
           }
        else{
        Pos2_ref = Inamptable1[InampCounter1]-2294;              // Reference position2 
        }
        OMPID2.controlReference = Pos2_ref;                    //Set the Reference Input for OMPID2 controller
        OMPID2.measuredOutput = Pos2_value;                    //Measured output from Encoder2
        PID(&OMPID2);                                          //Compute OMPID2                                   
        PID2_output = Fract2Float(OMPID2.controlOutput);       //OMPID2 output
        PWM6_set = (int)(4000*PID2_output);
        ++InampCounter1;
        if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                
        if (PWM2_set < MinDuty) PWM2_set = MinDuty;
        if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
        else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
        if (PWM6_set > MaxDuty) PWM6_set = MaxDuty;
        if (PWM6_set < MinDuty) PWM6_set = MinDuty;
        if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
        else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
        MPower1 = MCurrent1*MVoltage*fabsf(PID1_output);
        MPower2 = MCurrent2*MVoltage*fabsf(PID2_output);
		SPower = MPower1 + MPower2;                                                            // Total power of two motors
        if(InampCounter0 == 256)
           {
              InampCounter0 = 0;
           }            
        if(InampCounter1 == 256)
           {
              InampCounter1 = 0;
           }
      }
   break;
  case Weamp:
      if(Weampzerocounter<=384)
      {
          Weampzerocounter++;
          Pos1_value = Read32bitQEI1PositionCounter()<<4;       //Read the Encoder1 Position
          OMPID1.controlReference = 0;                          //Set the Reference Input as 0
          OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
          PID(&OMPID1);                                         //Compute OMPID1                                   
          PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
          PWM2_set = (int)(4000*PID1_output);
        
          Pos2_value = Read32bitQEI2PositionCounter()<<4;
          OMPID2.controlReference = 0;
          OMPID2.measuredOutput = Pos2_value;
          PID(&OMPID2);
          PID2_output = Fract2Float(OMPID2.controlOutput);
          PWM6_set = (int)(4000*PID2_output);
          if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                 
          if (PWM2_set < MinDuty) PWM2_set = MinDuty;
          if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
          else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
          if (PWM6_set > MaxDuty) PWM6_set = MaxDuty;             //Set PWM upper/lower limit 
          if (PWM6_set < MinDuty) PWM6_set = MinDuty;
          if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
          else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
      }
      else{
        Pos1_value = Read32bitQEI1PositionCounter()<<4;        //Read the Encoder2 Position
        if (WeampCounter0<0)
           {
              Pos1_ref = 0;
           }
        else{
        Pos1_ref = Weamptable0[WeampCounter0]-6372;              //Reference position1 
        }
        OMPID1.controlReference = Pos1_ref;                   //Set the Reference Input for OMPID1 controller
        OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
        PID(&OMPID1);                                         //Compute OMPID1                                   
        PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
        PWM2_set = (int)(4000*PID1_output);
        ++WeampCounter0;                                       //Sinetable indicator update
        
        Pos2_value = Read32bitQEI2PositionCounter()<<4;        //Read the Encoder2 Position
        if (WeampCounter1<0)
           {
              Pos2_ref = 0;
           }
        else{
        Pos2_ref = Weamptable1[WeampCounter1]-6080;              //Reference position1 
        }
        OMPID2.controlReference = Pos2_ref;                    //Set the Reference Input for OMPID2 controller
        OMPID2.measuredOutput = Pos2_value;                    //Measured output from Encoder2
        PID(&OMPID2);                                          //Compute OMPID2                                   
        PID2_output = Fract2Float(OMPID2.controlOutput);       //OMPID2 output
        PWM6_set = (int)(4000*PID2_output);
        ++WeampCounter1;
        if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                
        if (PWM2_set < MinDuty) PWM2_set = MinDuty;
        if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
        else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
        if (PWM6_set > MaxDuty) PWM6_set = MaxDuty;
        if (PWM6_set < MinDuty) PWM6_set = MinDuty;
        if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
        else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
        MPower1 = MCurrent1*MVoltage*fabsf(PID1_output);
        MPower2 = MCurrent2*MVoltage*fabsf(PID2_output);
		SPower = MPower1 + MPower2;                                                            // Total power of two motors
        if(WeampCounter0 == 256)
           {
              WeampCounter0 = 0;
           }            
        if(WeampCounter1 == 256)
           {
              WeampCounter1 = 0;
           }
      } 
   break;
  case Opposed:
      if(Opposedzerocounter<=384)
      {
          Opposedzerocounter++;
          Pos1_value = Read32bitQEI1PositionCounter()<<4;       //Read the Encoder1 Position
          OMPID1.controlReference = 0;                          //Set the Reference Input as 0
          OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
          PID(&OMPID1);                                         //Compute OMPID1                                   
          PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
          PWM2_set = (int)(4000*PID1_output);
          
          Pos2_value = Read32bitQEI2PositionCounter()<<4;
          OMPID2.controlReference = 0;
          OMPID2.measuredOutput = Pos2_value;
          PID(&OMPID2);
          PID2_output = Fract2Float(OMPID2.controlOutput);
          PWM6_set = (int)(4000*PID2_output);
          if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                 
          if (PWM2_set < MinDuty) PWM2_set = MinDuty;
          if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
          else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
          if (PWM6_set > MaxDuty) PWM6_set = MaxDuty;             //Set PWM upper/lower limit 
          if (PWM6_set < MinDuty) PWM6_set = MinDuty;
          if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
          else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
      }
      else{
          Pos1_value = Read32bitQEI1PositionCounter()<<4;        //Read the Encoder2 Position
          if(OpposedCounter0<0)
          {
              Pos1_ref = 0;
          }
          else{
              Pos1_ref = Sinetable[OpposedCounter0]-5461;        //Reference position1 
          }
          OMPID1.controlReference = Pos1_ref;                   //Set the Reference Input for OMPID1 controller
          OMPID1.measuredOutput = Pos1_value;                   //Measured output from Encoder1
          PID(&OMPID1);                                         //Compute OMPID1                                   
          PID1_output = Fract2Float(OMPID1.controlOutput);      //OMPID1 output
          PWM2_set = (int)(4000*PID1_output);
          ++OpposedCounter0;
          
          Pos2_value = Read32bitQEI2PositionCounter()<<4;        //Read the Encoder2 Position
          if(OpposedCounter1<0)
          {
              Pos2_ref = 0;
          }
          else{
              Pos2_ref = Sinetable[OpposedCounter1]-5461;
          }
           OMPID2.controlReference = Pos2_ref;                    //Set the Reference Input for OMPID2 controller
           OMPID2.measuredOutput = Pos2_value;                    //Measured output from Encoder2
           PID(&OMPID2);                                          //Compute OMPID2                                   
           PID2_output = Fract2Float(OMPID2.controlOutput);       //OMPID2 output
           PWM6_set = (int)(4000*PID2_output);
           ++OpposedCounter1;
           if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                
           if (PWM2_set < MinDuty) PWM2_set = MinDuty;
           if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
           else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
        
           if (PWM6_set > MaxDuty) PWM6_set = MaxDuty;
           if (PWM6_set < MinDuty) PWM6_set = MinDuty;
           if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
           else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
           MPower1 = MCurrent1*MVoltage*fabsf(PID1_output);
           MPower2 = MCurrent2*MVoltage*fabsf(PID2_output);
		   SPower = MPower1 + MPower2;                                                            // Total power of two motors
           if(OpposedCounter0 == 256)
           {
               OpposedCounter0 = 0;
           }
           if(OpposedCounter1 == 256)
           {
               OpposedCounter1 = 0;
           }
      }
   break;
  case Zero:
      GBCounter++;
       Pos1_value = Read32bitQEI1PositionCounter()<<4;        // Read the Encoder1 Position
       Pos1_ref = 0;
       GBPID1.controlReference = Pos1_ref;                   //Set the Reference Input for MSPID1 controller
       GBPID1.measuredOutput = Pos1_value;                    //Measured output from Encoder1
       PID(&GBPID1);                                          //Compute MSPID1                                   
       PID1_output = Fract2Float(GBPID1.controlOutput);       //MSPID1 output
       PWM2_set = (int)(4000*PID1_output);
       if (PWM2_set > MaxDuty) PWM2_set = MaxDuty;                  
       if (PWM2_set < MinDuty) PWM2_set = MinDuty;
        if(PWM2_set>0)
           {
              PDC2 = 4000;                                      //Set duty cycle of PWM2H (In1) 
              SDC2 = 4000-PWM2_set;                             //Set duty cycle of PWM2L (In2) 
           }
        else
           {
              PDC2 = 4000+PWM2_set;
              SDC2 = 4000;
           }
       
       Pos2_value = Read32bitQEI2PositionCounter()<<4;        // Read the Encoder2 Position
       Pos2_ref = 0;
       GBPID2.controlReference = Pos2_ref;                   //Set the Reference Input for MSPID2 controller
       GBPID2.measuredOutput = Pos2_value;                    //Measured output from Encoder1
       PID(&GBPID2);                                          //Compute MSPID2                                   
       PID2_output = Fract2Float(GBPID2.controlOutput);       //MSPID2 output
       PWM6_set = (int)(4000*PID2_output);
       if (PWM6_set >MaxDuty) PWM6_set = MaxDuty;
       if (PWM6_set < MinDuty) PWM6_set = MinDuty;
        if (PWM6_set>0)
           {
              PDC6 = 4000;                                      //Set duty cycle of PWM6H (In1) 
              SDC6 = 4000-PWM6_set;                             //Set duty cycle of PWM6L (In2) 
           }
        else
           {
              PDC6 = 4000+PWM6_set;
              SDC6 = 4000;
           }
      if(GBCounter >= 1024)
      {
        GBCounter = 0;
        Revzerocounter = 0;
        Inampzerocounter = 0;
        Weampzerocounter = 0;
        Opposedzerocounter = 0;
        state = StandBy;
      } 
   break;
  case StandBy:
      PDC2 = 4000;
      SDC2 = 4000;
      PDC6 = 4000;
      SDC6 = 4000;
      Close32bitQEI1(); 
      Close32bitQEI2();
   break;
  }
}                                    //End of Interrupt ISR

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
        IFS0bits.U1RXIF = 0;
        if(U1STAbits.OERR)
        {
           U1STAbits.OERR = 0;     //Clear overruns
        }
        while(U1STAbits.URXDA)
        {
          temp = (char)U1RXREG;
          if(MessageStarted)
          {
            if(MessageIndex>11)
            {
                MessageStarted = 0;
                MessageReady = 0;
            }
            else
            {
              if (temp == '\n')
              {
                MessageStarted = 0;
                MessageReady = 1;
                Message[MessageIndex] = temp; 
                Message[MessageIndex+1] = 0; //Terminate Message  
              }
              else
              {
                Message[MessageIndex] = temp;
                MessageIndex++;
              }
            }
          }
          else
          {
            if(temp == '*')
            {
             MessageIndex = 0;
             MessageReady = 0;
             MessageStarted = 1;
            }
          } 
        }
}

void __attribute__((__interrupt__, no_auto_psv)) _AD1Interrupt(void)
{
        IFS0bits.AD1IF = 0;         //Clear AD1 Flag
        ADCValues[0] = ADC1BUF0;
        ADCValues[1] = ADC1BUF1;
        ADCValues[2] = ADC1BUF2;
        ADCValues[3] = ADC1BUF3;
        MVoltage = ADCValues[0]*scale5;
        //LeakVolt = ADCValues[1]*scale1;
        MCurrent2 = ADCValues[2]*scale05;
        MCurrent1 = ADCValues[3]*scale05;
        /*if(LeakVolt < 2.78) SLeak = '!';
        else SLeak = '$';*/
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

char ReadAddress(void)                  //Subroutine to get myAddress  
{
    return((char)(!ADDRESS0 + (!ADDRESS1 * 2) + (!ADDRESS2 * 4)) + 0x30);
}
