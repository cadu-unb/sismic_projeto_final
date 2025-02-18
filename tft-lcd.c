#include <msp430.h> 
#include <stdint.h>
#include "spi.h"
#include "time-ctrl.h"
#include "tft-lcd.h"
#include "fonts.h"


void tftConfig()
{
    spiConfig(1, 0);       // Configura o SPI no modo mestre (1) e desabilita interrupções (0)
    CSDIR  |= CSBIT;       // Define o pino CS (Chip Select) como saída
    CSOUT  |= CSBIT;       // Inicializa CS em nível alto (desativado)
    RSTDIR |= RSTBIT;      // Define o pino RST (Reset) como saída
    RSTOUT |= RSTBIT;      // Inicializa RST em nível alto (desativado)
    DCDIR  |= DCBIT;       // Define o pino DC (Data/Command) como saída
    DCOUT  |= DCBIT;       // Inicializa DC em nível alto (modo Data)
    LEDDIR |= LEDBIT;      // Define o pino LED como saída
    LEDOUT |= LEDBIT;      // Liga o LED do display
    tftInitSeq();          // Executa a sequência de inicialização do display
}


void tftInitSeq()
{
    // Ciclo de RESET
    RSTOUT &= ~RSTBIT;     // Ativa o reset (nível baixo)
    delay(15);             // Aguarda 15ms
    RSTOUT |=  RSTBIT;     // Desativa o reset (nível alto)
    delay(15);             // Aguarda mais 15ms

    // Habilita o chip selecionando-o (CS em nível baixo)
    CSOUT &= ~CSBIT;

    // Envia uma série de comandos e dados para configurar o display
    sWcmd(0xCB); sWdata(0x39); sWdata(0x2C); sWdata(0x00); sWdata(0x34); sWdata(0x02);
    sWcmd(0xCF); sWdata(0x00); sWdata(0xC1); sWdata(0x30);
    sWcmd(0xE8); sWdata(0x85); sWdata(0x00); sWdata(0x78);
    sWcmd(0xEA); sWdata(0x00); sWdata(0x00);
    sWcmd(0xED); sWdata(0x64); sWdata(0x03); sWdata(0x12); sWdata(0x81);
    sWcmd(0xF7); sWdata(0x20);
    sWcmd(0xC0); sWdata(0x23);
    sWcmd(0xC1); sWdata(0x10);
    sWcmd(0xC5); sWdata(0x3E); sWdata(0x28);
    sWcmd(0xC7); sWdata(0x86);
    sWcmd(0x36); sWdata(0x28); // Modo de orientação da tela
    sWcmd(0x3A); sWdata(0x66); // Formato de pixel
    sWcmd(0xB1); sWdata(0x00); sWdata(0x18);
    sWcmd(0xB6); sWdata(0x08); sWdata(0x82); sWdata(0x27);
    sWcmd(0x11); delay(120);   // Sai do modo sleep
    sWcmd(0x29);               // Liga o display
    sWcmd(0x2C);               // Modo de escrita de memória

    // Desabilita o chip (CS em nível alto)
    CSOUT |= CSBIT;
}

void tftWrite(uint8_t cmd, uint8_t * data, uint8_t nData)
{
    CSOUT &= ~CSBIT;       // Habilita o chip (CS em nível baixo)
    DCOUT &= ~DCBIT;       // Define o modo Comando (DC = 0)
    spiTransfer(cmd);      // Envia o comando
    DCOUT |= DCBIT;        // Define o modo Dados (DC = 1)
    while(nData--)         // Envia todos os bytes de dados
        spiTransfer(data[nData]);
    CSOUT |= CSBIT;        // Desabilita o chip (CS em nível alto)
}


void tftRead(uint8_t cmd, uint8_t * data, uint8_t nData)
{
    CSOUT &= ~CSBIT;       // Habilita o chip (CS em nível baixo)
    DCOUT &= ~DCBIT;       // Define o modo Comando (DC = 0)
    spiTransfer(cmd);      // Envia o comando
    spiTransfer(DUMMY);    // Transfere um byte fictício
    DCOUT |= DCBIT;        // Define o modo Dados (DC = 1)
    while(nData--)         // Lê todos os bytes de dados
        data[nData] = spiTransfer(DUMMY);
    CSOUT |= CSBIT;        // Desabilita o chip (CS em nível alto)
}

