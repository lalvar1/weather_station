/*
 * DHT11.c
 *
 */

#include "aplicacion.h"

#define ERROR	0
#define OKEY	1


volatile uint8_t flag_systick=0,flag90=0,flag40=0,flag30=0;
extern volatile uint8_t count40,count90,count50,count30,count70,count35,ESTADO_I2C,ESTADO,flag_unidad;
extern volatile uint8_t hora[2],minuto[2];
extern RTC_HoraFecha horafecha;				//!< Estructura de datos del RTC



volatile uint8_t message[5]={0,0,0,0,0};


uint8_t inic_dht11(){

	uint8_t i;

	SetPIN(DATA,ON);


	SetPIN(DATA,OFF);		//pongo la linea en bajo por 18 ms
	flag_systick=ON;
	while(flag_systick);
	flag_systick=OFF;

	SetPIN(DATA,ON);		//subo la linea por 40us
	flag40=1;
	SetData(ENTRADA);
	while(flag40)	{		//mientra transcurren aprox 40us pregunto si el dht me bajo la linea
		if(GetPIN(DATA,0))
			break;
	}
	if(GetPIN(DATA,1)){		//si salio del while porque vencio el flag y todavia sigue en 1 hubo error
		return ERROR;
		SetData(SALIDA);
		SetPIN(DATA,ON);
	}

	flag90=1;
	while(flag90)	{
		if(GetPIN(DATA,1))
			break;
	}
	if(GetPIN(DATA,0)){
		return ERROR;
		SetData(SALIDA);
		SetPIN(DATA,ON);
	}

	flag90=1;
	count90=0;
	while(flag90){
		if(GetPIN(DATA,0))
			break;
	}
	if(GetPIN(DATA,1)){
		return ERROR;
		SetData(SALIDA);
		SetPIN(DATA,ON);
	}
	else
		return OKEY;


}

volatile uint8_t mensaje[40];

uint8_t datos(){

static uint8_t j,i;

message[0]=message[1]=message[2]=message[3]=message[4]=0;

	for(i=0;i<5;i++){
		for(j=0;j<8;j++)	{
			while(GetPIN(DATA,0));
			T0TCR=0x02;						//apago reinicio y enciendo
			T0TCR=0x01;
			flag30=1;
			while(flag30)	{
				if(GetPIN(DATA,0))
					break;
			}
			if(GetPIN(DATA,0))	{
			}
			else{
				message[i] |= 0x1<<(7-j);
				while(GetPIN(DATA,1));
			}
		}
	}


	if(message[4]!=message[0]+message[1]+message[2]+message[3])
		return ERROR;
	SetData(SALIDA);

	return OKEY;
}


extern uint8_t bandera;
extern volatile uint8_t temp_dht[4],hum[3],actualizar_dht;

void mostrar(uint8_t check)
{

uint8_t aux;


	if (check)	{
		aux=message[0]/10;		//la decena de la humedad
		hum[0]=aux+48;

		aux=message[0]%10;
		hum[1]=aux+48;

		hum[2]='%';
		hum[3]='\0';


		aux=message[2]/10;
		temp_dht[0]=aux+48;

		aux=message[2]%10;
		temp_dht[1]=aux+48;


		temp_dht[2]=178;
		temp_dht[3]='C';
		temp_dht[4]='\0';

		if(flag_unidad){
			celsius_a_farenheit();
			hpa_a_pa();
		}

	}


	}


void dht11(){

	uint8_t check,aux;
		check=inic_dht11();

		if(check){
			mostrar(datos());
		}
		bandera=0;



	if(ESTADO!=INICIAL)
			actualizar_dht=1;

}
