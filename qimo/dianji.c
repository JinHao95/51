/*
���뻷�� keil4
ʵ��ƽ̨ QX-mini51
2015/12/24-22:30����ͨ��
��ӭת�أ����뱣�޴˶�����

Copyrighnt@ 2015 jiangjinhao
							*/
//��ӭת�أ����뱣���˶�����
#include <reg52.h>
#define uchar unsigned char 
#define uint unsigned int
//#include <LCD1602.h>
#include <shumaguan.h>
sbit key1 = P3 ^ 5;		//����1 pwm+
sbit key2 = P3 ^ 4;		//����2	pwm-
sbit key3 = P3 ^ 3;		//����3 ��ת
sbit key4 = P3 ^ 2;		//����4 

sbit led1 = P1 ^0;		//led
sbit led2 = P1 ^1;
sbit led3 = P1 ^2;
sbit led4 = P1 ^3;

sbit Bxiang = P3 ^0;	//B������

uchar count=0,number=0,num=0;

sbit in1 = P2 ^6; // IN1
sbit in2 = P2 ^7; // IN2   

sbit PWM_OUT = P3 ^7;  // pwm���

sbit BEEP =P3 ^ 6; //������

uint pwm_h; 
uint pwm_l;
uint sudu=500;

void SetSpeed(uint num) //
{    
	if(num<1000) 		 //pwm���� 1ms	����1K һ·pwm
	{    
		pwm_l=65535-num;      //0.num ��ms��
		pwm_h=65536-(1001-num);  //��1-0.num��ms
	}  
	else  return ;
}
void init()
{
	in1=1;
	in2=0; 	
																    
    TMOD = 0x11;   //0001	  0001

    TH0 = 0x00;
    TL0 = 0x00;
    EA = 1;
    ET0 = 1;//��ʱ������0����ж��������λ
    TR0 = 1;

  	TH1 = 0x4C;//50ms
    TL1 = 0x00;
    ET1 = 1; //��ʱ������1����ж��������λ
    TR1 = 1;

	EX0 = 1;//�ⲿ�ж�1
	IT0 = 1;   //��Ե������ʽ
	
}
void scanf_key()
{

	uchar i=1;
	if(key1==0)
		{
			delay(5);	  //�������� ����
			if(key1==0)	  
			{
				BEEP=~BEEP;
				while(!key1);
				BEEP=~BEEP;
				if(sudu>949)	 
					sudu=960;
				else
				 sudu+=100;
			}
		}
		if(key2==0)				 //�������� ����
		{
			delay(5);	
			if(key2==0)	  
			{
				BEEP=~BEEP;
				while(!key2);
				BEEP=~BEEP;
				if(sudu<50)
					sudu=0;
				else
					sudu-=100;
			}
		}
		if(key3==0)			 //��������  ��ת
		{
			delay(5);	 
			if(key3==0)	 
			{
				BEEP=~BEEP;
				while(!key3);
				BEEP=~BEEP;
				//sudu+=50;
				in1=~in1;
	            in2=~in2;
			}
		}
	/*	if(key4==0)
		{
			delay(5);	 
			if(key4==0)	 
			{
				while(!key4);
				//sudu-=50;
			//	led4=~led4;
			}
		} */
//	dis2[13]=sudu/100;
//	dis2[14]=sudu%100/10;
	printfsudu[0]=sudu/100;	 //pwm ռ�ձ�
							//ռ�ձ���ָ�����źŵ�ͨ��ʱ����ͨ������֮�ȡ���һ��������������������У��緽������������ĳ���ʱ�������������ڵı�ֵ��					
}  /*
void toodisplay()
{
	display();
}  */
void main()
{
	//delay(500);
	init();			//��ʼ��
	//lcd_init ();
	SetSpeed(sudu);	
	led1=0;		  
   // display();
   	BEEP=1;
	while(1)
	{
		printfsudu[2]=(number/10);	//�ٶ�ʮλD 
		printfsudu[3]=(number%10);	//�ٶȸ�λD
	//	printfsudu[3]=(number*10%10); 
		scanf_key();				//����ɨ��
	//	delay(100);
		SetSpeed(sudu);				//����ռ�ձ�
	//	display_lcd1026();
		display();				  //�������ʾ
	}

}
   
void iINT0Interrupt(void) interrupt 0	 //�ⲿ�ж�0
{
	count++;
	if(Bxiang==0)			   //�ж�B��
		printfsudu[1]=11;	   //  �� - ��
	//	dis2[5]='+';
	else 
	    printfsudu[1]=10;	  // ' ' 
	//   dis2[5]='-';
}	
 
void int1() interrupt 1		//��ʱ��0
{
	static char stepflag;

	TR0=0;
//	led1=~led1;
	if(stepflag==1)
	{
		TL0=pwm_h%56;	 //	pwm_h=65536-(1001-num); ��1-0.num��ms
		TH0=pwm_h/256; //���������ʱ�� ��1-0.num��ms   ����1ms
		PWM_OUT=0;
		stepflag=0;
	}
	else
	{
		TL0=pwm_l%256;	   //pwm_l=65535-num; 0.num ��ms��
		TH0=pwm_l/256; //������ĳ���ʱ�� 0.num ms  ����1ms
		PWM_OUT=1;
		stepflag=1;
	}  
	 /* num++;
	if(num==20)
	{	num=0;
		number=count;
		count=0;

	led1=~led1;
		}*/	
	TR0=1;
}

void Timer1Interrupt(void) interrupt 3		//��ʱ��1
{
    TH1 = 0x4C;			  //50ms
    TL1 = 0x00;
	if(++num==3)		   //50ms*3	 
	{	num=0;
		number=count;	   //���������½��ظ���
		count=0;
		led1=~led1;		   //led��˸
	//	dis2[6]=number%1000/100+0x30;
	//	dis2[7]=number%1000%100/10+0x30;
	//	display();
	}
 
}