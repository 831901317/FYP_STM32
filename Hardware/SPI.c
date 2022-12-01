#include "stm32f10x.h"
#include "SPI.h"

//SPI2
void SPI2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	/* SPI?IO??SPI?????? */
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* SPI?IO??? */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行时钟的第二个跳变沿数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//使用软件出触发
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//波特率分频为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//数据传输从MSB开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //初始化
	
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	SPI2_ReadWriteByte(0xff);//启动传输
}

//SPI2速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB1时钟一般为36Mhz:
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI2->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI速度
	SPI_Cmd(SPI2,ENABLE); //使能SPI2
} 

//SPI2 读写一个字节
//TxData:要写入的字节
//返回值：读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//等待发送区空
	
	SPI_I2S_SendData(SPI2, TxData); //通过SPI2发送一个byte 数据
		
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //  等待接收完一个byte
 
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPI2最近接收的数据
 		    
}


