/*
 * Aplicacion.h
 *
 *  Created on: 26/10/2014
 */

#ifndef APLICACION_H_
#define APLICACION_H_

	#include "lcd.h"
	//#include "Ejemplo_RTC.h"
	#include "RTC.h"
	#include "GPIO.h"
	#include "BaseBoard.h"
	#include "RegLPC1769.h"
	#include "I2C.h"
	#include "teclado4x1.h"
	#include "Timers.h"
	#include "UART.h"


	void InicSysTick(void);
	void InicPLL ( void );
	void cuentoPulsos (void);
	void InicializarKit ( void );

	void MdE();
	void farenheit_a_celsius();
	void celsius_a_farenheit(void);
	void hpa_a_pa(void);
	void pa_a_hpa(void);
	uint8_t redondeo(float);


#define ARRANCAR_I2C	I2CONSET=0x60


#define INICIAL			0
#define BMP180			1
#define DHT11			2
#define CARGAR_EEPROM	3
#define LEER_EEPROM		4




#define NONE		0
#define PRIMARIA	1
#define SECUNDARIA	2


#endif /* APLICACION_H_ */
