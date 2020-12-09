/*
 * SD.h
 *
 *  Created on: 28/10/2014
 */

#ifndef SD_H_
#define SD_H_


#include "aplicacion.h"

#define START_BLOCK	0


/* Comandos MMC/SDC */
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD9	(0x40+9)	/* SEND_CSD */
#define CMD10	(0x40+10)	/* SEND_CID */
#define CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define CMD23	(0x40+23)	/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */


/* Flags indicadores de tipo de memoria (CardType) */
#define CT_MMC				0x01	/* MMC ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_SDC				(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK			0x08	/* Block addressing */

//Macros
#define	CLK_SLOW()		CPSR=250		/* Slow clock (100kHz-400kHz) */
#define	CLK_FAST()		CPSR=4			/* Fast clock (25MHz) */


//Prototipos
uint32_t SD_Inic (void);
uint8_t SD_Read (uint8_t* buf, uint32_t block);
uint8_t SD_Write (uint8_t* buf, uint32_t block);


void SD_Deassert (void);
uint32_t SD_Assert (void);
uint8_t SD_Ready(void);
uint8_t SD_EnviarComando(uint8_t cmd, uint32_t arg);
uint8_t SD_RecibirBloque(uint8_t *buf, uint32_t size);
uint8_t SD_EnviarBloque(uint8_t *buf, uint32_t size, uint8_t token);




#endif /* SD_H_ */
