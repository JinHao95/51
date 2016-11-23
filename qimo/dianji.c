/*
编译环境 keil4
实验平台 QX-mini51
2015/12/24-22:30测试通过
欢迎转载，但请保修此段文字

Copyrighnt@ 2015 jiangjinhao
							*/
//欢迎转载，但请保留此段文字
#include <reg52.h>
#define uchar unsigned char 
#define uint unsigned int
//#include <LCD1602.h>
#include <shumaguan.h>
sbit key1 = P3 ^ 5;		//按键1 pwm+
sbit key2 = P3 ^ 4;		//按键2	pwm-
sbit key3 = P3 ^ 3;		//按键3 翻转
sbit key4 = P3 ^ 2;		//按键4 

sbit led1 = P1 ^0;		//led
sbit led2 = P1 ^1;
sbit led3 = P1 ^2;
sbit led4 = P1 ^3;

sbit Bxiang = P3 ^0;	//B向输入

uchar count=0,number=0,num=0;

sbit in1 = P2 ^6; // IN1
sbit in2 = P2 ^7; // IN2   

sbit PWM_OUT = P3 ^7;  // pwm输出

sbit BEEP =P3 ^ 6; //蜂鸣器

uint pwm_h; 
uint pwm_l;
uint sudu=500;

void SetSpeed(uint num) //
{    
	if(num<1000) 		 //pwm周期 1ms	产生1K 一路pwm
	{    
		pwm_l=65535-num;      //0.num （ms）
		pwm_h=65536-(1001-num);  //（1-0.num）ms
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
    ET0 = 1;//定时计数器0溢出中断允许控制位
    TR0 = 1;

  	TH1 = 0x4C;//50ms
    TL1 = 0x00;
    ET1 = 1; //定时计数器1溢出中断允许控制位
    TR1 = 1;

	EX0 = 1;//外部中断1
	IT0 = 1;   //边缘触发方式
	
}
void scanf_key()
{

	uchar i=1;
	if(key1==0)
		{
			delay(5);	  //按键消抖 加速
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
		if(key2==0)				 //按键消抖 减速
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
		if(key3==0)			 //按键消抖  反转
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
	printfsudu[0]=sudu/100;	 //pwm 占空比
							//占空比是指脉冲信号的通电时间与通电周期之比。在一串理想的脉冲周期序列中（如方波），正脉冲的持续时间与脉冲总周期的比值。					
}  /*
void toodisplay()
{
	display();
}  */
void main()
{
	//delay(500);
	init();			//初始化
	//lcd_init ();
	SetSpeed(sudu);	
	led1=0;		  
   // display();
   	BEEP=1;
	while(1)
	{
		printfsudu[2]=(number/10);	//速度十位D 
		printfsudu[3]=(number%10);	//速度各位D
	//	printfsudu[3]=(number*10%10); 
		scanf_key();				//按键扫描
	//	delay(100);
		SetSpeed(sudu);				//调节占空比
	//	display_lcd1026();
		display();				  //数码管显示
	}

}
   
void iINT0Interrupt(void) interrupt 0	 //外部中断0
{
	count++;
	if(Bxiang==0)			   //判断B向
		printfsudu[1]=11;	   //  （ - ）
	//	dis2[5]='+';
	else 
	    printfsudu[1]=10;	  // ' ' 
	//   dis2[5]='-';
}	
 
void int1() interrupt 1		//定时器0
{
	static char stepflag;

	TR0=0;
//	led1=~led1;
	if(stepflag==1)
	{
		TL0=pwm_h%56;	 //	pwm_h=65536-(1001-num); （1-0.num）ms
		TH0=pwm_h/256; //负脉冲持续时间 （1-0.num）ms   周期1ms
		PWM_OUT=0;
		stepflag=0;
	}
	else
	{
		TL0=pwm_l%256;	   //pwm_l=65535-num; 0.num （ms）
		TH0=pwm_l/256; //正脉冲的持续时间 0.num ms  周期1ms
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

void Timer1Interrupt(void) interrupt 3		//定时器1
{
    TH1 = 0x4C;			  //50ms
    TL1 = 0x00;
	if(++num==3)		   //50ms*3	 
	{	num=0;
		number=count;	   //保修脉冲下降沿个数
		count=0;
		led1=~led1;		   //led闪烁
	//	dis2[6]=number%1000/100+0x30;
	//	dis2[7]=number%1000%100/10+0x30;
	//	display();
	}
 
}