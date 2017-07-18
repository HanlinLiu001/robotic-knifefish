#include <xc.h>
#include <string.h>
#include "MasterInitialization.h"
#include <libpic30.h> 
#include "flash.h"

/*** Write Enable command ***/
void flashWREN(void)
{
	unsigned char junk;
	
	FLASH_CS = 0;		            //Turn on flash
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = 0x06;                 //Send OP CODE - WriteEnable
	while(!SPI1STATbits.SPIRBF);	//Wait for Receive to complete
	junk = SPI1BUF;                 //Dummy read to clear receive buffer
	FLASH_CS = 1;
}
/*** Write Disable command*/
void flashWRDI(void)
{
	unsigned char junk;
	
	FLASH_CS = 0;		                 //Turn on flash
    while(SPI1STATbits.SPITBF);          //Wait for transmit started
	SPI1BUF = 0x04;                  	 //Send OP CODE - WriteDisable
	while(!SPI1STATbits.SPIRBF);          //Wait for transfer to complete
	junk = SPI1BUF;                      //Dummy read to clear receive buffer
	FLASH_CS = 1; 
}
/*** read status register 1 ***/
unsigned char getFlashStatus(void)
{
	unsigned char Status;
	
	FLASH_CS = 0;
    while(SPI1STATbits.SPITBF);         //Wait for transmit started
	SPI1BUF = 0x05;                  	//Send OP CODE - ReadStatus
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	Status = SPI1BUF;					 //Dummy read to clear receive buffer
    while(SPI1STATbits.SPITBF);         //Wait for transmit started
	SPI1BUF = 0x00;					    //Dummy write
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	Status = SPI1BUF;					//Read data from SPI buffer
	FLASH_CS = 1;
	return(Status);
}

unsigned char FlashBusy(void)
{
    unsigned char B;
    B = getFlashStatus() & 0x01;
    return (B);   //Returns 1 if busy
}

void writeDataToFlash(unsigned int address, data_union *data)
{
    unsigned char A2, A1, A0;           //Individual bytes of flash address. NOTE: A0 byte always 0x00
    unsigned char junk;
    int i;
    
    data->myData.sampleNumber = address;
    
    /*** setup address ***/
    A2 = ((unsigned char)(address >> 8)) & 0xFF;
    A1 = ((unsigned char)address) & 0xFF;
    A0 = 0x00;                          //Always zero because each sample is one page (256bytes max)
    
    /*** write out data ***/
	flashWREN();
    FLASH_CS = 0;						//Start write sequence 
    while(SPI1STATbits.SPITBF);         //Wait for transmit started
	SPI1BUF = 0x02;                  	//Send OP CODE - PageProgram
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
    //send out address
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A2;                  	    //Send MSByte of address
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A1;                    	//Send middle Byte of address
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A0;                  	    //Send LSByte of address
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
	//send out data
    for (i = 0; i < DATA_SIZE; i++)
    {
        while(SPI1STATbits.SPITBF);     //Wait for transmit started
        SPI1BUF = data->justBytes[i];   //Send data
        while(!SPI1STATbits.SPIRBF);     //Wait for transfer to complete
        junk = SPI1BUF;
    }
	FLASH_CS = 1;
    flashWRDI();
	while(FlashBusy());					//Wait for page programming to complete
}

void readDataFromFlash(unsigned int address, data_union *data)
{
    unsigned char A2, A1, A0;           //Individual bytes of flash address. NOTE: A0 byte always 0x00
    unsigned char junk;
    int i;
    
    /*** setup address ***/
    A2 = ((unsigned char)(address >> 8)) & 0xFF;
    A1 = ((unsigned char)address) & 0xFF;
    A0 = 0x00;                         //Always zero because each sample is one page (256bytes max)
    
    /*** send out command & address ***/
    FLASH_CS = 0;						//Start write sequence 
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = 0x03;                  	//Send OP CODE - Read Data
	while(!SPI1STATbits.SPIRBF);         //Wait for receive to complete
	junk = SPI1BUF;                     //Dummy read
    //send out address
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A2;                  	     //Send MSByte of address
	while(!SPI1STATbits.SPIRBF);          //Wait for transfer to complete
	junk = SPI1BUF;
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A1;                  	    //Send middle Byte of address
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A0;                   	//Send LSByte of address
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
	//read data in
    for (i = 0; i < DATA_SIZE; i++)
    {
        SPI1BUF = 0x00;                 //Send dummy data
        while(!SPI1STATbits.SPIRBF);     //Wait for transfer to complete
        data->justBytes[i] = SPI1BUF;
    }
	FLASH_CS = 1;
}

