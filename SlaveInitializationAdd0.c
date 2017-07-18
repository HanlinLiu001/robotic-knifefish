/* 
 * File:   InitPWM.c
 * Author: HarleyLiu
 *
 * Created on April 4, 2016, 5:17 PM
 */
#include<xc.h>
#include<p33EP512GM304.h>
#include<qei32.h>
#include<dsp.h>
#include"SlaveInitializationAdd0.h"

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
/******************External Interrupt Initialization*********************/
/************************************************************************/
void Init_Int1()
{
  INTCON2bits.GIE = 1;             //INT1 interrupt Enable
  IPC5bits.INT1IP = 0x07;          //Set Interrupt priority          
  IFS1bits.INT1IF = 0;             //Clear Interrupt flag  
  IEC1bits.INT1IE = 1;             //Enable External ISR
}
/************************************************************************/
/******************* Initialize Timer ***********************************/
/************************************************************************/
void Init_TMR1(void)
{
    T1CONbits.TON = 0;         // Disable Timer
    T1CONbits.TCS = 0;         // Select internal instruction cycle clock
    T1CONbits.TGATE = 0;       // Disable Gated Timer mode
    /*T1CONbits.TCKPS= 0b10;    // Select 1:1 Prescaler
    TMR1 = 0x00;               // Clear timer register
    PR1 = 19800;                // Load the period value Interrupt period = 0.001 sec
    IPC0bits.T1IP = 5;      // Set Timer 1 Interrupt Priority Level
    //IFS0bits.T1IF = 0;         // Clear Timer 1 Interrupt Flag
    //IEC0bits.T1IE = 1;         // Enable Timer1 interrupt
    //T1CONbits.TON = 1;         // Enable Timer*/
}

/************************************************************************/
/***********************Encoder Initialization***************************/
/************************************************************************/
unsigned int QEICON;
unsigned int QEIIOC;
unsigned int QEISTAT;
qeiCounter Max_encounter;
qeiCounter Ini_encounter;
qeiCounter Min_encounter;

void Init_Encoder(void)
{
Close32bitQEI1();  
Close32bitQEI2(); 
Max_encounter.l = 0x0000FFFF;
Ini_encounter.l = 0x00000000;
Min_encounter.l = 0x00000000;
QEICON = QEI_COUNTER_QEI_MODE & QEI_GATE_DISABLE & QEI_COUNT_POSITIVE & QEI_INPUT_PRESCALE_1 &
        QEI_INDEX_MATCH_NO_EFFECT & QEI_POS_COUNT_INIT_MODULO_COUNT & QEI_IDLE_CON & QEI_COUNTER_ENABLE;

QEIIOC = QEI_QEA_POL_NON_INVERTED & QEI_QEB_POL_NON_INVERTED & QEI_INDX_POL_NON_INVERTED &
         QEI_HOM_POL_NON_INVERTED & QEI_QEA_QEB_NOT_SWAPPED & QEI_COMPARE_HIGH_OUTPUT_DISABLE &
         QEI_DIF_FLTR_PRESCALE_32 & QEI_DIG_FLTR_ENABLE & QEI_POS_COUNT_TRIG_DISABLE;

QEISTAT = QEI_INDEX_INTERRUPT_DISABLE & QEI_HOME_INTERRUPT_DISABLE & QEI_VELO_OVERFLOW_INTERRUPT_DISABLE &
          QEI_POS_INIT_INTERRUPT_DISABLE & QEI_POS_OVERFLOW_INTERRUPT_DISABLE & QEI_POS_LESS_EQU_INTERRUPT_ENABLE &
          QEI_POS_GREAT_EQU_INTERRUPT_ENABLE;
ConfigInt32bitQEI1(QEI_INT_PRI_4 & QEI_INT_DISABLE);
ConfigInt32bitQEI2(QEI_INT_PRI_4 & QEI_INT_DISABLE);
}

