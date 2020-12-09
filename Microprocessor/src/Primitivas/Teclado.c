/*
 * Teclado.c
 *
 *  Created on: 26/10/2014
 */


#include "Aplicacion.h"
#define estado_bmp180 2
#define estado_dht11 1
#define reposo 0

#define SIN_TECLA	0
#define CON_TECLA	1

volatile uint8_t estado_teclado=SIN_TECLA,flag_interrupcion=0;

// Buffer de teclado
extern volatile uint8_t buffKey,ESTADO_I2C,actualizar_dht,actualizar_bmp,actualizar_dht,flag40,flag90,flag_systick;
extern volatile uint8_t hora[2],minuto[2],flag_interrupcion_eeprom;
extern RTC_HoraFecha horafecha;				//!< Estructura de datos del RTC
extern volatile uint16_t contador_bmp,contador_dht;



volatile uint8_t ESTADO,bandera=0,bandera_bmp=0,bandera_unidad=0,flag_unidad=0,flag_conversion=0;


uint8_t Teclado(void)
{

	uint8_t tecla;

	if(estado_teclado==SIN_TECLA){
		tecla=NO_KEY;
		if (buffKey != NO_KEY )
		{
			tecla = buffKey;
			buffKey = NO_KEY;
			estado_teclado=CON_TECLA;
		}
		return tecla;
	}

	if(estado_teclado==CON_TECLA){
		if(buffKey==NO_KEY)
			estado_teclado=SIN_TECLA;

		buffKey=NO_KEY;
		return buffKey;
	}

}

void teclas(){

	uint8_t tecla,aux;

	tecla = Teclado();

			 switch(tecla)
			 {
		    	case 1:


		    		if(ESTADO==DHT11){
		    			dht11();
		    			WComando8(LCD_CLEAR);
		    		}
		    		if(!bandera){
		    			ESTADO=DHT11;
		    			dht11();
		    			WComando8(LCD_CLEAR);
		    			bandera=1;
		    		}
		    		bandera_bmp=0;

		    		break;

		    	case 2:
		    		SetPIN(DATA,ON);
		    		flag_systick=OFF;
		    		flag40=flag90=0;
		    		if(ESTADO==BMP180){
			    		if(!flag_interrupcion_eeprom){
			    			ARRANCAR_I2C;
			    			ESTADO_I2C=BMP180;
			    			bandera_bmp=1;
			    		}
		    		}
		    		if(!bandera_bmp){
		    		WComando8(LCD_CLEAR);
		    		ARRANCAR_I2C;
		    		ESTADO=BMP180;
		    		bandera_bmp=1;
		    		}
		    		bandera=0;

		   			 break;

				case 3:

		    		WComando8(LCD_CLEAR);
					ESTADO=INICIAL;
					bandera=0;
					bandera_bmp=0;

					break;

				case 4:

						if(!flag_unidad){
				    		WComando8(LCD_CLEAR);
							celsius_a_farenheit();
							hpa_a_pa();
							flag_unidad=1;
							break;
						}
						else
						{
				    		WComando8(LCD_CLEAR);
							farenheit_a_celsius();
							pa_a_hpa();
							flag_unidad=0;
							break;
						}

			    		bandera=0;
					break;

				case 5:
					if(!flag_interrupcion_eeprom){
						flag_interrupcion=1;
						if(horafecha.hora<10){
							hora[0]=0+48;
							hora[1]=horafecha.hora+48;
						}
						else{
							aux=horafecha.hora/10;
							hora[0]=aux+48;
							aux=horafecha.hora%10;
							hora[1]=aux+48;
						}

						aux=horafecha.min/10;
						minuto[0]=aux+48;
						aux=horafecha.min%10;
						minuto[1]=aux+48;

						WComando8(LCD_CLEAR);

					ESTADO_I2C=CARGAR_EEPROM;
					ARRANCAR_I2C;
	}
					break;


				default: break;
			 }
}
