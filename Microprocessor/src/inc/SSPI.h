/*
 * SSPI.h
 *
 *  Created on: 26/10/2014
 */

#ifndef SSPI_H_
#define SSPI_H_

#include "Aplicacion.h"

#define SSPI	((__RW uint32_t *)	0x40030000UL)

#define MOSI	PORT0,9
#define MISO	PORT0,8
#define SCK		PORT0,7
#define SSEL	PORT0,6

#define CR0		SSPI[0]
#define CR1		SSPI[1]
#define DR		SSPI[2]
#define SR		SSPI[3]
#define CPSR	SSPI[4]

//Prototipos
void SSP_Inic(void);
void SSP_EnviarDato(uint16_t dato);
uint16_t SSP_RecibirDato();

#define SSEL_HIGH()	SetPIN(SSEL,1)
#define SSEL_LOW()	SetPIN(SSEL,0)


#endif /* SSPI_H_ */
