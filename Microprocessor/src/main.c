/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#include "aplicacion.h"
#include "lcd.h"

	volatile int DemoraLCD;					//!< Contador de demora rutinas de RTC
	uint32_t f_tick_rtc  = 0;				//!< flag indicador de interrupción de RTc por incremento de seg
	RTC_HoraFecha horafecha;				//!< Estructura de datos del RTC


	volatile uint8_t buffKey;				//Buffer de teclado

	int main(void)
	{

		horafecha.seg = 0;
		horafecha.min = 9;
		horafecha.hora = 14;
		horafecha.dia_mes = 19;
		horafecha.mes = 12;
		horafecha.anio = 2014;

		RTC_SetHoraFecha(&horafecha);

	InicializarKit ();




	while(1) {
			MdE();

		}
		return 0 ;
	}