/*************************************************************************/
/***************************PID Initialization****************************/
/*************************************************************************/
    float PID1_output;
    float PID2_output;
    tPID MSPID1;
    fractional abcCoefficient1[3]__attribute__((space(xmemory)));  
    fractional controlHistory1[3]__attribute__((eds,space(ymemory)));    
    fractional kCoeffs1[] = {0,0,0}; 
     
    tPID OMPID1;
    fractional abcCoefficient2[3]__attribute__((space(xmemory)));  
    fractional controlHistory2[3]__attribute__((eds,space(ymemory)));    
    fractional kCoeffs2[] = {0,0,0}; 
    
    tPID MSPID2;
    fractional abcCoefficient3[3]__attribute__((space(xmemory)));  
    fractional controlHistory3[3]__attribute__((eds,space(ymemory)));    
    fractional kCoeffs3[] = {0,0,0}; 
    
    tPID OMPID2;
    fractional abcCoefficient4[3]__attribute__((space(xmemory)));  
    fractional controlHistory4[3]__attribute__((eds,space(ymemory)));    
    fractional kCoeffs4[] = {0,0,0}; 
    
    tPID GBPID1;
    fractional abcCoefficient5[3]__attribute__((space(xmemory)));  
    fractional controlHistory5[3]__attribute__((eds,space(ymemory)));    
    fractional kCoeffs5[] = {0,0,0};
    
    tPID GBPID2;
    fractional abcCoefficient6[3]__attribute__((space(xmemory)));  
    fractional controlHistory6[3]__attribute__((eds,space(ymemory)));    
    fractional kCoeffs6[] = {0,0,0};
    
void Init_PID(void){
    //Mechanical Stop PID Motor1    
    MSPID1.abcCoefficients = &abcCoefficient1[0];      /*Set up pointer to derived coefficients*/       
    MSPID1.controlHistory = &controlHistory1[0];     /*Set up pointer to controller history samples */ 
    PIDInit(&MSPID1);
    kCoeffs1[0] = Q15(0.75);    
    kCoeffs1[1] = Q15(0.005);    
    kCoeffs1[2] = Q15(0.1);   
    PIDCoeffCalc(&kCoeffs1[0], &MSPID1);
    //Oscillating Motion PID Motor1
    OMPID1.abcCoefficients = &abcCoefficient2[0];      /*Set up pointer to derived coefficients*/       
    OMPID1.controlHistory = &controlHistory2[0];     /*Set up pointer to controller history samples */ 
    PIDInit(&OMPID1);

    //Mechanical Stop PID Motor2
    MSPID2.abcCoefficients = &abcCoefficient3[0];      /*Set up pointer to derived coefficients */       
    MSPID2.controlHistory = &controlHistory3[0];     /*Set up pointer to controller history samples */ 
    PIDInit(&MSPID2);
    kCoeffs3[0] = Q15(0.75);    
    kCoeffs3[1] = Q15(0.005);    
    kCoeffs3[2] = Q15(0.1);   
    PIDCoeffCalc(&kCoeffs3[0], &MSPID2);
    //Oscillating Motion PID Motor2
    OMPID2.abcCoefficients = &abcCoefficient4[0];      /*Set up pointer to derived coefficients */       
    OMPID2.controlHistory = &controlHistory4[0];     /*Set up pointer to controller history samples */ 
    PIDInit(&OMPID2);
    
    //Go back to zero PID Motor1    
    GBPID1.abcCoefficients = &abcCoefficient5[0];      /*Set up pointer to derived coefficients*/       
    GBPID1.controlHistory = &controlHistory5[0];     /*Set up pointer to controller history samples */ 
    PIDInit(&GBPID1);
    kCoeffs5[0] = Q15(0.75);    
    kCoeffs5[1] = Q15(0.005);    
    kCoeffs5[2] = Q15(0.1);   
    PIDCoeffCalc(&kCoeffs5[0], &GBPID1);
    
    //Go back to zero PID Motor2   
    GBPID2.abcCoefficients = &abcCoefficient6[0];      /*Set up pointer to derived coefficients*/       
    GBPID2.controlHistory = &controlHistory6[0];     /*Set up pointer to controller history samples */ 
    PIDInit(&GBPID2);
    kCoeffs6[0] = Q15(0.75);    
    kCoeffs6[1] = Q15(0.005);    
    kCoeffs6[2] = Q15(0.1);   
    PIDCoeffCalc(&kCoeffs6[0], &GBPID2);
}

