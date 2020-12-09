

#include "Aplicacion.h"

extern volatile int DemoraLCD;

void LCDDelay(uint16_t demora){

	DemoraLCD = demora;
	while(DemoraLCD);

}


void LatchLcd(void){
	SetPIN(LCD_E,ON);
	LCDDelay(2);
	SetPIN(LCD_E,OFF);
}


void Display_lcd( char *msg , char r , char p ){
	unsigned char i ;

	if( r )
        p = p + 0xc0 ;
	else
		p = p + 0x80 ;

	WComando8(p);
	for( i = 0 ; msg[ i ] != '\0' ; i++ )
		WDato(msg[ i ]);

}


/**
 * \fn 		void Inic_PinesLCD(void)
 * \brief 	Inicializa pines para manejo de LCD
 * \details
 * \author 	GRUPO 9
 * \date   	18/11/2014
 * \param 	void
 * \return 	void
 * */

void Inic_PinesLCD(void){
	SetPINSEL(LCD_RS,PINSEL_GPIO);
	SetDIR(LCD_RS,SALIDA);
	SetMODE_OD(LCD_RS,OPEN_DRAIN);
	SetPIN(LCD_RS,OFF);

	SetPINSEL(LCD_E,PINSEL_GPIO);
	SetDIR(LCD_E,SALIDA);
	SetMODE_OD(LCD_E,OPEN_DRAIN);
	SetPIN(LCD_E,OFF);

	SetPINSEL(LCD_D4,PINSEL_GPIO);
	SetDIR(LCD_D4,SALIDA);
	SetMODE_OD(LCD_D4,OPEN_DRAIN);
	SetPIN(LCD_D4,OFF);

	SetPINSEL(LCD_D5,PINSEL_GPIO);
	SetDIR(LCD_D5,SALIDA);
	SetMODE_OD(LCD_D5,OPEN_DRAIN);
	SetPIN(LCD_D5,OFF);

	SetPINSEL(LCD_D6,PINSEL_GPIO);
	SetDIR(LCD_D6,SALIDA);
	SetMODE_OD(LCD_D6,OPEN_DRAIN);
	SetPIN(LCD_D6,OFF);

	SetPINSEL(LCD_D7,PINSEL_GPIO);
	SetDIR(LCD_D7,SALIDA);
	SetMODE_OD(LCD_D7,OPEN_DRAIN);
	SetPIN(LCD_D7,OFF);
}


void WComando8(uint8_t comando){
	SetPIN(LCD_RS,OFF);
	// Parte Alta del comando
	SetPIN(LCD_D4,((comando & 0x10) >> 4));
	SetPIN(LCD_D5,((comando & 0x20) >> 5));
	SetPIN(LCD_D6,((comando & 0x40) >> 6));
	SetPIN(LCD_D7,((comando & 0x80) >> 7));
	LatchLcd();
	LCDDelay(2);

	// Parte Baja del comando
	SetPIN(LCD_D4,(comando & 0x01));
	SetPIN(LCD_D5,((comando & 0x02) >> 1));
	SetPIN(LCD_D6,((comando & 0x04) >> 2));
	SetPIN(LCD_D7,((comando & 0x08) >> 3));
	LatchLcd();
	LCDDelay(2);
}



void WComando4(uint8_t comando){
	SetPIN(LCD_RS,OFF);
	SetPIN(LCD_D4,((comando & 0x10) >> 4));
	SetPIN(LCD_D5,((comando & 0x20) >> 5));
	SetPIN(LCD_D6,((comando & 0x40) >> 6));
	SetPIN(LCD_D7,((comando & 0x80) >> 7));
	LatchLcd();
	LCDDelay(2);
}


void WDato (uint8_t dato)
{
	SetPIN(LCD_RS,ON);
	// Parte Alta del comando
	SetPIN(LCD_D4,((dato & 0x10) >> 4));
	SetPIN(LCD_D5,((dato & 0x20) >> 5));
	SetPIN(LCD_D6,((dato & 0x40) >> 6));
	SetPIN(LCD_D7,((dato & 0x80) >> 7));
	LatchLcd();
	LCDDelay(2);

	// Parte Baja del comando
	SetPIN(LCD_D4,(dato & 0x01));
	SetPIN(LCD_D5,((dato & 0x02) >> 1));
	SetPIN(LCD_D6,((dato & 0x04) >> 2));
	SetPIN(LCD_D7,((dato & 0x08) >> 3));
	LatchLcd();
	LCDDelay(2);

}




void Inic_LCD(void)
{
	Inic_PinesLCD();		// Configura pines de interfaz con LCD
	LCDDelay(100);			// 400 Demora inicial para dejatr que todo se estabilice

	LCDDelay(10); 			// 25ms
	WComando4(0x30);

	LCDDelay(3);			// 7,5ms
	WComando4(0x30);

	LCDDelay(2);			// 5ms
	WComando4(0x30);

	// A partir de aca se puede chequear BF
	LCDDelay(2);			// 5ms
	WComando4(0x20);		// 4 bits

	LCDDelay(2);			// 5ms
	WComando4(0x20);		// 4 bits

	LCDDelay(2);			// 5ms
	WComando4(0x80);		// Clear del LCD

	LCDDelay(2);			// 5ms
	WComando4(0x00);		// Display y cursos off

	LCDDelay(2);			// 5ms
	WComando4(0xC0);		// Display de dos lineas caractetes de 5x11

	LCDDelay(2);			// 5ms
	WComando8(LCD_CLEAR);	// Limpia LCd y va a Home

}
