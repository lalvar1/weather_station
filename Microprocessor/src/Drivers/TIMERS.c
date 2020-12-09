/*
 * TIMERS.c
 *
 *  Created on: 05/11/2014
 */


#include "Aplicacion.h"

extern volatile uint8_t flag90,flag40,flag30;

volatile uint8_t count40=0,count90=0,count50=0,count30=0,count70=0,count35=0;

void SetData(uint8_t modo){

	if(modo==ENTRADA){
		SetDIR(DATA,ENTRADA);
		SetPINMODE(DATA,PINMODE_NONE);
	}

	if(modo==SALIDA){
		SetDIR(DATA,SALIDA);
	}
}

void Inic_timer()	{

	SetPINSEL(DATA,PINSEL_GPIO);		//configuro la linea de datos
	SetData(SALIDA);
	SetPIN(DATA,ON);

	SetPINSEL(M0T0,PINSEL_FUNC3);		//configuro el pin de match0.0

	PCONP |= 1<<1;
	PCLKSEL0 |= 1 << 2; 	// Clock for timer PCLK = CCLK Selecciono clock

	T0MCR=0x03;			//cuando alcanze el valor del match, interrumpira y se reiniciara
	ISER0 |=(0x01 << 1); // Habilito Interrupcion TIMER0

	T0MR0= 1000;					//cargo el valor del match
	T0TCR=0x02;						//apago reinicio y enciendo
	T0TCR=0x01;

}

void TIMER0_IRQHandler (void)
{
static uint8_t aux=3;

T0IR|=0x1;

if(flag30){
	if(aux)
		aux--;
	else{
		aux=3;
		flag30=0;
	}
}

if(flag40){
	if(count40!=4)
		count40++;
	else	{
		count40=0;
		flag40=0;
		}
	}

if(flag90)	{
		if(count90!=9)
			count90++;
		else	{
			count90=0;
			flag90=0;
		}
}


}


