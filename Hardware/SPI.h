#ifndef _SPI_H
#define _SPI_H
#include "sys.h"

void SPI1_Init(void);			 //???SPI1?
void SPI1_SetSpeed(u8 SpeedSet); //??SPI1??   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1????????

void SPI2_Init(void);			 //???SPI2?
void SPI2_SetSpeed(u8 SpeedSet); //??SPI2??   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2????????

#endif

