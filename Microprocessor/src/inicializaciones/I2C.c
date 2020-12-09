/*
 * I2C.c
 *
 *  Created on: 26/10/2014
 */

#include "Aplicacion.h"

extern uint8_t regcal[];

uint8_t AC1,AC2,AC3,AC4,AC5,AC6,B1,B2,MC,MD,MB;

void INIC_I2C1(void)
{

 //yo necesito el P0.19(SDA1 con la funcion 11) y P0.20(SCL1) con la funcion 11

	PCONP |= 1 << 19; 				// Habilitar I2C1 que esta en el bit 19 del registro PCONP
	PCLKSEL1 &=~(11<< 6);
	//PCLKSEL1 |= (0x01<<6);


	SetPINSEL(PORT0,0,PINSEL_FUNC3);			//la funcion 11 es la correspondiente al SDA1
	SetPINSEL(PORT0,1,PINSEL_FUNC3);		//la funcion 11 es la correspondiente al SCL1

	SetPINMODE(PORT0,0,PINMODE_NONE);		//los pines no deben ser ni pull up ni pull down (pagina 428)
	SetPINMODE(PORT0,1,PINMODE_NONE);

	SetMODE_OD(PORT0,0,OPEN_DRAIN);			//deben ser open drain (pagina 428)
	SetMODE_OD(PORT0,1,OPEN_DRAIN);

	ISER0 |= 0x01<<11;		//habilito interrupcion del NVIC del I2C1

	//al elegir el clock de 25MHz necesito que se trabaje a 100Khz por ende de la formula en la pagina 448
	//necesito que ambos registros sean igual a 125=7D

	I2C1SCLH=0x7D;
	I2C1SCLL=0x7D;

	I2CONSET=0x40;


}

