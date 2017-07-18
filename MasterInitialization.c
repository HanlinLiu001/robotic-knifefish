/* 
 * File:   MasterInitialization.c
 * Author: HarleyLiu
 *
 * Created on June 2, 2016, 11:31 AM
 */
#include<p33EP512GM304.h>
#include<string.h>
#include<xc.h>
#include<stdio.h>
#include"MasterInitialization.h"

void Init_Pins_Master()
{    
// Configure PPS Registers
__builtin_write_OSCCONL(OSCCON & 0xBF); // Unlock Registers
/**** UART Pins Selection****/
/**Input**/
RPINR18bits.U1RXR = 38;                 // UART1 RX is RP38 - Comms Bus
RPINR19bits.U2RXR = 43;                 // UART2 RX is RP43 - Topside
RPINR27bits.U3RXR = 56;                 // UART3 RX is RP56 - Radio

/**Output*/
RPOR1bits.RP37R = 0b000001;              // UART1 TX is RP37 - Comms Bus
RPOR4bits.RP42R = 0b000011;              // UART2 TX is RP42 - Topside
RPOR7bits.RP57R = 0b011011;              // UART3 TX is RP57 - Radio
__builtin_write_OSCCONL(OSCCON | 0x40);  // Lock Registers

/*****Configure Port Analog or Digital******/
ANSELA = 0b0000000000000011;             // PortA 0 1 analog input
ANSELB = 0b0000000000000011;             // PortB 0 1 analog input
ANSELC = 0b0000000000000001;             // PortC 0 analog input

/*****Configure Port Directions and defaults*****/
/****Port A****/
//RA0 - Pressure analog input
//RA1 - Temperature analog input
//RA2 - External Oscillator input
//RA3 - External Oscillator input

//RA4 - SPI1 MOSI output
//RA7 - Hall 2 input
//RA8 - SDA2 input
//RA9 - SPI1 MISO input
//RA10- Unused output   
LATA = 0x0000;                   //Set all outputs low  
TRISA = 0b0000001110001111;      //Set I/O directions
 
/****Port B****/
//RB0 - Bus voltage analog input
//RB1 - 3.000 Reference voltage analog input
//RB2 - PGC1 debug program input
//RB3 - PGD1 debug program input

//RB4 - SCL2 output
//RB5 - comms TX output
//RB6 - comms RX input
//RB7 - comms TX/RX enable output

//RB8 - SYNC output
//RB9 - Hall1 input
//RB10- Topside TX output
//RB11- Topside RX input

//RB12- unused output
//RB13- unused output
//RB14- RedLED output
//RB15- GreenLED output
LATB = 0x0000;                //Set all outputs low
GLED = 1;                     //Set Green led
RLED = 1;                     //Set Red led
TRISB = 0b0000101001001111;   //Set I/O directions

/****Port C****/
//RC0 - Leak analog input
//RC1 - unused output
//RC2 - MMMINT input
//RC3 - SPI1 SCK output

//RC4 - SPI1 FLASH_CS output
//RC5 - SPI1 SD_CS output
//RC6 - Radio RTS output (Input to radio)
//RC7 - Radio CTS input  (Output from radio)

//RC8 - Radio TX - input (Output from radio)
//RC9 - Radio RX - output (Input to radio)
LATC = 0x0000;                  //Set all outputs low
FLASH_CS = 1;                   //Set flash cs
SD_CS = 1;                      //Set SD cs
TRISC = 0b0000000110000101;     //Set I/O directions
}
/************************************************************************/
/***********************Oscillator Initialization************************/
/************************************************************************/
void Init_Oscillator(void)
{
    // Fin = 7.3728 MHZ External Oscillator
    // 0.8M<FPLLI<8M FPLLI = Fin/N1
    // 120M<Fsys<340M Fsys = Fin*M/N1
    // 15M<Fosc<120M  Fosc = Fin*M/(N1*N2))
    CLKDIVbits.PLLPRE = 0;     // N1= 2 Default
    CLKDIVbits.PLLPOST = 1;    // N2= 4 Default
    PLLFBDbits.PLLDIV = 86;   //M = 88
    // FPLLI = 3.6864M
    // Fsys = 324.4M
    // Fosc = 81.1M
    // Fcy = 40.55MS
    OSCTUNbits.TUN = 0;
    RCONbits.SWDTEN = 0;
    __builtin_write_OSCCONH(0x03);  //External Oscillator with PLL
    __builtin_write_OSCCONL(0x01);   //Start clock switching
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b011);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);
    Nop();
    Nop();
    Nop();
}

