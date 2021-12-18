#include <reg52.h>
#include <intrins.h>

sbit led0 = P0^0;
sbit led1 = P0^1;
sbit led2 = P0^2;
sbit led3 = P0^3;
sbit led4 = P0^4;
sbit led5 = P0^5;
sbit led6 = P0^6;
sbit led7 = P0^7;


unsigned char IR_Time;

unsigned char IR_Date[33];

unsigned char IR_Byte[4]; 
bit StartFlag; 

bit IR_DateOK; 

bit IR_DecodeOK;

void init()
{EA=1; 
ET0=1;
TMOD|=0x02;
TH0=TL0=0;
TR0=1;

EX0=1;
IT0=1; 

TMOD|=0x20;
TH1=0xfd;
TL1=0xfd; 
TR1=1; 
SM1=1;
}

void time0( )interrupt 1 
{IR_Time++;
}

void int0( )interrupt 0
{static unsigned char i; 

if(StartFlag)
{
if((IR_Time>=32)&&(IR_Time<53)) i=0;
IR_Date[i]=IR_Time;
IR_Time=0;
i++;
if(i==33)
{i=0; 
IR_DateOK=1; 
StartFlag=0; 
}
}
else
{StartFlag=1; 
IR_Time=0; 
}
}

void IR_Decode()
{unsigned char i,j,k=1; 
for(i=0;i<4;i++)
{for(j=0;j<8;j++)
{if(IR_Date[k]>5) IR_Byte[i]|=0x80; 
if(j<7) IR_Byte[i]>>=1; 
k++;
}
}
if(k==33)
IR_DecodeOK=1; 

}

void main()
{ 
	unsigned char i; 
init(); 
	
while(1)
{ 
			if(IR_DateOK)
		{ 
			IR_DateOK=0;
			IR_Decode(); 
			
			if(IR_DecodeOK) 
			{ 
				IR_DecodeOK=0;
				switch(IR_Byte[2])
				{
					case 0x45: led0 = ~led0; break;
					case 0x46: led1 = ~led1; break;
					case 0x47: led2 = ~led2; break;
					case 0x44: led3 = ~led3; break;
					case 0x40: led4 = ~led4; break;
					case 0x43: led5 = ~led5; break;
					case 0x07: led6 = ~led6; break;
					case 0x15: led7 = ~led7; break;
					case 0x09: led7 = ~led7; break;
					default: P0 = 0XFF; break;
				}

				SBUF=IR_Byte[2]; 
				while(!TI);
				TI=0; 
				
			}
		}
}
}