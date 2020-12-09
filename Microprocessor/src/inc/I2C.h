/*
 * I2C.h
 *
 *  Created on: 26/10/2014
 */

#ifndef I2C_H_
#define I2C_H_

#include "aplicacion.h"


#define		I2C1		( ( __RW uint32_t  * ) 0x4005C000UL )

#define I2CONSET		I2C1[0]
#define I2C1STAT   		I2C1[1]				 // bits 3 al 7 indican el estado actual de la interfaz. los demas estan reservados
#define I2C1DAT			I2C1[2]				 // solo los primeros 8 bits, contiene el dato actual que se enviara o que llega
#define I2C1SCLH		I2C1[4]	 			// maxima frecuencia de trabajo (bit 0 al 15)	(CREO)
#define I2C1SCLL		I2C1[5]				 //minima frecuencia de trabajo (bit 0 al 15)	(CREO)
#define I2CONCLR		I2C1[6]


#define ULTRA_LOW_POWER			0
#define STANDARD				1
#define HIGH_RESOLUTION			2
#define ULTRA_HIGH_RESOLUTION	3

//prototipos de funciones

void INIC_I2C1(void);

//funciones del bmp180
void ordenar();
void calcular_parametros();
void bmp180();



#endif /* I2C_H_ */