/**************************************************************************/
/***************************PWM Initialization*****************************/
/**************************************************************************/
int PWM2_set;
int PWM6_set;
int Pos1_ref;
int Pos2_ref;

const int MStable[] =
{
0,-64,-128,-192,-256,-320,-384,-448,-512,-576,
-640,-704,-768,-832,-896,-960,-1024,-1088,-1152,-1216,
-1280,-1344,-1408,-1472,-1536,-1600,-1664,-1728,-1792,-1856,
-1920,-1984,-2048,-2112,-2176,-2240,-2304,-2368,-2432,-2496,
-2560,-2624,-2688,-2752,-2816,-2880,-2944,-3008,-3072,-3136,
-3200,-3264,-3328,-3392,-3456,-3520,-3584,-3648,-3712,-3776,
-3840,-3904,-3968,-4032,-4096,-4160,-4224,-4288,-4352,-4416,
-4480,-4544,-4608,-4672,-4736,-4800,-4864,-4928,-4992,-5056,
-5120,-5184,-5248,-5312,-5376,-5440,-5504,-5568,-5632,-5696,
-5760,-5824,-5888,-5952,-6016,-6080,-6144,-6208,-6272,-6336,
-6400,-6464,-6528,-6592,-6656,-6720,-6784,-6848,-6912,-6976,
-7040,-7104,-7168,-7232,-7296,-7360,-7424,-7488,-7552,-7616,
-7680,-7744,-7808,-7872,-7936,-8000,-8064,-8128,-8192,-8256,
-8320,-8384,-8448,-8512,-8576,-8640,-8704,-8768,-8832,-8896,
-8960,-9024,-9088,-9152,-9216,-9280,-9344,-9408,-9472,-9536,
-9600,-9664,-9728,-9792,-9856,-9920,-9984,-10048,-10112,-10176,
-10240,-10304,-10368,-10432,-10496,-10560,-10624,-10688,-10752,-10816,
-10880,-10944,-11008,-11072,-11136,-11200,-11264,-11328,-11392,-11456,
-11520,-11584,-11648,-11712,-11776,-11840,-11904,-11968,-12032,-12096,
-12160,-12224,-12288,-12352,-12416,-12480,-12544,-12608,-12672,-12736,
-12800,-12864,-12928,-12992,-13056,-13120,-13184,-13248,-13312,-13376,
-13440,-13504,-13568,-13632,-13696,-13760,-13824,-13888,-13952,-14016,
-14080,-14144,-14208,-14272,-14336,-14400,-14464,-14528,-14592,-14656,
-14720,-14784,-14848,-14912,-14976,-15040,-15104,-15168,-15232,-15296,
-15360,-15424,-15488,-15552,-15616,-15680,-15744,-15808,-15872,-15936,
-16000,-16064,-16128,-16192,-16256,-16320,-16384
};