unsigned int readSampleNumberFromFlash(unsigned int address)
{
    unsigned char A2, A1, A0;           //Individual bytes of flash address. NOTE: A0 byte always 0x00
    unsigned char junk;
    unsigned int i, lowByte, highByte;
    
    /*** setup address ***/
    A2 = ((unsigned char)(address >> 8)) & 0xFF;
    A1 = ((unsigned char)address) & 0xFF;
    A0 = 0x00;  //always zero because each sample is one page (256bytes max)
    
    /*** send out command & address ***/
    FLASH_CS = 0;						//Start write sequence 
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = 0x03;                  	//Send OP CODE - ReadData
	while(!SPI1STATbits.SPIRBF);         //Wait for receive to complete
	junk = SPI1BUF;                     //Dummy read
    //send out address
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A2;                  	    //Send MSByte of address
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A1;                    	//Send middle Byte of address
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = A0;                   	//Send LSByte of address
	while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
	junk = SPI1BUF;
	//read low byte of sample number
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
    SPI1BUF = 0x00;                     //Send dummy data
    while(!SPI1STATbits.SPIRBF);         //Wait for transfer to complete
    lowByte = SPI1BUF & 0x00FF;
	//read high byte of sample number
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
    SPI1BUF = 0x00;                     //send dummy data
    while(!SPI1STATbits.SPIRBF);         //wait for transfer to complete
    highByte = SPI1BUF & 0x00FF;
	FLASH_CS = 1;
    i = (highByte << 8) | lowByte;
    return(i);
}

void eraseFlash(unsigned int address, unsigned int mode)
{
    unsigned char A2, A1, A0;           //Individual bytes of flash address. NOTE: A0 byte always 0x00
    unsigned char junk;
    
    flashWREN();
    FLASH_CS = 0;						//Start write sequence 

    if (mode == _ALL)
    {
        while(SPI1STATbits.SPITBF);     //Wait for transmit started
        SPI1BUF = 0xC7;                 //Send OP CODE - C7 - ChipErase    
    }
    else
    {    
        /*** setup address ***/
        A2 = ((unsigned char)(address >> 8)) & 0xFF;
        A1 = ((unsigned char)address) & 0xFF;
        A0 = 0x00;  //always zero because each sample is one page (256bytes max)

        /*** send out command & address ***/
        if (mode == _64K) 
        {
            while(SPI1STATbits.SPITBF);     //Wait for transmit started
            SPI1BUF = 0xD8;                //Send OP CODE - D8 - 64K block erase
        }
        else if (mode == _32K) 
        {
            while(SPI1STATbits.SPITBF);     //Wait for transmit started
            SPI1BUF = 0x52;                //Send OP CODE - 52 - 32K block erase
        }
        else 
        {
            while(SPI1STATbits.SPITBF);     //Wait for transmit started
            SPI1BUF = 0x20;                //Send OP CODE - 20 - 4K sector erase
        }
        while(!SPI1STATbits.SPIRBF);        //Wait for receive to complete
        junk = SPI1BUF;                    //Dummy read
        //send out address
        while(SPI1STATbits.SPITBF);     //Wait for transmit started
        SPI1BUF = A2;                  	   //Send MSByte of address
        while(!SPI1STATbits.SPIRBF);        //Wait for transfer to complete
        junk = SPI1BUF;
        while(SPI1STATbits.SPITBF);     //Wait for transmit started
        SPI1BUF = A1;                  	   //Send middle Byte of address
        while(!SPI1STATbits.SPIRBF);        //Wait for transfer to complete
        junk = SPI1BUF;
        SPI1BUF = A0;                  	   //Send LSByte of address
    }
    while(!SPI1STATbits.SPIRBF);            //Wait for transfer to complete
    junk = SPI1BUF;   
	FLASH_CS = 1;
    flashWRDI();
	while(FlashBusy())                      //Wait for erase to complete
    {
        RLED = LEDON;
        __delay_ms(100);
        RLED = LEDOFF;
        putsUSER(".");
    }
}

