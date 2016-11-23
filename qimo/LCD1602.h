#include<stdio.h>
#include <intrins.h>
#include <string.h>
#define uchar unsigned char 
#define uint unsigned int
typedef bit BOOL;

sbit rs=P0^7;
sbit rw=P0^6;
sbit ep=P0^5;

uchar  dis1 []={"     QXMCU     "};
uchar  dis2 []={"HTTP://QXMCU.COM"};

  uchar i;
void delay (int m)
{
 uchar i,j;
 for (i=0;i<m;i++)
 for (j=0;j<253;j++);
}

 BOOL lcd_bz()
{
  BOOL result;
  rs=0;           // ¶ÁÃ¦ÐÅºÅ
  rw=1;
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  result = (BOOL)(P2&0x80);
  ep=0;
  return result;
}

void lcd_wcmd (uchar cmd)
{
  while (lcd_bz());
  rs=0;
  rw=0;
  ep=0;
  _nop_();
  _nop_();
  P2=cmd ;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=0;
}

void lcd_pos (uchar pos)
{
  lcd_wcmd (pos|0x80);
}

void lcd_wdat (uchar dat)
{
   while (lcd_bz());
  rs=1;
  rw=0;
  ep=0;
  _nop_();
  _nop_();
  P2=dat ;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=0;
}

void lcd_init ()
{  
  lcd_wcmd (0x38);
  delay (1);
  lcd_wcmd (0x0c);
  delay (1);
  lcd_wcmd (0x06);
  delay (1);
  lcd_wcmd (0x01);
  delay (1);
}

display_lcd1602 ()
{
  
 // lcd_init ();
  delay (10);

  lcd_pos (0);
  i=0;
  while (dis1 [i]!='\0')
   {
     lcd_wdat (dis1 [i]);
     i++;	
   } 

   lcd_pos (0x40);
  i=0;
  while (dis2 [i]!='\0')
   {
     lcd_wdat (dis2 [i]);
     i++;	
   } 
  
} 