const unsigned int Sinetable[] =
{
5461,5595,5729,5863,5996,6129,6262,6395,6526,6658,
6788,6918,7046,7174,7301,7426,7551,7674,7796,7916,
8035,8153,8269,8383,8495,8605,8714,8821,8925,9028,
9128,9227,9323,9416,9507,9596,9682,9766,9847,9926,
10002,10075,10145,10213,10277,10339,10398,10453,10506,10556,
10603,10646,10687,10724,10758,10789,10817,10842,10863,10881,
10896,10907,10915,10920,10922,10920,10915,10907,10896,10881,
10863,10842,10817,10789,10758,10724,10687,10646,10603,10556,
10506,10453,10398,10339,10277,10213,10145,10075,10002,9926,
9847,9766,9682,9596,9507,9416,9323,9227,9128,9028,
8925,8821,8714,8605,8495,8383,8269,8153,8035,7916,
7796,7674,7551,7426,7301,7174,7046,6918,6788,6658,
6526,6395,6262,6129,5996,5863,5729,5595,5461,5327,
5193,5059,4926,4793,4660,4527,4396,4264,4134,4004,
3876,3748,3621,3496,3371,3248,3126,3006,2887,2769,
2653,2539,2427,2317,2208,2101,1997,1894,1794,1695,
1599,1506,1415,1326,1240,1156,1075,996,920,847,
777,709,645,583,524,469,416,366,319,276,
235,198,164,133,105,80,59,41,26,15,
7,2,0,2,7,15,26,41,59,80,
105,133,164,198,235,276,319,366,416,469,
524,583,645,709,777,847,920,996,1075,1156,
1240,1326,1415,1506,1599,1695,1794,1894,1997,2101,
2208,2317,2427,2539,2653,2769,2887,3006,3126,3248,
3371,3496,3621,3748,3876,4004,4134,4264,4396,4527,
4660,4793,4926,5059,5193,5327,5461
};
const int Weamptable0[] =
{
6372,6528,6684,6840,6996,7151,7306,7461,7615,7768,
7920,8071,8221,8370,8518,8665,8810,8953,9096,9236,
9375,9512,9647,9780,9911,10040,10167,10291,10414,10533,
10650,10765,10877,10986,11092,11196,11297,11394,11489,11581,
11669,11754,11837,11915,11991,12063,12131,12196,12258,12316,
12371,12421,12469,12512,12552,12588,12621,12649,12674,12695,
12712,12726,12735,12741,12743,12741,12735,12726,12712,12695,
12674,12649,12621,12588,12552,12512,12469,12421,12371,12316,
12258,12196,12131,12063,11991,11915,11837,11754,11669,11581,
11489,11394,11297,11196,11092,10986,10877,10765,10650,10533,
10414,10291,10167,10040,9911,9780,9647,9512,9375,9236,
9096,8953,8810,8665,8518,8370,8221,8071,7920,7768,
7615,7461,7306,7151,6996,6840,6684,6528,6372,6215,
6059,5903,5747,5592,5437,5282,5128,4975,4823,4672,
4522,4373,4225,4078,3933,3790,3647,3507,3368,3231,
3096,2963,2832,2703,2576,2452,2329,2210,2093,1978,
1866,1757,1651,1547,1446,1349,1254,1162,1074,989,
906,828,752,680,612,547,485,427,372,322,
274,231,191,155,122,94,69,48,31,17,
8,2,0,2,8,17,31,48,69,94,
122,155,191,231,274,322,372,427,485,547,
612,680,752,828,906,989,1074,1162,1254,1349,
1446,1547,1651,1757,1866,1978,2093,2210,2329,2452,
2576,2703,2832,2963,3096,3231,3368,3507,3647,3790,
3933,4078,4225,4373,4522,4672,4823,4975,5128,5282,
5437,5592,5747,5903,6059,6215,6372    
};
const int Weamptable1[] = 
{
6080,6229,6378,6527,6676,6824,6972,7119,7266,7412,
7557,7702,7845,7987,8128,8268,8407,8544,8680,8814,
8946,9077,9206,9333,9458,9581,9702,9821,9937,10051,
10163,10272,10379,10483,10585,10684,10780,10873,10964,11051,
11135,11217,11295,11370,11442,11511,11576,11638,11697,11753,
11805,11853,11898,11940,11978,12012,12043,12070,12094,12114,
12131,12144,12153,12158,12160,12158,12153,12144,12131,12114,
12094,12070,12043,12012,11978,11940,11898,11853,11805,11753,
11697,11638,11576,11511,11442,11370,11295,11217,11135,11051,
10964,10873,10780,10684,10585,10483,10379,10272,10163,10051,
9937,9821,9702,9581,9458,9333,9206,9077,8946,8814,
8680,8544,8407,8268,8128,7987,7845,7702,7557,7412,
7266,7119,6972,6824,6676,6527,6378,6229,6080,5931,
5782,5633,5484,5336,5188,5041,4894,4748,4603,4458,
4315,4173,4032,3892,3753,3616,3480,3346,3214,3083,
2954,2827,2702,2579,2458,2339,2223,2109,1997,1888,
1781,1677,1575,1476,1380,1287,1196,1109,1025,943,
865,790,718,649,584,522,463,407,355,307,
262,220,182,148,117,90,66,46,29,16,
7,2,0,2,7,16,29,46,66,90,
117,148,182,220,262,307,355,407,463,522,
584,649,718,790,865,943,1025,1109,1196,1287,
1380,1476,1575,1677,1781,1888,1997,2109,2223,2339,
2458,2579,2702,2827,2954,3083,3214,3346,3480,3616,
3753,3892,4032,4173,4315,4458,4603,4748,4894,5041,
5188,5336,5484,5633,5782,5931,6080
};
const int Inamptable0[] =
{
2003,2052,2101,2150,2199,2248,2296,2345,2393,2441,
2489,2537,2584,2631,2677,2723,2769,2814,2859,2903,
2946,2990,3032,3074,3115,3156,3195,3235,3273,3310,
3347,3383,3418,3453,3486,3519,3550,3581,3611,3640,
3668,3694,3720,3745,3769,3791,3813,3833,3853,3871,
3888,3904,3919,3932,3945,3956,3967,3976,3983,3990,
3995,4000,4003,4004,4005,4004,4003,4000,3995,3990,
3983,3976,3967,3956,3945,3932,3919,3904,3888,3871,
3853,3833,3813,3791,3769,3745,3720,3694,3668,3640,
3611,3581,3550,3519,3486,3453,3418,3383,3347,3310,
3273,3235,3195,3156,3115,3074,3032,2990,2946,2903,
2859,2814,2769,2723,2677,2631,2584,2537,2489,2441,
2393,2345,2296,2248,2199,2150,2101,2052,2003,1953,
1904,1855,1806,1757,1709,1660,1612,1564,1516,1468,
1421,1374,1328,1282,1236,1191,1146,1102,1059,1015,
973,931,890,849,810,770,732,695,658,622,
587,552,519,486,455,424,394,365,337,311,
285,260,236,214,192,172,152,134,117,101,
86,73,60,49,38,29,22,15,10,5,
2,1,0,1,2,5,10,15,22,29,
38,49,60,73,86,101,117,134,152,172,
192,214,236,260,285,311,337,365,394,424,
455,486,519,552,587,622,658,695,732,770,
810,849,890,931,973,1015,1059,1102,1146,1191,
1236,1282,1328,1374,1421,1468,1516,1564,1612,1660,
1709,1757,1806,1855,1904,1953,2003
};
const int Inamptable1[] = 
{
2294,2350,2406,2462,2518,2574,2630,2686,2741,2796,
2851,2905,2959,3013,3066,3119,3171,3223,3274,3325,
3375,3424,3473,3521,3568,3614,3660,3705,3748,3792,
3834,3875,3915,3955,3993,4030,4066,4102,4136,4169,
4200,4231,4261,4289,4316,4342,4367,4390,4412,4433,
4453,4471,4488,4504,4518,4531,4543,4553,4562,4570,
4576,4581,4584,4586,4587,4586,4584,4581,4576,4570,
4562,4553,4543,4531,4518,4504,4488,4471,4453,4433,
4412,4390,4367,4342,4316,4289,4261,4231,4200,4169,
4136,4102,4066,4030,3993,3955,3915,3875,3834,3792,
3748,3705,3660,3614,3568,3521,3473,3424,3375,3325,
3274,3223,3171,3119,3066,3013,2959,2905,2851,2796,
2741,2686,2630,2574,2518,2462,2406,2350,2294,2237,
2181,2125,2069,2013,1957,1901,1846,1791,1736,1682,
1628,1574,1521,1468,1416,1364,1313,1262,1212,1163,
1114,1066,1019,973,927,882,839,795,753,712,
672,632,594,557,521,485,451,418,387,356,
326,298,271,245,220,197,175,154,134,116,
99,83,69,56,44,34,25,17,11,6,
3,1,0,1,3,6,11,17,25,34,
44,56,69,83,99,116,134,154,175,197,
220,245,271,298,326,356,387,418,451,485,
521,557,594,632,672,712,753,795,839,882,
927,973,1019,1066,1114,1163,1212,1262,1313,1364,
1416,1468,1521,1574,1628,1682,1736,1791,1846,1901,
1957,2013,2069,2125,2181,2237,2294
};

