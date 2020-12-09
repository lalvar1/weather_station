/*
 * FW_I2C.c
 *
 *  Created on: 26/10/2014
 */



#include "Aplicacion.h"


#define ESPERANDO	0
#define PIDIENDO 	1
#define LISTO 		2


//	// 	I2CONSET: 	0	1	   2	3	 4		5	6		7	.....
//	//				reserved   AA	SI  STO	  STA	I2EN	reserved

volatile uint8_t reg=0,dat=0,systick=0,systick_pres=0,ESTADO_I2C=BMP180,dato[180],eeprom_temp=0,eeprom_pres=0,direc=0;
extern volatile uint8_t flag_temp,flag_pres,eeprom_lectura,ESTADO;

extern volatile uint8_t temp[4],pres[9];
extern volatile uint16_t calib[22],RUT[2];
extern volatile uint32_t RUP[3];


void I2C1_IRQHandler(void) {

	static uint8_t n=0;
	static uint16_t i=0;
	static uint8_t flag_temp=ESPERANDO,flag_pres=ESPERANDO;


	switch(I2C1STAT){

        case 0x08:

        	if(ESTADO_I2C==BMP180)
                I2C1DAT= 0xEE;
        	else
        		I2C1DAT=0xA0;

            I2CONCLR=0x18;			//STO=SI=0;		AA=STA=X;

                break;

        case 0x10:
        	if(ESTADO_I2C==BMP180)
        		I2C1DAT= 0xEF;
        	else
        		I2C1DAT=0xA1;

        	I2CONCLR=0x18;			//STO=SI=0;		AA=STA=X;

        	break;

        case 0x18:

        	if(ESTADO_I2C==BMP180){

        		if(!reg){
                    I2C1DAT=0xAA;		//cargo primer direccion de los registros de calib
                    I2CONCLR=0x38;		//STO=STA=SI=0		AA=X
                 }
                else{

                	if(flag_temp==PIDIENDO){
                		I2C1DAT=0xF6;
                		I2CONCLR=0x38;		//STO=STA=SI=0		AA=X
                	}
                	else{
                		if(flag_pres==PIDIENDO)	{
                			I2C1DAT=0xF6;
                			I2CONCLR=0x38;		//STO=STA=SI=0		AA=X
                		}
                		else{
                			I2C1DAT=0xF4;			//cargo direccion donde esta UT o UP
                			I2CONCLR=0x38;		//STA=STO=SI=0		AA=X
                		}
                	}
                }
        	}

        	if(ESTADO_I2C==CARGAR_EEPROM){

        		I2C1DAT=MSB_EEPROM();
                I2CONCLR=0x38;		//STO=STA=SI=0		AA=X
        	}

        	if(ESTADO_I2C==LEER_EEPROM){

        		if(eeprom_lectura==BMP180)
        			I2C1DAT=0x00;
        		else
        			I2C1DAT=0x10;

        		I2CONCLR=0x38;			//STA=STO=SI

        	}

                break;

        case 0x28:
        	if(ESTADO_I2C==BMP180){
        		if(!reg){
                	I2CONSET=0x20;		//STA=1
                	I2CONCLR=0x18;		//STO=SI=0		AA=X
                }
        		if(reg && (flag_temp!=LISTO)){

        			if(flag_temp==PIDIENDO){
        				I2CONSET=0x20;		//STA=1
        				I2CONCLR=0x18;		//STO=SI=0		AA=X
        			}

        			if(flag_temp==ESPERANDO){
        				if(!dat){
        					I2C1DAT=0x2E;
        					I2CONCLR=0x38;		//STA=STO=SI=0;		AA=X;
        					dat=1;
        				}
        				else{
        					I2CONSET=0x10;		//STO=1	A=X;
        					I2CONCLR=0x28;		//STA=SI=0;
        					dat=0;
        					flag_temp=PIDIENDO;
        					systick=1;
        				}
        			}
        		}
        		if(reg && (flag_temp==LISTO))	{

        			if(flag_pres==PIDIENDO){
        				I2CONSET=0x20;		//STA=1
        				I2CONCLR=0x18;		//STO=SI=0		AA=X
        			}

        			if(flag_pres==ESPERANDO){
        				if(!dat){
        					I2C1DAT=(0x34 + (ULTRA_HIGH_RESOLUTION<<6));
        					I2CONCLR=0x38;		//STA=STO=SI=0;		AA=X;
        					dat=1;
        				}
        				else{
        					I2CONSET=0x10;		//STO=1	A=X;
        					I2CONCLR=0x28;		//STA=SI=0;
        					dat=0;
        					flag_pres=PIDIENDO;
        					systick_pres=1;
        				}
        			}
        		}
        	}
        		if(ESTADO_I2C==CARGAR_EEPROM)	{

        			if(!direc){
        				I2C1DAT=Cargar_datos();
        				I2CONCLR=0x38;		//STA=STO=SI=0;		AA=X;
        			}
        			else{
        				direc=0;
        				I2CONSET=0x10;
        				I2CONCLR=0x28;
        				ESTADO_I2C=BMP180;
        			}
        		}

        		if(ESTADO_I2C==LEER_EEPROM){

        			if(!direc){
        				I2C1DAT=0x00;			//LSB
        				I2CONCLR=0x38;			//STA=STO=SI
        				direc=1;
        			}
        			else{						//Ya mande la direccion entera
        										//si tengo que leer mando la condicion de restart
        				I2CONSET=0x20;		//STA=1
        				I2CONCLR=0x18;		//STO=SI=0
        				direc=0;
        			}

        		}

        	break;

        case 0x40 :
                I2CONCLR=0x38;		//STA=STO=SI=0
                I2CONSET=0x04;		//AA=1
//                        		for(i=0;i<180;i++)
//                        			dato[i]=0;
                break;

        case 0x50:

        	if(ESTADO_I2C==BMP180){

        		if(!reg){
        			calib[n]=I2C1DAT;
        			n++;

        			if(n==21)	{
        				I2CONCLR=0x3C;			//STA=STO=SI=AA=0
        				n=0;
        			}
        			else	{
        				I2CONCLR=0x38;			//STA=STO=SI=0
        				I2CONSET=0x04;			//AA=1
        			}
				}

                if(flag_temp==PIDIENDO)	{
                	RUT[0]=I2C1DAT;
                	I2CONCLR=0x3C;
                }
                if(flag_pres==PIDIENDO)	{
                	RUP[n]=I2C1DAT;
            		n++;
            		if(n==2){
            			n=0;
                    	I2CONCLR=0x3C;
            		}
            		else{
            			I2CONCLR=0x38;			//STA=STO=SI=0
            			I2CONSET=0x04;			//AA=1
            		}
                }
        	}

        	if(ESTADO_I2C==LEER_EEPROM){


//        		if(ESTADO==BMP180){
        			dato[i]=I2C1DAT;
        			i++;
        			if(i==179){
        				i=0;
        				I2CONCLR=0x3C;
        			}
        			else{
        				I2CONCLR=0x38;
        				I2CONSET=0x04;
        			}
//        		}
//        		else{
//        			dato[i]=I2C1DAT;
//        			i++;
//        			if(i==119){
//        				i=0;
//        				I2CONCLR=0x3C;
//        			}
//        			else{
//        				I2CONCLR=0x38;
//        				I2CONSET=0x04;
//        			}
//        		}
        	}

                break;

        case 0x58:

        	if(ESTADO_I2C==BMP180){

        			if(!reg){
        				calib[21]=I2C1DAT;
        				reg=1;

   	            	   I2CONSET=0x30;
   	            	   I2CONCLR=0x08;
      	               }
                	if(flag_temp==PIDIENDO)	{
                		RUT[1]=I2C1DAT;
                		I2CONSET=0x30;
                		I2CONCLR=0x08;
                		flag_temp=LISTO;
                		}
                	if(flag_pres==PIDIENDO)		{
                		RUP[2]=I2C1DAT;
                		I2CONSET=0x10;
                		I2CONCLR=0x28;
                		flag_temp=ESPERANDO;
                		flag_pres=ESPERANDO;
                		reg=0;

            		ordenar();

                	}
        	}

        	if(ESTADO_I2C==LEER_EEPROM){

//        		if(ESTADO==BMP180)
        			dato[180]=I2C1DAT;
//        		else
//        			dato[120]=I2C1DAT;

        		I2CONSET=0x10;		//STO=1
        		I2CONCLR=0x28;		//STA=SI=0
        		ESTADO_I2C=BMP180;

        		Enviar_Buffer();
        	}
                	break;
}

}



