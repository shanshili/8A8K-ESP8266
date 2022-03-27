/*
	����1����ESPͨ��
*/
//=================================================================================//
// OLED
//   VCC         DC 5V/3.3V    
//   GND           GND        
//   D1            P15          
//   CS            P13         
//   RES           P33          
//   DC            P12          
//   D0            P17          
//==================================================================================//

#include "8a8k.h"
#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "gui.h"
#include "test.h"
#include <stdio.h>
#include <string.h>

u8 save = 0;
u8 flag = 0;
u8 Receive;                                       
u8 Recive_table[150]=0; 
u8 Timer0 = 0;
void Clear_Table(void);
void IOT_Publish_Temphum(int temp, unsigned int hum);

//*********************************MQTT message*************************************//

code const u8 MessageConnect[115] = {
		0X10,0X71,0X00,0X04,0X4D,0X51,0X54,0X54,0X04,0XC2,0X00,0X64,0X00,0X26,0X30,
		0X31,0X34,0X34,0X7c,0X73,0X65,0X63,0X75,0X72,0X65,0X6d,0X6f,0X64,0X65,0X3d,
		0X33,0X2c,0X73,0X69,0X67,0X6e,0X6d,0X65,0X74,0X68,0X6f,0X64,0X3d,0X68,0X6d,
		0X61,0X63,0X73,0X68,0X61,0X31,0X7c,0X00,0X13,0X64,0X65,0X76,0X69,0X63,0X65,
		0X31,0X26,0X61,0X31,0X55,0X55,0X42,0X68,0X76,0X65,0X4b,0X4d,0X76,0X00,0X28,
		0X61,0X36,0X39,0X31,0X34,0X65,0X36,0X36,0X38,0X65,0X30,0X65,0X36,0X61,0X34,
		0X64,0X38,0X63,0X61,0X62,0X35,0X66,0X39,0X31,0X39,0X36,0X61,0X37,0X32,0X66,
		0X32,0X62,0X30,0X65,0X38,0X33,0X34,0X65,0X36,0X35
};