void Init_PWM(void){
    // PWM Time Base Control Register Prescaler is 1:4; Fosc = 81.1 MHz; Period PTPER = 4000; PWM frequency 20kHz
    PTCONbits.PTEN = 0;          //Timer Enable bit: disable PWM
    PTPER = 4000;                // Period Value bits 4000
    PTCON2bits.PCLKDIV = 0b000;  // Pre scaler is 1:1
    PWMCON2 = 0x0080;            // PTPER holds the PWM period; Dead time function disabled; PDC and SDC hold the PWM2 duty cycle
    IOCON2 = 0xCC00;             // PENH high PENL high True Independent PWM2 Output mode
    FCLCON2 = 0x0003;            // Fault input is disabled
    PWMCON6 = 0x0080;            // PTPER holds PWM period PDC and SDC hold PWM6 duty cycle
    IOCON6 = 0xCC00;             // PENH high PENL high True Independent PWM6 Output mode
    FCLCON6 = 0x0003;            // Fault input is disabled
    IOCON1 = 0;                  // Set PWM1 to GPIO
    IOCON3 = 0;                  // Set PWM3 to GPIO
    IOCON4 = 0;                  // Set PWM4 to GPIO 
    IOCON5 = 0;                  // Set PWM5 to GPIO
    PTCONbits.PTEN = 1;          // PWM Enable bit: enable PWM
}