void sWcmd(uint8_t byte)
{
    DCOUT &= ~DCBIT;       // Define o modo Comando (DC = 0)
    spiTransfer(byte);     // Envia o comando
}

void sWdata(uint8_t byte)
{
    DCOUT |= DCBIT;        // Define o modo Dados (DC = 1)
    spiTransfer(byte);     // Envia o dado
    DCOUT &= ~DCBIT;       // Retorna ao modo Comando (DC = 0)
}

void Address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    sWcmd(0x2a);           // Define a área X (colunas)
    sWdata(x1 >> 8); sWdata(x1);
    sWdata(x2 >> 8); sWdata(x2);
    sWcmd(0x2b);           // Define a área Y (linhas)
    sWdata(y1 >> 8); sWdata(y1);
    sWdata(y2 >> 8); sWdata(y2);
    sWcmd(0x2c);           // Modo de escrita de memória
}

void draw_pixel(uint16_t cr, uint16_t cg, uint16_t cb)
{
    sWdata(cr); sWdata(cg); sWdata(cb); // Envia os valores RGB do pixel
}

void drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t ii,mm;
	CSOUT &= ~CSBIT;       // Habilita o chip
	Address_set(x1, y1, x2, y2); // Define a área
    for(ii = 0; ii < (x2 - x1); ii++)
        for(mm = 0; mm < (y2 - y1); mm++)
            draw_pixel(0, 0xFFFF, 0); // Preenche com cor (verde)
    CSOUT |= CSBIT;        // Desabilita o chip
}

void LCD_Clear(uint16_t cr, uint16_t cg, uint16_t cb, int screen_orientation)
{
}

void LCD_Clear(uint16_t cr, uint16_t cg, uint16_t cb, int screen_orientation)
{
	uint16_t ii,mm;
    
	CSOUT &= ~CSBIT;       // Habilita o chip
	if (!screen_orientation) {
		Address_set(0, 0, 240, 320); // Orientação retrato
		for(ii = 0; ii < 240; ii++)
			for(mm = 0; mm < 320; mm++)
				draw_pixel(cr, cg, cb); // Preenche com cor
	} else {
		Address_set(0, 0, 320, 240); // Orientação paisagem
		for(ii = 0; ii < 320; ii++)
			for(mm = 0; mm < 240; mm++)
				draw_pixel(cr, cg, cb); // Preenche com cor
	}
	CSOUT |= CSBIT;        // Desabilita o chip
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

void drawPixel(uint16_t x, uint16_t y, uint16_t cr, uint16_t cg, uint16_t cb, int mult)
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

	for( col = 0; col < 5; col++) {
        uint8_t line = font5x7[c][col]; // Obtém a linha do caractere
        for( row = 0; row < 7; row++) {
            if(line & (1 << row))       // Verifica se o pixel está ativo
                drawPixel(x + col, y + row, cr, cg, cb, 1);
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

void drawChar_3x(uint16_t x, uint16_t y, char c, uint16_t cr, uint16_t cg, uint16_t cb)
{
    uint8_t col, row;

    for (col = 0; col < 15; col++) {
        uint8_t line = font15x21[0][col]; // Ajustando indice do caractere
        for (row = 0; row < 21; row++) {
            if (line & (1 << row)) {
                drawPixel(x + col, y + row, cr, cg, cb, 1);
            }
        }
    }
}

void drawString_3x(uint16_t x, uint16_t y, const char *str, uint16_t cr, uint16_t cg, uint16_t cb)
{
    while (*str) {
        drawChar_3x(x, y, *str, cr, cg, cb);
        x += 6; // Espacamento entre caracteres (5 pixels + 1 de espaco)
        str++;
    }
}