/************************************************************************/
/******************* Initialize Timer ***********************************/
/************************************************************************/
void Init_TMR1(void)
{
    T1CONbits.TON = 0;         // Disable Timer
    T1CONbits.TCS = 0;         // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;       // Disable Gated Timer mode
    /*T1CONbits.TCKPS= 0b00;    // Select 1:1 Prescaler
    TMR1 = 0x00;               // Clear timer register
    PR1 = 19800;                // Load the period value Interrupt period = 0.001 sec*/
    IPC0bits.T1IP = 7;      // Set Timer 1 Interrupt Priority Level
    IFS0bits.T1IF = 0;         // Clear Timer 1 Interrupt Flag
    IEC0bits.T1IE = 1;         // Enable Timer1 interrupt
    //T1CONbits.TON = 1;         // Enable Timer
}

void Init_TMR2(void)
{
    T2CONbits.TON = 0;         // Disable Timer
    T2CONbits.TCS = 0;         // Select internal instruction cycle clock
    T2CONbits.TGATE = 0;       // Disable Gated Timer mode
    T2CONbits.TCKPS= 0b00;    // Select 1:1 Prescaler
    TMR2 = 0x00;               // Clear timer register
    PR2 = 39600;                // Load the period value Interrupt period = 0.001 sec
    IPC1bits.T2IP = 2;      // Set Timer 2 Interrupt Priority Level
    IFS0bits.T2IF = 0;         // Clear Timer 2 Interrupt Flag
    IEC0bits.T2IE = 1;         // Enable Timer2 interrupt
    T2CONbits.TON = 1;         // Enable Timer2
}

/***************************************************************************/
/****************************UART Initialization****************************/
/***************************************************************************/   
void Init_UART(void)
{  
   U1BRG = 0;
    U1STAbits.URXISEL = 0b00;    //Receive interrupt when a character is received
    IPC2bits.U1RXIP = 5;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

   U2BRG =  Baud115200;
   U2MODE = 0x8000;
   U2STA = 0x0400;
 
   U3BRG = Baud57600;
    U3MODEbits.UARTEN = 1;
    U3STAbits.UTXEN = 1;
}

/***************************************************************************/
/***************************ADC Initialization******************************/
/***************************************************************************/
void Init_ADC(void)
{
    AD1CON1 = 0b0000000011101100;           //AD1 auto-convert sample 4 channels simultaneously SAMP auto set
    AD1CON2 = 0b0000001100001100;           //Convert 4 channels; interrupt after every 4th conversion 1352us
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = 31;       //Sample for 15 Tad before triggering conversion Sample time = 270.4 us
    AD1CON3bits.ADCS = 255;      //ADC conversion clock Tad = 6.2884706 us conversion time = 75.461647 us
    AD1CON4 = 0x0000;
    AD1CSSH = 0x0000;          
    AD1CSSL = 0x0000;
    AD1CHS0bits.CH0SA = 6;      //AN6 for CH0 +ve input
    AD1CHS0bits.CH0NA = 0;      //Vref- as CH0 -ve input
    AD1CHS123bits.CH123SA0 = 0; //CH1 positive input AN0
    AD1CHS123bits.CH123SA1 = 0; //CH2 positive input AN1
    AD1CHS123bits.CH123SA2 = 0; //CH3 positive input AN2
    IPC3bits.AD1IP = 3;         //AD1 priority 3
    IFS0bits.AD1IF = 0;         //Clear AD1 Flag
    IEC0bits.AD1IE = 1;         //Enable AD1 Interrupt
    AD1CHS123bits.CH123NA = 0;  //Vref- as CH123 -ve input
    AD1CON1bits.ADON = 1;       //Enable ADC module
    Delay_us(20);
}

