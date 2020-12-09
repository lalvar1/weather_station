/*
 * Systick.c
 *
 *  Created on: 26/10/2014
 */


#include "Aplicacion.h"


extern volatile int DemoraLCD;

void InicSysTick(void){ //si divido x 4, interrumpe cada 2,5ms
		STRELOAD  = ( STCALIB / 4 ) - 1 ;   //N=1 para 10ms
		STCURR = 0;

		ENABLE = 1;
		TICKINT = 1;
		CLKSOURCE = 1;
		return;
}


extern volatile uint8_t systick,flag,systick_pres;

extern volatile uint8_t flag_systick;

extern volatile uint32_t sdtimer1, sdtimer2;

volatile uint8_t actualizar_bmp=0,actualizar_dht=0,flag_actualizar_bmp=0,flag_actualizar_dht=0;
volatile uint16_t contador_bmp=400,contador_dht=4000;

void SysTick_Handler(void)
{
	static uint8_t n=0,i=0;
	DriverTeclado();

	if(systick)	{				//flag de interrupcion para la demora de la temperatura en el BMP180
	if(!n)
		n++;
	else{
		I2CONSET=0x60;
		n=0;
		systick=0;
	}

	}

	if(systick_pres){		//flag de interrupcion para la demora de la presion en el BMP180
		if(n!=11)
			n++;
		else{
			I2CONSET=0x60;
			n=0;
			systick_pres=0;
		}
	}

	if(flag_systick)	{

		if(i!=8)
			i++;
		else{
			i=0;
			flag_systick=0;
		}

	}

	if ( DemoraLCD ){

		DemoraLCD--;

	}


	if(actualizar_dht){

		if(contador_dht)
			contador_dht--;
		else{
			contador_dht=4000;
			flag_actualizar_dht=1;
		}
	}

	if(actualizar_bmp){

			if(contador_bmp)
				contador_bmp--;
			else{
				contador_bmp=4000;
				flag_actualizar_bmp=1;
			}
		}

}


