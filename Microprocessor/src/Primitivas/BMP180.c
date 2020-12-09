/*
 * BMP180.c
 *
 *  Created on: 26/10/2014
 */


#include "aplicacion.h"

short  AC1,AC2,AC3,B1,B2,MB,MC,MD;
unsigned short AC4,AC5,AC6;
long UP,UT;
volatile uint16_t calib[22],RUT[2];
volatile uint32_t RUP[3];


volatile long TEMPERATURA,PRESION;

extern volatile uint8_t pres[9],bandera_bmp,ESTADO_I2C,ESTADO,actualizar_bmp,flag_unidad,flag_interrupcion_eeprom;

volatile uint8_t temp_bmp[4];

void calcular_parametros()	{

        long X1,X2,X3,B3,B5,B6,TEMPERATURA,PRESION;
        unsigned long B4,B7;

        X1=X2=X3=B3=B5=B6=TEMPERATURA=PRESION=B4=B7=0;

        X1=(UT-AC6)*AC5/32768;
        X2=MC*2048/(X1+MD);
        B5=X1+X2;
        TEMPERATURA=(B5+8)/16;

        B6=( B5-4000);
        X1=((B2*(B6*B6/4096))/2048);
        X2=(AC2*B6/2048);
        X3=X1+X2;
        B3=((((AC1*4 + X3)<< ULTRA_HIGH_RESOLUTION)+2)/4);
        X1=(AC3*B6/8192);
        X2=((B1*(B6*B6/4096))/65536);
        X3=((X1+X2)+2)/4;
        B4=(AC4* (unsigned long)(X3+32768)/32768);
        B7=((unsigned long)UP-B3)*(50000>> ULTRA_HIGH_RESOLUTION);
        if(B7< 0x80000000)
                PRESION=(long)((B7*2)/B4);
        else
                PRESION=(long)((B7/B4)*2);

        X1=((PRESION/256)*(PRESION/256));
        X1=((X1*3038)/65536);
        X2=((-7357*PRESION)/65536);
        PRESION= PRESION + (X1+X2+3791)/16;

        TEMPERATURA/=10;			//PASO LA TEMPERATURA DE 0.1°C A °C
        PRESION/=100;				//PASO LA PRESION DE PASCAL A HECTOPASCAL

        uint8_t aux;

        aux=TEMPERATURA/10;
        temp_bmp[0]=aux+48;

        aux=TEMPERATURA%10;
        temp_bmp[1]=aux+48;

        temp_bmp[2]=178;
        temp_bmp[3]='C';
		temp_bmp[4]='\0';

        aux=PRESION/1000;
        pres[0]=aux+48;


        aux=PRESION/100;
        aux%=10;
        pres[1]=aux+48;


        aux=PRESION%1000;
        aux/=10;
        pres[2]=aux+48;


        aux=PRESION%1000;
        aux%=10;
        pres[3]=aux+48;			//el + 48 es para pasarlo a ascii

        pres[4]='H';
        pres[5]='P';
        pres[6]='a';
        pres[7]='\0';


        if(flag_unidad){
        	celsius_a_farenheit();
        	hpa_a_pa();
        }

        if(ESTADO!=INICIAL)
        	actualizar_bmp=1;

        flag_interrupcion_eeprom=0;
}

void ordenar(){

	AC1= (signed short)((calib[0]<<8)|calib[1]);
	AC2= (signed short)((calib[2]<<8)|calib[3]);
	AC3= (signed short)((calib[4]<<8)|calib[5]);
	AC4= (unsigned short)((calib[6]<<8)|calib[7]);
	AC5= (unsigned short)((calib[8]<<8)|calib[9]);
	AC6= (unsigned short)((calib[10]<<8)|calib[11]);
	B1= (signed short)((calib[12]<<8)|calib[13]);
	B2= (signed short)((calib[14]<<8)|calib[15]);
	MB= (signed short)((calib[16]<<8)|calib[17]);
	MC= (signed short)((calib[18]<<8)|calib[19]);
	MD= (signed short)((calib[20]<<8)|calib[21]);

	UT=(long)((RUT[0]<<8)|RUT[1]);

	UP=(long)(((RUP[0]<<16)|(RUP[1]<<8)|RUP[2])>>(8-ULTRA_HIGH_RESOLUTION));

	calcular_parametros();

}


