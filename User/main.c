#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "AD.h"
#include "SPI.h"
#include "SD.h"
#include <stdlib.h>

//uint16_t ADC_DATA[4000] = {0};
//uint16_t i = 0;

uint16_t ADValue;
float Voltage;
u8 i=0;
u32 sd_size;
u8 sd_buf[6] = {'A', 'B', 'C', 'D', 'E', 'F'};

//PA11 - SC
//PB13 - SCK
//PB14 - MISO
//PB15 - MOSI

void SD_Write_Sectorx(u32 sec, u8 *buf)
{
	if(SD_WriteDisk(buf, sec, 1) == 0){
		for(int i = 0; i <= 6; i++){
			Serial_Printf("%c ", buf[i]);
		}
	}
	Serial_Printf("Transfer Over!");
}

void SD_Read_Sectorx(u32 sec){
	u8 *buf;
	u16 i;
	buf = malloc(512);
	if(SD_ReadDisk(buf, sec, 1) == 0){
		OLED_ShowString(1, 1, "USART Sending Data...");
		Serial_Printf("SEXTOR 0 DATA: \r\n");
		for(i = 0; i < 512; i++){
			Serial_Printf("%x ", buf[i]);
		}
		Serial_Printf("\r\nDATA ENDED\r\n");
		OLED_ShowString(2, 1, "USART Sending Data Over!");
	}
	free(buf);
}

int main(void)
{
	OLED_Init();
	Serial_Init();
	AD_Init();
	
	while(SD_Init())
	{
		OLED_ShowString(1,1,"SD Card Error!");
		Serial_Printf("SD Card Error!\r\n");
		Delay_ms(500);
	}
	Serial_Printf("SD Card OK!\r\n");
	OLED_ShowString(1,1,"SD Card OK!");
	
		/* ??SD??? */
    if(SD_Type == 0x06)
	{
		OLED_ShowString(2,1,"SDV2HC OK!");
	}
	else if(SD_Type == 0x04)
	{
		OLED_ShowString(2,1,"SDV2 OK!");
	}
	else if(SD_Type == 0x02)
	{
		OLED_ShowString(2,1,"SDV1 OK!");
	}
	else if(SD_Type == 0x01)
	{
		OLED_ShowString(2,1,"MMC OK!");
	}

	OLED_ShowString(3,1,"SD Card Size:   MB");
	
	sd_size=SD_GetSectorCount();//?????
	sd_size=sd_size>>11;  //??SD???   MB
	Serial_Printf("\nSD size:%dMB\n", sd_size);
	
	sd_buf[0]=sd_size/10000+0x30;
	sd_buf[1]=sd_size%10000/1000+0x30;
	sd_buf[2]=sd_size%10000%1000/100+0x30;
	sd_buf[3]=sd_size%10000%1000%100/10+0x30;
	sd_buf[4]=sd_size%10000%1000%100%10+0x30;
	sd_buf[5]='\0';
	
	OLED_ShowString(4,1,(char *)sd_buf);

/*
	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(2, 1, "Volatge:0.00V");
*/
	while (1)
	{
		uint8_t KeyNum = 0;
		KeyNum = Key_GetNum();
		if(KeyNum == 1){
			SD_Write_Sectorx(1, sd_buf);
		}
		Delay_ms(20);
		
		/*
		ADValue = ADC_ConvertedValue;	
		Voltage = (float)ADValue / 4095 * 3.3;
		
		SD_WriteDisk((u8 *)ADC_ConvertedValue,0,(u8)28172);
		*/
		
		//OLED_ShowNum(1, 9, ADValue, 4);
		//OLED_ShowNum(2, 9, Voltage, 1);
		//OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);
		
		//ADC_DATA[i] = ADValue;
		
		//Serial_Printf("\r\nADValue = %d", ADValue);
		//Serial_Printf("\tVoltage = %f", Voltage);
		//Serial_Printf("\r\n");
		//i++;
		//Delay_ms(100);
	}

}
