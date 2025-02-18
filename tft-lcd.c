#include <msp430.h> 
#include <stdint.h>
#include "spi.h"
#include "time-ctrl.h"
#include "tft-lcd.h"
#include "fonts.h"


void tftConfig()
{
    spiConfig(1, 0);
    CSDIR  |= CSBIT;        // Active low
    CSOUT  |= CSBIT;

    RSTDIR |= RSTBIT;       // Active low
    RSTOUT |= RSTBIT;

    DCDIR  |= DCBIT;        // Data or Command
    DCOUT  |= DCBIT;        // 0: cmd, 1: data

    LEDDIR |= LEDBIT;       // LED always ON
    LEDOUT |= LEDBIT;

    tftInitSeq();
}


void tftInitSeq()
{
    // RESET Cycle
    RSTOUT &= ~RSTBIT;
    delay(15);
    RSTOUT |=  RSTBIT;
    delay(15);

    // Enable CS
    CSOUT &= ~CSBIT;

    sWcmd(0xCB);    //Power control A
    sWdata(0x39);
    sWdata(0x2C);
    sWdata(0x00);
    sWdata(0x34);
    sWdata(0x02);

    sWcmd(0xCF);    //Power control B
    sWdata(0x00);
    sWdata(0XC1);
    sWdata(0X30);

    sWcmd(0xE8);    //Driver timing control A
    sWdata(0x85);
    sWdata(0x00);
    sWdata(0x78);

    sWcmd(0xEA);    //Driver timing control B
    sWdata(0x00);
    sWdata(0x00);

    sWcmd(0xED);    //Power on sequence control
    sWdata(0x64);   // Custom data
    sWdata(0x03);   // Custom data
    sWdata(0X12);   // Custom data
    sWdata(0X81);   // Custom data

    sWcmd(0xF7);    //Pump ratio control
    sWdata(0x20);   // Custom data

    sWcmd(0xC0);    //Power control 1
    sWdata(0x23);   // 23 VRH[5:0] => ? => 100011==0x23 <=> 4.60 V

    sWcmd(0xC1);    //Power control 2
    sWdata(0x10);   // SAP[2:0];BT[3:0]

    sWcmd(0xC5);    //VCM control
    sWdata(0x3E);   // Contrast => 1111110 <=> 5.850 V
    sWdata(0x28);   // => 1010000 <=> -0.500 V

    sWcmd(0xC7);    //VCM control2
    sWdata(0x86);   //--

    sWcmd(0x36);    // Memory Access Control
    sWdata(0x28);   // 48    (28 -> mode paysage, 48 -> mode portrait)

    sWcmd(0x3A);    // Pixel format set
    sWdata(0x66);   // 55?

    sWcmd(0xB1);    //Frame Rate control
    sWdata(0x00);
    sWdata(0x18);   // 18

    sWcmd(0xB6);    // Display Function Control
    sWdata(0x08);
    sWdata(0x82);   // 82
    sWdata(0x27);

    sWcmd(0x11);    //Exit Sleep
    delay(120);

    sWcmd(0x29);    //Display on
    sWcmd(0x2c);

    // Disable CS
    CSOUT |=  CSBIT;
}

void tftWrite(uint8_t cmd, uint8_t * data, uint8_t nData)
{
    // Enable CS
    CSOUT &= ~CSBIT;

    // Write Command Address
    DCOUT &= ~DCBIT;            // DC == 0
    spiTransfer(cmd);

    // DUMMY transfer
    //spiTransfer(DUMMY);

    // Write Command Data
    DCOUT |=  DCBIT;            // DC == 1
    while(nData--)
        spiTransfer(data[nData]);
    //*data++ (starts with LSByte)

    // Disable CS
    CSOUT |=  CSBIT;
}


void tftRead(uint8_t cmd, uint8_t * data, uint8_t nData)
{
    // Enable CS
    CSOUT &= ~CSBIT;

    // Write Command
    DCOUT &= ~DCBIT;
    spiTransfer(cmd);

    // DUMMY transfer
    spiTransfer(DUMMY);

    // Read Data
    DCOUT |=  DCBIT;
    while(nData--)
        data[nData] = spiTransfer(DUMMY);

    // Disable CS
    CSOUT |=  CSBIT;

}

void sWcmd(uint8_t byte)
//   singleWriteComand
{
    DCOUT &= ~DCBIT;        // CMD
    spiTransfer(byte);
}

void sWdata(uint8_t byte)
//   singleWriteData
{
    DCOUT |=  DCBIT;        // DATA
    spiTransfer(byte);
    DCOUT &= ~DCBIT;        // return to 0
}

