/*
 * UARTC.c
 *
 *  Created on: 19/11/2014
 */

#include "aplicacion.h"

void InitUART0 (void)
{
	//1.- Registro PCONP (0x400FC0C4) - bit 4 en 1 prende la UART1:
	PCONP |= 0x01<<3;
	//2.- Registro PCLKSEL0 (0x400FC1A8) - bits 8 y 9 en 0 seleccionan que el clk de la UART1 sea CCLK/4:
	PCLKSEL0 &= ~(0x03<<6);	//con un CCLK=100Mhz, nos queda PCLOCK=25Mhz
	//3.- Registro U1LCR (0x4001000C) - transmision de 8 bits, 1 bit de stop, sin paridad, sin break cond, DLAB = 1:
	U0LCR = 0x00000083;
	//4.- Registros U1DLL (0x40010000) y U1DLM (0x40010004) - 9600 baudios:
	U0DLM = 0;
	U0DLL = 0xA3;//0xA3 para 9600
	//5.- Registros PINSEL0 (0x4002C000) y PINSEL1 (0x4002C004) - habilitan las funciones especiales de los pines:
	//TX1D : PIN ??	-> 		P0[15]	-> PINSEL0: 30:31
	SetPINSEL(TX0,PINSEL_FUNC1);
	//RX1D : PIN ??	-> 		P0[16]	-> PINSEL1: 00:01
	SetPINSEL(RX0,PINSEL_FUNC1);
	//6.- Registro U1LCR, pongo DLAB en 0:
	U0LCR = 0x03;
	//7. Habilito las interrupciones (En la UART -IER- y en el NVIC -ISER)
	U0IER = 0x03;
	ISER0 |= (1<<5);
}