int checkFlashComms(void)
{
	unsigned char ManufacturerID; 	    //correct readback is 0xEF
	unsigned char ID_H;                 //correct readback is 0x40
	unsigned char ID_L;                 //correct readback is 0x18

	//read serial flash JEDEC ID info
	FLASH_CS = 0;
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = 0x9F;                  	//Send OP CODE - Instruction
	while(!SPI1STATbits.SPIRBF);		    //Wait for transfer to complete
	ManufacturerID = SPI1BUF;			//Dummy read to clear receive buffer status
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = 0x00;						//Dummy write
	while(!SPI1STATbits.SPIRBF);		    //Wait for transfer to complete
	ManufacturerID = SPI1BUF;			//Read data from SPI buffer
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = 0x00;						//Dummy write
	while(!SPI1STATbits.SPIRBF);		    //Wait for transfer to complete
	ID_H = SPI1BUF;				        //Read data from SPI buffer
    while(SPI1STATbits.SPITBF);     //Wait for transmit started
	SPI1BUF = 0x00;						//Dummy write
	while(!SPI1STATbits.SPIRBF);		    //Wait for transfer to complete
	ID_L = SPI1BUF;			            //Read data from SPI buffer
	FLASH_CS = 1;
	if ((ManufacturerID == 0xEF) && (ID_H == 0x40) && (ID_L == 0x18))
    {
		return (1);
    }
	else{
		return (0);
    }
}

void testFlash(void)
{
    data_union testdata;   //structure/union to hold one data sample
    data_union testread;

    //write, read, erase a few addresses to make sure the flash is working
    strcpy(testdata.myData.Power0, "0.1111");
    strcpy(testdata.myData.Track0, "22222");
    strcpy(testdata.myData.Power1, "0.2222");
    strcpy(testdata.myData.Track1, "33333");
    
    putsUSER("DATA TO FLASH LOWER 4K\r\n");
    writeDataToFlash(15,&testdata);
    printData(&testdata);
    readDataFromFlash(15,&testread);
    putsUSER("DATA FROM FLASH LOWER 4K\r\n");
    printData(&testread);
    eraseFlash(0,_4K);
    putsUSER("\r\n4K Erase complete\r\n");
    readDataFromFlash(15,&testread);
    putsUSER("DATA FROM FLASH LOWER 4K\r\n");
    printData(&testread);
    
    putsUSER("\r\nDATA TO FLASH LOWER 32K\r\n");
    writeDataToFlash(126,&testdata);
    printData(&testdata);
    readDataFromFlash(126,&testread);
    putsUSER("DATA FROM FLASH LOWER 32K\r\n");
    printData(&testread);
    eraseFlash(0,_32K);
    putsUSER("\r\n32K Erase complete\r\n");
    readDataFromFlash(126,&testread);
    putsUSER("DATA FROM FLASH LOWER 32K\r\n");
    printData(&testread);

    putsUSER("\r\nDATA TO FLASH LOWER 64K\r\n");
    writeDataToFlash(247,&testdata);
    printData(&testdata);
    readDataFromFlash(247,&testread);
    putsUSER("DATA FROM FLASH LOWER 64K\r\n");
    printData(&testread);
    eraseFlash(0,_64K);
    putsUSER("\r\n64K Erase complete\r\n");
    readDataFromFlash(247,&testread);
    putsUSER("DATA FROM FLASH LOWER 64K\r\n");
    printData(&testread);
   
    putsUSER("\r\nDATA TO FLASH NEXT 64K\r\n");
    writeDataToFlash(0x0100,&testdata);
    printData(&testdata);
    readDataFromFlash(0x0100,&testread);
    putsUSER("DATA FROM FLASH NEXT 64K\r\n");
    printData(&testread);
    eraseFlash(0,_64K);
    putsUSER("\r\nBottom 64K Erase complete\r\n");
    readDataFromFlash(0x0100,&testread);
    putsUSER("DATA FROM FLASH NEXT 64K\r\n");
    printData(&testread);

    putsUSER("ERASING ENTIRE CHIP\r\n");
    eraseFlash(0,_ALL);
    putsUSER("\r\nErase complete\r\n");
    readDataFromFlash(0x0100,&testread);
    putsUSER("DATA FROM FLASH ADDRESS 256\r\n");
    printData(&testread);   
}