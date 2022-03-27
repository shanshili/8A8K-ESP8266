/*
	串口1用于ESP通信
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
"Confidentiality confidentiality"
};

code const u8 MessageSubscribeTopic[57] = {
"Confidentiality confidentiality"
};
//**********************************UART-INIT*****************************************//
void Uart_Init()  //串口1 模式1  8位数据 定时器1 115200bps@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xCC;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	EA = 1;
	ES = 1;
	TR1 = 1;		//启动定时器1
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
    strcat(oristr, "\":"); // {"id":"1234567","version":"1.0","params":{"temp"：
    if (value < 0)
    {
        temp[0] = '-';   //如果小于0，加负号
    }
    else
    {
        temp[0] = ' ';
    }
    temp[1] = value / 10 + '0';
    temp[2] = value % 10 + '0';
    strcat(oristr, temp); // {"id":"1234567","version":"1.0","params":{"temp"：温度值
    strcat(oristr, ",");  // {"id":"1234567","version":"1.0","params":{"temp"：温度值，
}
//**********************************ESP8266*****************************************//
//Commend Send
void ESP8266_Set(u8 *puf) 	
{    
		Clear_Table();//每此发送命令前清空前一次接收到的回复
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
	ESP8266_Set("AT+CWJAP_CUR=\"Confidentiality confidentiality\",\"Confidentiality confidentiality\"");//Connect to the ap
	delay_s(6);
}
//JSON报文发送
void IOT_Publish_Temphum(int temp, unsigned int hum)
{
    // example: {"id":"12","version":"1.0","params":{"Temp":4,"Hum":4,"Gas":4},"method":"thing.event.property.set"}
    unsigned int i,j=0;
    unsigned char id[8] = "1234567";//姑且先写死
    unsigned char str[300] = {0x30};
    unsigned char content[200] = "{\"id\":\""; // 7nums
    //unsigned char contentnum = 7;              // id from 8th
    unsigned char totalnum = 0; // TOPIC+CONTENT
    unsigned char enlargedbit = 0;
		// Random id
	//    Adc_Init();  //adc获得随机数
	//    for (i = 0; i < 8; i++)
	//    {
	//        id[i] = '0' + (getRandom() % 10);
	//    }
    strcat(content, id);  //连接content 和 id ， 即  {"id":"1234567
    // Payload
    strcat(content, "\",\"version\":\"1.0\",\"params\":{");   //  {"id":"1234567","version":"1.0","params":{
    toJson(content, "temp", temp);                           
    toJson(content, "hum", hum);                              
    content[strlen(content) - 1] = '}';                       //  {"id":"1234567","version":"1.0","params":{"Temp":4,"Hum":4,"Gas":4}
    strcat(content, ",\"method\":\"thing.event.property.set\"}");
    // Length
    totalnum = 52 + strlen(content);  //50 ：length  of topic 
		str[1] = totalnum + 0x00;
		str[2] = 0X01;
		enlargedbit = 1;
		str[2 + enlargedbit ] = 0x00;
		str[3 + enlargedbit ] = 0x32; //topic 的长度
    //TOPIC    /没加topic的长度
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
		ESP8266_Set("AT+CIPSTART=\"TCP\",\"Confidentiality confidentiality\",1883");
		Delay800ms();
		ESP8266_Set("AT+CIPMODE=1");//使能透传
		Delay100ms();
		ESP8266_Set("AT+CIPSEND");//开始透传
		Delay100ms();
		Clear_Table();//发送报文前清空接收
		i = 0;	
		while (i < 115)
		{
			Send_Uart(MessageConnect[i++]);	 //连接报文	    
		}
		i=0;
		Delay100ms();
		P10 = 0;  //连接成功亮一次灯		
		while (i < 57)
		{
			Send_Uart(MessageSubscribeTopic[i++]);	 //订阅报文	    
		}
		i=0;
		P10 = 1;  //订阅结束灭灯
		for(i=0;i<12;i++)
		{
			IOT_Publish_Temphum(1+i,1+i);   //循环发送温度
			delay_s(10);
		}
		Delay800ms();
		Send_Uart(0xE0); //连续发送，无法断开
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