void Delay_us(unsigned int delay)
{
    int i;
    for(i=0; i<delay;i++)
    {
        __asm__ volatile("repeat #40");
        __asm__ volatile("nop");
    }
}

/**************************************************************************/
/**************************I2C Initialization******************************/
/**************************************************************************/
void Init_I2C(void)
{
    I2C2CON = 0x2220;
    I2C2STAT = 0x0000;
    I2C2BRG = 34;
    I2C2CONbits.I2CEN = 1;
    I2C2STATbits.I2COV = 0;
}

/**************************************************************************/
/******************************SPI Initialization**************************/
/**************************************************************************/
void Init_SPI(void)
{
    SPI1STATbits.SPIEN = 0;            //Disable SPI module
    SPI1STATbits.SPISIDL = 1;          //SPI stop in Idle mode
    SPI1STATbits.SPIROV = 0;           //SPI clear overflow
    SPI1CON1 = 0x013E;                 //Byte-wide sample at the middle of data; Output changed from active to Idle
    SPI1STATbits.SPIEN = 1;
}
/****************************************************************************/
/******************************PutsUser function*****************************/
/****************************************************************************/
void putsUSER(char* message)
{
    while(*message != '\0')
    {
        while(U3STAbits.UTXBF);   //Check transmit buffer full or not
        U3TXREG = (unsigned int)*message++;
    }
    while(!U3STAbits.TRMT);       //Wait for empty transmit shift register
}

/***************************************************************************/
/*******************************printData***********************************/
/***************************************************************************/
unsigned char rv;
char message[MESSAGESIZE];
void printData(data_union *data)
{
    rv = sprintf(message,"IMU heading: %f, pitch: %f, roll: %f\r\n", data->myData.heading, data->myData.pitch, data->myData.roll);
    putsUSER(message);
    rv = sprintf(message,"IMU AX: %d, AY: %d, AZ: %d\r\n", data->myData.ax, data->myData.ay, data->myData.az);
    putsUSER(message);
    rv = sprintf(message,"IMU RX: %d, RY: %d, RZ: %d\r\n", data->myData.rx, data->myData.ry, data->myData.rz);
    putsUSER(message);
}
/*******************************************************************************/
/**********************************printDataCSV*********************************/
/*******************************************************************************/
void printDataCSV(data_union *d)
{
    //Sample number & Power0 - Power7
    rv = sprintf(message,"%d,%s,%s,%s,%s,%s,%s,%s,%s,",d->myData.sampleNumber,d->myData.Power0, d->myData.Power1,d->myData.Power2,d->myData.Power3,d->myData.Power4,d->myData.Power5,d->myData.Power6,d->myData.Power7);
    putsUSER(message);
    //IMU heading, pitch, roll
    rv = sprintf(message,"%f,%f,%f,",d->myData.heading,d->myData.pitch,d->myData.roll);
    putsUSER(message);
    //IMU linear acceleration x,y,z
    rv = sprintf(message,"%d,%d,%d,",d->myData.ax,d->myData.ay,d->myData.az);
    putsUSER(message);
    //IMU rotational velocity x,y,z
    rv = sprintf(message,"%d,%d,%d,",d->myData.rx,d->myData.ry,d->myData.rz);
    putsUSER(message);
    //Track0 - Track7
    rv = sprintf(message,"%s,%s,%s,%s,%s,%s,%s,%s\r\n",d->myData.Track0,d->myData.Track1,d->myData.Track2,d->myData.Track3,d->myData.Track4,d->myData.Track5,d->myData.Track6,d->myData.Track7);
    putsUSER(message);
}