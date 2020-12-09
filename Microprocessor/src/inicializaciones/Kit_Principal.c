/*
 * Kit_Principal.c
 *
 *  Created on: 26/10/2014
 */


#include "Aplicacion.h"

extern volatile uint8_t ESTADO,ESTADO_I2C;
void InicializarKit ( void );

void InicializarKit ( void )
{
	InicPLL();
	Inicializar_Teclado();
	Inicializar_Relay();

	InicSysTick();
	Inic_LCD();
	Inic_RTC();
	INIC_I2C1();
	Inic_timer();
	InitUART0();


}