void Address_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    sWcmd(0x2a);
        sWdata(x1>>8);
        sWdata(x1);
        sWdata(x2>>8);
        sWdata(x2);
    sWcmd(0x2b);
        sWdata(y1>>8);
        sWdata(y1);
        sWdata(y2>>8);
        sWdata(y2);
    sWcmd(0x2c);
}

void draw_pixel (uint16_t cr,uint16_t cg,uint16_t cb) 
{
    sWdata(cr);
    sWdata(cg);
    sWdata(cb);
}

void drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t ii,mm;
    // Enable CS        
    CSOUT &= ~CSBIT;

    Address_set(x1,y1,x2,y2);
    for(ii=0;ii<(x2-x1);ii++)
    for(mm=0;mm<(y2-y1);mm++)
    {
        draw_pixel(0, 0xffff, 0);
    }

    // Disable CS
    CSOUT |=  CSBIT;
}

void LCD_Clear(uint16_t cr, uint16_t cg, uint16_t cb, int screen_orientation)
{
    uint16_t ii,mm;
    
    // Enable CS
    CSOUT &= ~CSBIT;

    if (!screen_orientation)
    {
        Address_set(0,0,240,320);
        for(ii=0;ii<240;ii++)
        for(mm=0;mm<320;mm++)
        {
            draw_pixel(cr, cg, cb);
        }
    }
    else
    {
        Address_set(0,0,320,240);
        for(ii=0;ii<320;ii++)
        for(mm=0;mm<240;mm++)
        {
            draw_pixel(cr, cg, cb);
        }
    }


    // Disable CS
    CSOUT |=  CSBIT;
}

void LCD_Arc()
{
    uint8_t ii,mm;
    uint16_t cb = 0, cg = 0, cr = 0;
    // Enable CS
    CSOUT &= ~CSBIT;

    Address_set(0,0,240,320);
    for(ii=0;ii<320;ii++) {
        if (ii<80) cr=3*(ii%80);
        else if (ii<160) cr=3*(80-(ii-80)%80);
        else if (ii<240) cb=3*((ii-160)%80);
        else cb=3*(80-(ii-240)%80);
        for(mm=0;mm<240;mm++)
        {
            cg = mm%240;
            draw_pixel(cr, cg, cb);
        }
    }
    // Disable CS
    CSOUT |=  CSBIT;
}

void LCD_Carre()
{
    uint8_t ii;
    // Enable CS
    CSOUT &= ~CSBIT;

    Address_set(10,10,20,20);
    for(ii=0;ii<100;ii++) {
      draw_pixel(0xff, 0xff, 0xff);
    }
    // Disable CS
    CSOUT |=  CSBIT;
}

void drawPixel(uint16_t x, uint16_t y, uint16_t cr, uint16_t cg, uint16_t cb)
{
    // Enable CS
    CSOUT &= ~CSBIT;

    Address_set(x, y, x, y);
    draw_pixel(cr, cg, cb);

    // Disable CS
    CSOUT |=  CSBIT;
}

void drawChar(uint16_t x, uint16_t y, char c, uint16_t cr, uint16_t cg, uint16_t cb)
{
    uint8_t col, row;

    for (col = 0; col < 5; col++) {
        uint8_t line = font5x7[c][col]; // Ajustando indice do caractere
        for (row = 0; row < 7; row++) {
            if (line & (1 << row)) {
                drawPixel(x + col, y + row, cr, cg, cb);
            }
        }
    }
}

void drawString(uint16_t x, uint16_t y, const char *str, uint16_t cr, uint16_t cg, uint16_t cb)
{
    while (*str) {
        drawChar(x, y, *str, cr, cg, cb);
        x += 6; // Espacamento entre caracteres (5 pixels + 1 de espaco)
        str++;
    }
}

void drawChar_4x(uint16_t x, uint16_t y, char c, uint16_t cr, uint16_t cg, uint16_t cb)
{
    uint16_t col, row;

    for (col = 0; col < 20; col++) {
        volatile uint32_t line = font20x28[c][col]; // Ajustando indice do caractere
        for (row = 0; row < 28; row++) {
            if (line & (1 << row)) {
                drawPixel(x + col, y + row, cr, cg, cb);
            }
        }
    }
}

void drawString_4x(uint16_t x, uint16_t y, const char *str, uint16_t cr, uint16_t cg, uint16_t cb)
{
    while (*str) {
        drawChar_4x(x, y, *str, cr, cg, cb);
        x += 10; // Espacamento entre caracteres (5 pixels + 1 de espaco)
        str++;
    }
}