/***************************************************************************/
/****************************UART Initialization****************************/
/***************************************************************************/   
void Init_UART(void)
{
    U1BRG = 0;
    U1STAbits.URXISEL = 0b00;    //Receive interrupt when one character is received
    IPC2bits.U1RXIP = 5;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

/****************************************************************************/
/********************************ADC Initialization**************************/
/****************************************************************************/
void Init_ADC(void)
{
    AD1CON1 = 0b0000000011100100;         //ADC auto convert; 
    AD1CON2 = 0b0000010000011100;         //Scans inputs for CH0; interrupt after 8th conversion 8*(Tsamp+Tcon)=1.086ms
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = 31;               //Sample time = 97.854 us
    AD1CON3bits.ADCS = 127;               //ADC conversion clock Tad = 3.1566 us; Conversion time = 12*Tad = 37.88 us
    AD1CON4 = 0x0000;
    AD1CHS0bits.CH0SA = 0;
    AD1CHS0bits.CH0NA = 0;
    AD1CSSH = 0b0011000000000000;        //AN28-Motor2 current AN29-Motor1 current
    AD1CSSL = 0b0000000001000010;        //AN1-Motor1/2 voltage AN6-Leak sensor
    IPC3bits.AD1IP = 3;                  //AD1 priority 3
    
    Delay_us(20);
}

int i;
void Delay_us(unsigned int delay)
{
    for(i=0; i<delay;i++)
    {
        __asm__ volatile("repeat #40");
        __asm__ volatile("nop");
    }
}