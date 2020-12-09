/*
 * Timers.h
 *
 *  Created on: 05/11/2014
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "RegLPC1769.h"

#define DATA	PORT1,22		//EXPANSION8
#define	M0T0	PORT1,28
#define	M0T1	PORT1,29


// Timer 0
			#define		TIMER0 		( ( __RW uint32_t  * ) 0x40004000UL )
			#define		T0IR		TIMER0[0]
			#define		T0TCR		TIMER0[1]
			#define		T0TC		TIMER0[2]
			#define		T0PR		TIMER0[3]
			#define		T0PC		TIMER0[4]
			#define		T0MCR		TIMER0[5]
			#define		T0MR0		TIMER0[6]
			#define		T0MR1		TIMER0[7]
			#define		T0MR2		TIMER0[8]
			#define		T0MR3		TIMER0[9]
			#define		T0CCR		TIMER0[10]
			#define		T0CR0		TIMER0[11]
			#define		T0CR1		TIMER0[12]

			#define		TIMER0EMR	( ( __RW uint32_t  * ) 0x4000403CUL )

			#define		TIMER0CTCR	( ( __RW uint32_t  * ) 0x40004070UL )



void Inic_timer(void);


void dht11(void);
void mostrar(uint8_t);
uint8_t inic_dht11();
uint8_t datos(void);
void SetData(uint8_t);

#endif /* TIMERS_H_ */
