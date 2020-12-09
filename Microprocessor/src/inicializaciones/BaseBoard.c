/*
 * BaseBoard.c
 *
 *  Created on: 26/10/2014
 */

#include "Aplicacion.h"


/**
 * \fn 		void Inicializar_Teclado( void )
 * \brief 	Inicializacion pines de teclas placa base
 * \details
 * \author 	GOS
 * \date   	2014.05.06
 * \param 	[in] void
 * \return 	void
 * */
void Inicializar_Teclado( void )
{
	SetPINSEL ( KEY0 , PINSEL_GPIO);
	SetPINSEL ( KEY1 , PINSEL_GPIO);
	SetPINSEL ( KEY2 , PINSEL_GPIO);
	SetPINSEL ( KEY3_RC , PINSEL_GPIO);
	SetPINSEL ( KEY4_RC , PINSEL_GPIO);

	SetDIR ( KEY0 , ENTRADA);
	SetDIR ( KEY1 , ENTRADA);
	SetDIR ( KEY2 , ENTRADA);
	SetDIR ( KEY3_RC , ENTRADA);
	SetDIR ( KEY4_RC , ENTRADA);

	SetPINMODE ( KEY0 , PINMODE_PULLUP);
	SetPINMODE ( KEY1 , PINMODE_PULLUP);
	SetPINMODE ( KEY2 , PINMODE_PULLUP);
}

/**
 * \fn 		void Inicializar_Relay( void )
 * \brief 	Inicializacion pines de teclas placa base
 * \details
 * \author 	GOS
 * \date   	2014.05.06
 * \param 	[in] void
 * \return 	void
 * */
void Inicializar_Relay( void )
{
	SetPINSEL ( LED1 , PINSEL_GPIO);
	SetPINSEL ( LED2 , PINSEL_GPIO);
	SetPINSEL ( LED3 , PINSEL_GPIO);
	SetPINSEL ( LED4 , PINSEL_GPIO);
	SetPINSEL ( BUZZ , PINSEL_GPIO);

	SetDIR ( LED1 , SALIDA);
	SetDIR ( LED2 , SALIDA);
	SetDIR ( LED3 , SALIDA);
	SetDIR ( LED4 , SALIDA);
	SetDIR ( BUZZ , SALIDA);

	SetPIN (BUZZ, BUZZ_OFF);  //se apaga con 1
}



