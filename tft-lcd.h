/// tft-lcd.h - Definições e funções para controle do display TFT LCD
#ifndef __TFT_LCD_H
#define __TFT_LCD_H

#include <msp430.h> 
#include <stdint.h>

#define DUMMY   0xFF  // Valor padrão para transferências SPI fictícias

// Definições de pinos de controle do LCD
#define CSDIR   P8DIR
#define CSOUT   P8OUT
#define CSBIT   BIT1

#define RSTDIR  P2DIR
#define RSTOUT  P2OUT
#define RSTBIT  BIT3

#define LEDDIR  P2DIR
#define LEDOUT  P2OUT
#define LEDBIT  BIT7

#define DCDIR   P2DIR
#define DCOUT   P2OUT
#define DCBIT   BIT6


void tftConfig ();
void tftInitSeq();

void sWcmd      (uint8_t byte);
void sWdata     (uint8_t byte);
void tftWrite   (uint8_t cmd, uint8_t * data, uint8_t nData);
void tftRead    (uint8_t cmd, uint8_t * data, uint8_t nData);

void Address_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void draw_pixel (uint16_t cr,uint16_t cg,uint16_t cb);
void LCD_Clear  (uint16_t cr, uint16_t cv, uint16_t cb, int screen_orientation);

void LCD_Arc   ();
void LCD_Carre ();

void drawPixel  (uint16_t x, uint16_t y, uint16_t cr, uint16_t cg, uint16_t cb);
void drawChar   (uint16_t x, uint16_t y, char c, uint16_t cr, uint16_t cg, uint16_t cb);
void drawString (uint16_t x, uint16_t y, const char *str, uint16_t cr, uint16_t cg, uint16_t cb);

void drawChar_4x    (uint16_t x, uint16_t y, char c, uint16_t cr, uint16_t cg, uint16_t cb);
void drawString_4x  (uint16_t x, uint16_t y, const char *str, uint16_t cr, uint16_t cg, uint16_t cb);
void drawRect       (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#endif  // __TFT_LCD_H
