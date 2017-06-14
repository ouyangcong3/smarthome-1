#include <STC12C5A60S2.H>
#include <TimeAndDelay.H>
#include <IOSetting.H>
#include <AT24C16.H>

#include <FlashOperate.H>

void start() //������I��C������
{
	SDA=1; 
	Delay2us(); 
	SCL=1; 
	Delay2us();
	SDA=0; 
	Delay2us();
	SCL=0; 
	Delay2us();
}
void stop() //ֹͣ��I��C������
{
	SDA=0; 
	Delay2us(); 
	SCL=1; 
	Delay2us(); 
	SDA=1; 
	Delay2us();
}
void IICwrite(unsigned char y) //дһ���ֽ�
{
unsigned char x,temp;
	temp=y;
	for (x=0;x<8;x++)
	{
		temp=temp<<1; 
		SCL=0; 
		Delay2us(); 
		SDA=CY; 
		Delay2us(); 
		SCL=1; 
		Delay2us();
	}
	SCL=0; 
	Delay2us(); 
	SDA=1; 
	Delay2us();
}

unsigned char IICread() //��һ���ֽ�
{
	unsigned char w,v,u=0;
	SCL=0; 
	Delay2us(); 
	SDA=1;
	for (w=0;w<8;w++)
	{
		Delay2us(); 
		SCL=1; 
		Delay2us();
		if (SDA==1) v=1;
		else v=0;
		u=(u<<1)|v;
		SCL=0;
	}
	Delay2us(); 
	return(u);
}
void ACK() //(I��C��Ӧ��λ
	{
	unsigned char t=0;
	SCL=1; 
	Delay2us();
	while ((SDA==1)&&(t<255))t++;
	SCL=0; 
	Delay2us();
}

/********************************************************************
                AT24C16 ��ʼ���ӳ���
*********************************************************************/
void Init_24C16() //
{
	SCL=1;
	Delay2us();
	SDA=1;                    
	Delay2us();
}
/********************************************************************
                ��AT24C16 ��address��ַ�ж���һ�ֽ�����
*********************************************************************/
unsigned char Read_Byte_24C16(unsigned int address)
{
	unsigned char s;
	union{
		unsigned int a;
		unsigned char b[2];
		}exchange;
	unsigned char device;
	exchange.a=address;	  //1xa0+page address
	device = (exchange.b[0]&0x07);
	device=device<<1;
	device |= 0xa0;
	start(); 
	IICwrite(device);
	ACK(); 
	IICwrite(exchange.b[1]);
	ACK();
	device |= 0x01; 
	start();
	IICwrite(device); 
	ACK();
	s=IICread();
	stop();
	Delay2us();
	Delay2us();
	return(s);
}
/********************************************************************
                ��AT24C16 ��address��ַ��д��һ�ֽ�����
*********************************************************************/
void Write_Byte_24C16(unsigned int address,unsigned char info)
{
	
	union{
		unsigned int a;
		unsigned char b[2];
		}exchange;
	unsigned char device;

//	echoint(address) ;

	exchange.a=address;	  //0xa0+page address
	device = (exchange.b[0]);
	device=device<<1;
	device |= 0xa0;
	start(); 
	IICwrite(device);
	ACK(); 
	IICwrite(exchange.b[1]);
	ACK(); 
	IICwrite(info);
	ACK(); 
	stop();
	Delay2ms();//2������ʱ,�����ٿ���!!
}
void Write_Block_24C16(unsigned int address,unsigned char* info,unsigned char length)
{

/***��д��ʱû�гɹ�,����д+��ʱʵ��****/
//	union{
//		unsigned int a;
//		unsigned char b[2];
//		}exchange;
//	unsigned char device,i;
//	exchange.a=address;	  //0xa0+page address
//	device = (exchange.b[0]&0x07);
//	device=device<<1;
//	device |= 0xa0;
//	start(); 
//	IICwrite(device);
//	ACK(); 
//	IICwrite(exchange.b[1]);
//	ACK(); 
//	for(i=0;i<length;i++)
//	{
//		IICwrite(*info);
//		ACK();
//		info++; 
//	}	
//	stop();
//	Delay2ms();//2������ʱ,�����ٿ���!!
	unsigned char i;
	for(i=0;i<length;i++)
	{
		Write_Byte_24C16(address+i,*info);
		info++;
	}
}