code const u8 MessageSubscribeTopic[57] = {
	  0X82,0X37,0X00,0X0A,0X00,0X32,0X2f,0X73,0X79,0X73,0X2f,0X61,0X31,0X55,0X55,
	  0X42,0X68,0X76,0X65,0X4b,0X4d,0X76,0X2f,0X64,0X65,0X76,0X69,0X63,0X65,0X31,
	  0X2f,0X74,0X68,0X69,0X6e,0X67,0X2f,0X65,0X76,0X65,0X6e,0X74,0X2f,0X70,0X72,
	  0X6f,0X70,0X65,0X72,0X74,0X79,0X2f,0X70,0X6f,0X73,0X74,0X00
};
//**********************************UART-INIT*****************************************//
void Uart_Init()  //����1 ģʽ1  8λ���� ��ʱ��1 115200bps@24.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xCC;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	EA = 1;
	ES = 1;
	TR1 = 1;		//������ʱ��1
	RI = 0;
}
//Esp Uart Send
void Send_Uart(u8 value) 
{  
		ES=0;                 
		TI=0;                 
		SBUF=value;           
		while(TI==0);          
		TI=0;                 
		ES=1;                
}
//***************************BASIC STRING HANDLING*************************************//
//clear receive table
void Clear_Table()
{
	  u8 i;
		for (i = 0; i < 150; i++)
	  Recive_table[i] = 0;
}
void toJson(char *oristr, const char *str, int value)
{
    char temp[] = "000";
    strcat(oristr, "\"");  // {"id":"1234567","version":"1.0","params":{"
    strcat(oristr, str);   // {"id":"1234567","version":"1.0","params":{"temp
    strcat(oristr, "\":"); // {"id":"1234567","version":"1.0","params":{"temp"��
    if (value < 0)
    {
        temp[0] = '-';   //���С��0���Ӹ���
    }
    else
    {
        temp[0] = ' ';
    }
    temp[1] = value / 10 + '0';
    temp[2] = value % 10 + '0';
    strcat(oristr, temp); // {"id":"1234567","version":"1.0","params":{"temp"���¶�ֵ
    strcat(oristr, ",");  // {"id":"1234567","version":"1.0","params":{"temp"���¶�ֵ��
}
//**********************************ESP8266*****************************************//
//Commend Send
void ESP8266_Set(u8 *puf) 	
{    
		Clear_Table();//ÿ�˷�������ǰ���ǰһ�ν��յ��Ļظ�
	    while(*puf!='\0')                    
		{   
				Send_Uart(*puf);               
				puf++;    
		}            
		Send_Uart('\r');                       
		Send_Uart('\n');		
}
//Esp Initialize
void esp_int()
{	    
	ESP8266_Set("AT+RST");
	Delay1000ms();	  Delay1000ms();	  Delay1000ms();
	ESP8266_Set("ATE0");
	Delay100ms();	
	ESP8266_Set("AT+CWMODE_CUR=1");//station pattern
	Delay100ms();	
	ESP8266_Set("AT+CIPMUX=0");//Enabling single connection
	Delay100ms();			
	ESP8266_Set("AT+CWJAP_CUR=\"316316\",\"@@@@@@@@\"");//Connect to the ap
	delay_s(6);
}
//JSON���ķ���
void IOT_Publish_Temphum(int temp, unsigned int hum)
{
    // example: {"id":"12","version":"1.0","params":{"Temp":4,"Hum":4,"Gas":4},"method":"thing.event.property.set"}
    unsigned int i,j=0;
    unsigned char id[8] = "1234567";//������д��
    unsigned char str[300] = {0x30};
    unsigned char content[200] = "{\"id\":\""; // 7nums
    //unsigned char contentnum = 7;              // id from 8th
    unsigned char totalnum = 0; // TOPIC+CONTENT
    unsigned char enlargedbit = 0;
		// Random id
	//    Adc_Init();  //adc��������
	//    for (i = 0; i < 8; i++)
	//    {
	//        id[i] = '0' + (getRandom() % 10);
	//    }
    strcat(content, id);  //����content �� id �� ��  {"id":"1234567
    // Payload
    strcat(content, "\",\"version\":\"1.0\",\"params\":{");   //  {"id":"1234567","version":"1.0","params":{
    toJson(content, "temp", temp);                           
    toJson(content, "hum", hum);                              
    content[strlen(content) - 1] = '}';                       //  {"id":"1234567","version":"1.0","params":{"Temp":4,"Hum":4,"Gas":4}
    strcat(content, ",\"method\":\"thing.event.property.set\"}");
    // Length
    totalnum = 52 + strlen(content);  //50 ��length  of topic 
		str[1] = totalnum + 0x00;
		str[2] = 0X01;
		enlargedbit = 1;
		str[2 + enlargedbit ] = 0x00;
		str[3 + enlargedbit ] = 0x32; //topic �ĳ���
    //TOPIC    /û��topic�ĳ���
    for (i = 0; i < 50; i++)
    {
        str[4 + enlargedbit + i] = MessageSubscribeTopic[6+i];
    }
    //CONTENT
    for (i = 0; i < strlen(content); i++)
    {
        str[54 + enlargedbit + i] = content[i];
    }
	//    Esp8266_Send(str, 2 + enlargedbit + totalnum);
		while(j<(4 + enlargedbit + totalnum))
		{
				Send_Uart(str[j++]);
		}
}

//*************************************MAIN********************************************//
void main()
{  
	unsigned int i = 0;	
	Uart_Init();
	esp_int();
	delay_s(15);
	while(1)
	{
		ESP8266_Set("AT+CIPSTART=\"TCP\",\"a1UUBhveKMv.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883");
		Delay800ms();
		ESP8266_Set("AT+CIPMODE=1");//ʹ��͸��
		Delay100ms();
		ESP8266_Set("AT+CIPSEND");//��ʼ͸��
		Delay100ms();
		Clear_Table();//���ͱ���ǰ��ս���
		i = 0;	
		while (i < 115)
		{
			Send_Uart(MessageConnect[i++]);	 //���ӱ���	    
		}
		i=0;
		Delay100ms();
		P10 = 0;  //���ӳɹ���һ�ε�		
		while (i < 57)
		{
			Send_Uart(MessageSubscribeTopic[i++]);	 //���ı���	    
		}
		i=0;
		P10 = 1;  //���Ľ������
		for(i=0;i<12;i++)
		{
			IOT_Publish_Temphum(1+i,1+i);   //ѭ�������¶�
			delay_s(10);
		}
		Delay800ms();
		Send_Uart(0xE0); //�������ͣ��޷��Ͽ�
		Send_Uart(0x00);	
		ESP8266_Set("+++");
		delay_s(2); 		    
		ESP8266_Set("AT+CIPMODE=0");
		delay_s(15);
   }
}

//**********************************interrupt**********************************************/
//uart inrerrupt 
void Uart1_Interrupt() interrupt 4         
{     
	    ES = 0;  
			if( RI == 1 )  
		  {  	
						RI=0;   
						Receive=SBUF;                                      
						Recive_table[save]=Receive;			
				    if((Recive_table[save]=='\r')|(Recive_table[save]=='K'))
						{                     
										save = 0;
							      flag = 1;
						}    
						else save++;                                       
			}   
			else TI=0;  
      ES = 1;			
}


