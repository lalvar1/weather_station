/*
 * RTC.c
 *
 *  Created on: 26/10/2014
 */


#include "Aplicacion.h"

extern uint32_t f_tick_rtc;


/**
 * \fn 		void Inic_RTC(void)
 * \brief 	Inicializacion del RTC
 * \details
 * \author 	GOS
 * \date   	2014.05.06
 * \param 	[in] void
 * \return 	void
 * */
void Inic_RTC(void)
{
	RTCILR = 0x00;				// Limpio Flags
	RTCCIIR = 0x00;				// Deshabilito interrupciones
	RTCAMR = 0xFF;				// Enmascaro comparador de alarmas
	RTCCALIBRATION = 0x00;		// Inicializa registro de calibración
	RTCCCR = 0x01;				// Arranca contador

	RTC_HabilitaInterrupcion(RTC_INTERRUPCION_SEG);

}

/**
 * \fn 		void RTC_HabilitaInterrupcion(uint32_t tipo_int)
 * \brief 	Inicializacion de interrupción del RTC
 * \details
 * \author 	GOS
 * \date   	2014.05.06
 * \param 	[in] uint32_t tipo_int (Indica causa de la interrupción)
 * \return 	void
 * */
void RTC_HabilitaInterrupcion(uint32_t tipo_int)
{
	RTCCIIR |= 1<< tipo_int;	//se habilita el tipo de interrupcion pedido
	RTCILR = 0x03;				//limpio flags de interrupcion
	ISER0 |= 1<<17;				//habilito interrupcion desde el NVIC
}

/**
 * \fn 		void RTC_SetHoraFecha(RTC_HoraFecha* horafecha)
 * \brief 	Pone en fecha y hora al RTC
 * \details
 * \author 	GOS
 * \date   	2014.05.06
 * \param 	[in] RTC_HoraFecha* horafecha (Puntero a estructura de datos iniciales del RTC)
 * \return 	void
 * */
void RTC_SetHoraFecha(RTC_HoraFecha* horafecha)
{
	RTCSEC = horafecha->seg;
	RTCMIN = horafecha->min;
	RTCHOUR = horafecha->hora;
	RTCDOM = horafecha->dia_mes;
	RTCDOW = horafecha->dia_semana;
	RTCDOY = horafecha->dia_anio;
	RTCMONTH = horafecha->mes;
	RTCYEAR = horafecha->anio;
}


/**
 * \fn 		void RTC_GetHoraFecha(RTC_HoraFecha* horafecha)
 * \brief 	Lee el RTC
 * \details
 * \author 	GOS
 * \date   	2014.05.06
 * \param 	[in] RTC_HoraFecha* horafecha (Destino de los datos del RTC)
 * \return 	void
 * */
void RTC_GetHoraFecha(RTC_HoraFecha* horafecha)
{
	horafecha->seg = RTCSEC;
	horafecha->min = RTCMIN;
	horafecha->hora = RTCHOUR;
	horafecha->dia_mes = RTCDOM;
	horafecha->dia_semana = RTCDOW;
	horafecha->dia_anio = RTCDOY;
	horafecha->mes = RTCMONTH;
	horafecha->anio = RTCYEAR;
}


/**
 * \fn 		void RTC_IRQHandler(void)
 * \brief 	ISR del RTC
 * \details
 * \author 	GOS
 * \date   	2014.05.06
 * \param 	[in] void
 * \return 	void
 * */
void RTC_IRQHandler(void)
{
	RTCILR |= 0xFF;		//limpio todos los flags de interrupcion
	f_tick_rtc = 1;
}

