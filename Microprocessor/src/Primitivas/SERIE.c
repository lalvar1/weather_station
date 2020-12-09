/*
 * SERIE.c
 *
 *  Created on: 19/11/2014
 */

#include "aplicacion.h"

#define INACTIVO	0
#define LEYENDO		1

volatile uint8_t ESTADO_SERIE=INACTIVO;
volatile char BufferRx[TOPE],BufferTx[TOPE];
volatile uint8_t Rxin=0,Rxout=0,Txin=0,Txout=0,TxStart=0;

extern volatile uint8_t ESTADO,temp_bmp[4],temp_dht[4],hum[3],pres[9],ESTADO_I2C,flag_interrupcion;

volatile uint8_t eeprom_lectura=0;

void PushRx(uint8_t dato){
	BufferRx[Rxin]=dato;
	Rxin++;
	Rxin%=TOPE;

	if(dato=='P')
		Enviar_Parametros(pres);

	if(dato=='H')
		Enviar_Parametros(hum);

	if(dato=='B')
		Enviar_Parametros(temp_bmp);

	if(dato=='D')
		Enviar_Parametros(temp_dht);

	if(dato=='F'){
		if(ESTADO_SERIE==INACTIVO){
		ESTADO_I2C=LEER_EEPROM;
		ARRANCAR_I2C;
		eeprom_lectura=DHT11;
		ESTADO_SERIE=LEYENDO;
		}

	}

	if(dato=='I'){
		if(ESTADO_SERIE==INACTIVO){
		ESTADO_I2C=LEER_EEPROM;
		ARRANCAR_I2C;
		eeprom_lectura=BMP180;
		ESTADO_SERIE=LEYENDO;
		}
	}

	if(dato=='Z'){
			ESTADO_I2C=LEER_EEPROM;
			ARRANCAR_I2C;
	}



}

uint16_t PopRx(void){

	uint16_t aux;

	if(Rxout!=Rxin)	{
		aux=BufferRx[Rxout];
		Rxout++;
		Rxout%=TOPE;
	}
	return aux;
}

uint16_t PopTx(void){
	uint8_t aux=0xFF;

	if(Txout!=Txin)	{
		aux=BufferTx[Txout];
		Txout++;
		Txout%=TOPE;
	}
	return aux;
}

void PushTx(char dato){
	BufferTx[Txin]=dato;
	Txin++;
	Txin%=TOPE;

	if(!TxStart)	{
		U0THR=PopTx();
		TxStart=0;
	}

}


void Enviar_Parametros(char* dato){

	uint8_t i;

	for(i=0;dato[i]!='\0';i++){
		PushTx(dato[i]);
	}

}

extern volatile uint8_t eeprom_lectura,dato[180],Indice_BMP180,Indice_DHT11;
volatile uint8_t indice_aux=0,indice_aux_dht=0;

void Enviar_Buffer(){

	uint8_t aux[15],aux_dht[12],i,j;

	if(eeprom_lectura==BMP180){
		if(indice_aux!=Indice_BMP180){
			for(i=0;i<15;i++)
				aux[i]=dato[indice_aux+i];

			indice_aux+=15;
			if(indice_aux>179)
				indice_aux=0;
			Enviar_Parametros(aux);
			return;
		}
		else{
			for(i=0;i<15;i++)
				aux[i]='X';
			Enviar_Parametros(aux);
			indice_aux=0;
			ESTADO_SERIE=INACTIVO;
			flag_interrupcion=0;

		}
	}

	if(eeprom_lectura==DHT11){
		aux_dht[11]=aux_dht[12]='\0';
		if(indice_aux_dht!=Indice_DHT11){
			for(i=0;i<11;i++)
				aux_dht[i]=dato[indice_aux_dht+i];

			indice_aux_dht+=11;
			if(indice_aux_dht>164)
				indice_aux_dht=0;
			Enviar_Parametros(aux_dht);

			return;
		}
		else{
			for(i=0;i<11;i++)
				aux[i]='X';
				aux[11]='\0';
				Enviar_Parametros(aux);
				indice_aux_dht=0;
				ESTADO_SERIE=INACTIVO;
				flag_interrupcion=0;
			}
	}
}
