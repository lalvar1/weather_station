/*
 * EEPROM.c
 *
 *  Created on: 20/11/2014
 */
#include "aplicacion.h"

volatile uint8_t Indice_BMP180=0,Indice_DHT11=0;
extern volatile uint8_t temp_bmp[4],temp_dht[4],pres[9],hum[3],direc,hora[2],minuto[2],ESTADO,eeprom_lectura;

//divido la memoria del BMP180 para poder cargar hasta 40 posiciones

uint8_t Cargar_datos(void);
uint8_t LSB_EEPROM(void);

uint8_t LSB_EEPROM(){
	uint16_t aux;

	if(ESTADO==BMP180){
		aux=Indice_BMP180;
		Indice_BMP180+=15;

		if(Indice_BMP180>179)
			Indice_BMP180=0;
	}
	if(ESTADO==DHT11){
		aux=Indice_DHT11;
		Indice_DHT11+=11;

		if(Indice_DHT11>164)
			Indice_DHT11=0;
	}

	aux&=0x00FF;

	return (uint8_t)aux;
}

uint8_t MSB_EEPROM(){

	if(ESTADO==BMP180)
		return 0x00;
	if(ESTADO==DHT11)
		return 0x10;

}

volatile uint8_t flag_hora=0,flag_minutos=0;

uint8_t Cargar_datos(){

	static uint8_t datos=0,eeprom_temp=0,eeprom_pres=0,i=0;
	uint8_t aux;

	if(!datos){
		datos=1;
		return (LSB_EEPROM());
	}

	else{
		if(!flag_hora){
			aux=hora[i];
			i++;
			if(i==2){
				flag_hora=1;
				i=0;
			}
			return aux;
		}
		else{
			if(!flag_minutos){
				aux=minuto[i];
				i++;
				if(i==2){
					flag_minutos=1;
					i=0;
				}
				return aux;
			}
			else{
				if(ESTADO==BMP180){
					if(!eeprom_temp){
						aux=temp_bmp[i];
						i++;
						if(i==4){
							i=0;
							eeprom_temp=1;
						}
						return aux;
					}
					else{
						aux=pres[i];
						i++;
						if(i==7){
							i=0;
							eeprom_temp=0;
							eeprom_pres=0;
							flag_hora=0;
							flag_minutos=0;
							datos=0;
							direc=1;
						}
						return aux;
					}
				}
				if(ESTADO==DHT11){
					if(!eeprom_temp){
						aux=temp_dht[i];
						i++;
						if(i==4){
							i=0;
							eeprom_temp=1;
						}
						return aux;
					}
					else{
						aux=hum[i];
						i++;
						if(i==3){
							i=0;
							eeprom_temp=0;
							eeprom_pres=0;
							flag_hora=0;
							flag_minutos=0;
							datos=0;
							direc=1;
						}
						return aux;
					}
				}
			}
		}
	}
}
