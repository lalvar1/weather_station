/*
 * Aplicacion.c
 */

#include "Aplicacion.h"

#define INT_DIGITOS 63

static char buf[INT_DIGITOS + 2];		//!< Buffer para INT_DIGITS dígitos, signo - y fin de string '\0'

extern RTC_HoraFecha horafecha;
extern uint32_t f_tick_rtc;

char * itoa(int i, unsigned int base)
{
	char *p = buf + INT_DIGITOS + 1; /* apunta a posición de unidades */
	int dig, signo=0;
	if (i<0) {signo=1;i = -i;}
	if(base<=0 || base>36) base=10; /*evita división por cero */

	do { dig=(i%base); if (dig <=9) *--p = '0' + dig; else *--p= '7'+ dig ; i /= base;}
	while (i != 0);

	if(signo) *--p = '-';
	return p;
}

void Display_HoraFecha(void)
{
	char LCD_renglon[LCD_ANCHO+1];
	char *renglon;// = LCD_renglon;

	if(f_tick_rtc){
		//Obtengo la hora y la muestro en el display
		RTC_GetHoraFecha(&horafecha);
		renglon = itoa(horafecha.dia_mes,10);
		if(horafecha.dia_mes>9)
		Display_lcd(renglon, 0 , 3 );
		else
		Display_lcd(renglon, 0 , 4 );

		Display_lcd("/", 0 , 5 );
		renglon = itoa(horafecha.mes,10);
		Display_lcd(renglon, 0 , 6 );
		if(horafecha.mes>9){
		Display_lcd("/", 0 , 8 );
		renglon = itoa(horafecha.anio,10);
		Display_lcd(renglon, 0 , 9 );
		}
		else{
		Display_lcd("/", 0 , 7 );
		renglon = itoa(horafecha.anio,10);
		Display_lcd(renglon, 0 , 8 );
		}
		if(horafecha.hora<10){
		renglon = itoa(horafecha.hora,10);
//		Display_lcd(itoa(0,19), 1 , 4 );
//		Display_lcd(renglon,1,4);
		Display_lcd(renglon, 1 , 5 );
		}
		else{
		renglon = itoa(horafecha.hora,10);
		Display_lcd(renglon, 1 , 4 );
		}
		Display_lcd(":", 1 , 6 );

		if(horafecha.min<10){
		renglon = itoa(horafecha.min,10);
		Display_lcd(itoa(0,10),1,7);
		Display_lcd(renglon, 1 , 8 );
		}
		else{
		renglon = itoa(horafecha.min,10);
		Display_lcd(renglon, 1 , 7 );
		}

		Display_lcd(":", 1 , 9 );
		renglon = itoa(horafecha.seg,10);

		if(horafecha.seg<10){
			Display_lcd("0", 1 , 10 );
			Display_lcd(renglon, 1 , 11 );
		}
		else{
		renglon = itoa(horafecha.seg,10);
		Display_lcd(renglon, 1 , 10 );
		}
		f_tick_rtc = 0;
	}
}

volatile uint8_t temp_dht[4],hum[3],pres[9],ESTADO=INICIAL,hora[2],minuto[2],flag_interrupcion_eeprom;
extern volatile uint8_t flag_conversion,flag_actualizar_bmp,flag_actualizar_dht,ESTADO_I2C,temp_bmp[4],flag_interrupcion;


void MdE(){

	if(ESTADO==INICIAL)	{
		Display_HoraFecha();
		teclas();
	}

	if(ESTADO==DHT11)	{

			Display_lcd("TEMP:",0,0);
			Display_lcd(temp_dht,0,5);
			Display_lcd("HUM:",1,0);
			Display_lcd(hum,1,4);

			if(flag_actualizar_dht){
				flag_actualizar_dht=0;
				dht11();
				WComando8(LCD_CLEAR);
			}

		teclas();

	}

	if(ESTADO==BMP180){

			Display_lcd("TEMP:",0,0);
			Display_lcd(temp_bmp,0,5);
			Display_lcd("PRES:",1,0);
			Display_lcd(pres,1,5);

			if(flag_actualizar_bmp){
				if(!flag_interrupcion)
				{
				WComando8(LCD_CLEAR);
				flag_actualizar_bmp=0;
				ESTADO_I2C=BMP180;
				ARRANCAR_I2C;
				flag_interrupcion_eeprom=1;
				}
			}

		teclas();
	}


}

void celsius_a_farenheit(void)
{
uint8_t dato;
if(ESTADO==DHT11){
	float c=(float)(temp_dht[0]-48)*10+(temp_dht[1]-48),f;

	f=((9.0/5.0)*c)+32 ;

	dato=redondeo(f);

	temp_dht[0]=(dato/10)+48;
	temp_dht[1]=(dato%10)+48;
	temp_dht[3]='F';

//	flag_conversion=0;
}
if(ESTADO==BMP180){
	float c=(float)(temp_bmp[0]-48)*10+(temp_bmp[1]-48),f;

	f=((9.0/5.0)*c)+32 ;

	dato=redondeo(f);

	temp_bmp[0]=(dato/10)+48;
	temp_bmp[1]=(dato%10)+48;
	temp_bmp[3]='F';

	flag_conversion=0;
}
}

void farenheit_a_celsius(void)
{
	if(ESTADO==BMP180){
	uint8_t dato;
	float f= (float)(temp_bmp[0]-48)*10+(temp_bmp[1]-48), c;
	c=(5.0/9.0)*(f-32 );

	dato=redondeo(c);
	temp_bmp[0]=(dato/10)+48;
	temp_bmp[1]=(dato%10)+48;
	temp_bmp[3]=67;
	}
	if(ESTADO==DHT11){
		uint8_t dato;
		float f= (float)(temp_dht[0]-48)*10+(temp_dht[1]-48), c;
		c=(5.0/9.0)*(f-32 );

		dato=redondeo(c);
		temp_dht[0]=(dato/10)+48;
		temp_dht[1]=(dato%10)+48;
		temp_dht[3]=67;
	}
	flag_conversion=0;


}

void hpa_a_pa(void)
{
	pres[4]=pres[5]='0';
	pres[6]='P';
	pres[7]='a';
    pres[8]='\0';
}

void pa_a_hpa(void)
{
	 	 	pres[4]='H';
	        pres[5]='P';
	        pres[6]='a';
	        pres[7]='\0';
}

uint8_t redondeo(float dato)
{
	uint8_t resultado=dato;

int aux=dato*10;

aux%=100;

aux%=10;

if(aux>5)
	resultado++;

return resultado;
}
