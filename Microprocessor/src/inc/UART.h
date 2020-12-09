/*
 * UART.h
 *
 *  Created on: 19/11/2014
 */

#ifndef UART_H_
#define UART_H_

#include "aplicacion.h"

//!< /////////////		UARTs		///////////////////////////
	//0x40010000UL : Registro de recepcion de la UART0:
	#define		DIR_UART0		( ( __RW uint32_t  * ) 0x4000C000UL )

	#define		U0RBR		DIR_UART0[0]
	#define		U0THR		DIR_UART0[0]
	#define		U0DLL		DIR_UART0[0]
	#define		U0IER		DIR_UART0[1]
	#define		U0DLM		DIR_UART0[1]
	#define		U0IIR		DIR_UART0[2]
	#define		U0LCR		DIR_UART0[3]
	#define		U0LSR		DIR_UART0[5]


	#define TOPE 80
	#define TX0 PORT0,2
	#define RX0 PORT0,3


	void InitUART0 (void);
	void PushRx(uint8_t);
	void PushTx(char);
	uint16_t PopRx(void);
	uint16_t PopTx(void);
	void Enviar_Parametros(char*);
	void Enviar_Buffer();

	//0x40010000UL : Registro de recepcion de la UART1:
	#define		DIR_UART1		( ( __RW uint32_t  * ) 0x40010000UL )

	#define		U1RBR		DIR_UART1[0]
	#define		U1THR		DIR_UART1[0]
	#define		U1DLL		DIR_UART1[0]
	#define		U1IER		DIR_UART1[1]
	#define		U1DLM		DIR_UART1[1]
	#define		U1IIR		DIR_UART1[2]
	#define		U1LCR		DIR_UART1[3]
	#define		U1LSR		DIR_UART1[5]

#endif /* UART_H_ */
