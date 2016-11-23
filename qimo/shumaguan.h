sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;

uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
                        0xf8,0x80,0x90,0xff,0xBF};
int printfsudu[]={0,0,0,0};
  
void delay(uchar i)
{
  uchar j,k; 
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
  
void display()
{
  	P0=table[printfsudu[0]];
   P2_0 = 0;
   delay(5);
   P2_0 = 1;

   P0=table[printfsudu[1]];
   P2_1 = 0;
   delay(5);
   P2_1 = 1;

   P0=table[printfsudu[2]];
   P2_2 = 0;
   delay(5);
   P2_2 = 1;

    P0=table[printfsudu[3]];
   P2_3 = 0;
   delay(5);
   P2_3 = 1;
   
}
