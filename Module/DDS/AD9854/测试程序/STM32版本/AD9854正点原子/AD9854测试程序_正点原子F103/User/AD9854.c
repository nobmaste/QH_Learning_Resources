#include <stm32f10x.h>
#include "AD9854.h"
#include "Delay.h"

//**********************ÒÔÏÂÎªÏµÍ³Ê±ÖÓÒÔ¼°ÆäÏà¹Ø±äÁ¿ÉèÖÃ**************************/
/* 
  ´Ë´¦¸ù¾Ý×Ô¼ºµÄÐèÒªÉèÖÃÏµÍ³Ê±ÖÓÒÔ¼°ÓëÆäÏà¹ØµÄÒò×Ó£¬Ò»´ÎÐèÇÒÖ»Ðè¿ªÆôÒ»¸ö   
  CLK_SetÎªÊ±ÖÓ±¶ÆµÉèÖÃ£¬¿ÉÉèÖÃ4~20±¶±¶Æµ£¬µ«×î´ó²»ÄÜ³¬¹ý300MHZ
  Freq_mult_ulongºÍFreq_mult_doulle¾ùÎª2µÄ48´Î·½³ýÒÔÏµÍ³Ê±ÖÓ£¬Ò»¸öÎª³¤ÕûÐÎ£¬Ò»¸öÎªË«¾«¶ÈÐÍ
*/
//#define CLK_Set 7
//const u32  Freq_mult_ulong  = 1340357;
//const double Freq_mult_doulle = 1340357.032;

//#define CLK_Set 9
//const u32  Freq_mult_ulong  = 1042500;		 
//const double Freq_mult_doulle = 1042499.9137431;

// #define CLK_Set 8
// const u32  Freq_mult_ulong  = 1172812;
// const double Freq_mult_doulle = 1172812.403;

#define CLK_Set 10
const u32  Freq_mult_ulong  = 938250;
const double Freq_mult_doulle = 938249.9224;

u8 FreqWord[6];              //6¸ö×Ö½ÚÆµÂÊ¿ØÖÆ×Ö

//====================================================================================
// ³õÊ¼»¯³ÌÐòÇø
//====================================================================================
//====================================================================================
// void GPIO_AD9854_Configuration(void)
// º¯Êý¹¦ÄÜ: AD9854Òý½ÅÅäÖÃº¯Êý
// Èë¿Ú²ÎÊý: ÎÞ
// ·µ»Ø²ÎÊý: ÎÞ
// È«¾Ö±äÁ¿: ÎÞ
// µ÷ÓÃÄ£¿é: RCC_APB2PeriphClockCmd(); GPIO_Init();
// ×¢ÒâÊÂÏî: ÎÞ
//====================================================================================
void GPIO_AD9854_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	// Ê¹ÄÜIO¿ÚÊ±ÖÓ
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 |	GPIO_Pin_7 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;;		// ÍÆÍìÊä³ö
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ¿ØÖÆ¶Ë¿Ú
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1  | GPIO_Pin_2  |	GPIO_Pin_3 | GPIO_Pin_4 
								| GPIO_Pin_5 | GPIO_Pin_6 |	GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;;		// ÍÆÍìÊä³ö
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

//====================================================================================
// AD9854Çý¶¯Çø
//====================================================================================
//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_WR_Byte(u8 Adata)
//º¯Êý¹¦ÄÜ:AD9854²¢ÐÐ¿ÚÐ´ÈëÊý¾Ý
//Èë¿Ú²ÎÊý:adata     µØÖ·&Êý¾Ý
													
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
void AD9854_WR_Byte(u8 Adata)
{
	unsigned char  i;
	
	for(i=8;i>0;i--)
	{
		if(Adata&0x80)						
			SPI_SDI_Set;
		else
			SPI_SDI_Clr;
		Adata <<= 1;	
		AD9854_WR_Clr;
		AD9854_WR_Set;
	}
}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_Init(void)
//º¯Êý¹¦ÄÜ:AD9854³õÊ¼»¯
//Èë¿Ú²ÎÊý:ÎÞ
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
void AD9854_Init(void)
{
	AD9854_SP_Clr;								 // ´®ÐÐ¿ØÖÆ
	AD9854_WR_Clr;
	AD9854_UDCLK_Clr;
	AD9854_RST_Set;                // ¸´Î»AD9854
	Delay_10us (10);
	AD9854_RST_Clr;
	SPI_IO_RST_Clr;
	AD9854_RD_Clr;
	
  AD9854_WR_Byte(CONTR);
// 	AD9854_WR_Byte(0x10);							//¹Ø±Õ±È½ÏÆ÷
	AD9854_WR_Byte(0x00);							//´ò¿ª±È½ÏÆ÷

	AD9854_WR_Byte(CLK_Set);					//ÉèÖÃÏµÍ³Ê±ÖÓ±¶Æµ       
	AD9854_WR_Byte(0x00);							//ÉèÖÃÏµÍ³ÎªÄ£Ê½0£¬ÓÉÍâ²¿¸üÐÂ
	AD9854_WR_Byte(0x60);	

	AD9854_UDCLK_Set;              // ¸üÐÂAD9854Êä³ö
	AD9854_UDCLK_Clr;	
}

//====================================================================================
//º¯ÊýÃû³Æ:void Freq_convert(long Freq)
//º¯Êý¹¦ÄÜ:ÕýÏÒÐÅºÅÆµÂÊÊý¾Ý×ª»»
//Èë¿Ú²ÎÊý:Freq   ÐèÒª×ª»»µÄÆµÂÊ£¬È¡Öµ´Ó0~SYSCLK/2
//³ö¿Ú²ÎÊý:ÎÞ   µ«ÊÇÓ°ÏìÈ«¾Ö±äÁ¿FreqWord[6]µÄÖµ
//ËµÃ÷£º   ¸ÃËã·¨Î»¶à×Ö½ÚÏà³ËËã·¨£¬ÓÐ¹«Ê½FTW = (Desired Output Frequency ¡Á 2N)/SYSCLK
//         µÃµ½¸ÃËã·¨£¬ÆäÖÐN=48£¬Desired Output Frequency ÎªËùÐèÒªµÄÆµÂÊ£¬¼´Freq£¬SYSCLK
//         Îª¿É±à³ÌµÄÏµÍ³Ê±ÖÓ£¬FTWÎª48BitµÄÆµÂÊ¿ØÖÆ×Ö£¬¼´FreqWord[6]
//====================================================================================
void Freq_convert(long Freq)   
{	
  u32 FreqBuf;
  u32 Temp=Freq_mult_ulong;   	       

	u8 Array_Freq[4];			     //½«ÊäÈëÆµÂÊÒò×Ó·ÖÎªËÄ¸ö×Ö½Ú
	Array_Freq[0]=(u8) Freq;
	Array_Freq[1]=(u8)(Freq>>8);
	Array_Freq[2]=(u8)(Freq>>16);
	Array_Freq[3]=(u8)(Freq>>24);

	FreqBuf=Temp*Array_Freq[0];                  
	FreqWord[0]=FreqBuf;    
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[1]);
	FreqWord[1]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[2]);
	FreqWord[2]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[3]);
	FreqWord[3]=FreqBuf;
	FreqBuf>>=8;

	FreqWord[4]=FreqBuf;
	FreqWord[5]=FreqBuf>>8;	
}  

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_SetSine(ulong Freq,uint Shape)
//º¯Êý¹¦ÄÜ:AD9854ÕýÏÒ²¨²úÉú³ÌÐò
//Èë¿Ú²ÎÊý:Freq   ÆµÂÊÉèÖÃ£¬È¡Öµ·¶Î§Îª0~(1/2)*SYSCLK
//         Shape  ·ù¶ÈÉèÖÃ. Îª12 Bit,È¡Öµ·¶Î§Îª(0~4095) ,È¡ÖµÔ½´ó,·ù¶ÈÔ½´ó 
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
void AD9854_SetSine(u32 Freq,u16 Shape)
{
	u8 count;
	u8 i=0;

	Freq_convert(Freq);		              //ÆµÂÊ×ª»»

	for(count=6;count>0;)	              //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
  {
		if(i==0)
			AD9854_WR_Byte(FREQ1);
		AD9854_WR_Byte(FreqWord[--count]);
		i++;
  }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	
	AD9854_WR_Byte(SHAPEI);						  //ÉèÖÃIÍ¨µÀ·ù¶È
	AD9854_WR_Byte(Shape>>8);
	AD9854_WR_Byte((u8)(Shape&0xff));
	
	AD9854_WR_Byte(SHAPEQ);							//ÉèÖÃQÍ¨µÀ·ù¶È
	AD9854_WR_Byte(Shape>>8);
	AD9854_WR_Byte((u8)(Shape&0xff));
	
	AD9854_UDCLK_Set;                   //¸üÐÂAD9854Êä³ö
	AD9854_UDCLK_Clr;

}

//====================================================================================
//º¯ÊýÃû³Æ:void Freq_doublt_convert(double Freq)
//º¯Êý¹¦ÄÜ:ÕýÏÒÐÅºÅÆµÂÊÊý¾Ý×ª»»
//Èë¿Ú²ÎÊý:Freq   ÐèÒª×ª»»µÄÆµÂÊ£¬È¡Öµ´Ó0~SYSCLK/2
//³ö¿Ú²ÎÊý:ÎÞ   µ«ÊÇÓ°ÏìÈ«¾Ö±äÁ¿FreqWord[6]µÄÖµ
//ËµÃ÷£º   ÓÐ¹«Ê½FTW = (Desired Output Frequency ¡Á 2N)/SYSCLKµÃµ½¸Ãº¯Êý£¬
//         ÆäÖÐN=48£¬Desired Output Frequency ÎªËùÐèÒªµÄÆµÂÊ£¬¼´Freq£¬SYSCLK
//         Îª¿É±à³ÌµÄÏµÍ³Ê±ÖÓ£¬FTWÎª48BitµÄÆµÂÊ¿ØÖÆ×Ö£¬¼´FreqWord[6]
//×¢Òâ£º   ¸Ãº¯ÊýÓëÉÏÃæº¯ÊýµÄÇø±ðÎª¸Ãº¯ÊýµÄÈë¿Ú²ÎÊýÎªdouble£¬¿ÉÊ¹ÐÅºÅµÄÆµÂÊ¸ü¾«È·
//         ¹ÈÓê½¨ÒéÔÚ100HZÒÔÏÂÓÃ±¾º¯Êý£¬ÔÚ¸ßÓÚ100HZµÄÇé¿öÏÂÓÃº¯Êývoid Freq_convert(long Freq)
//====================================================================================
//void Freq_double_convert(double Freq)   
//{
//	u32 Low32;
//	u16 High16;
//  double Temp=Freq_mult_doulle;   	            //23ca99Îª2µÄ48´Î·½³ýÒÔ120M
//	Freq*=(double)(Temp);
////	1 0000 0000 0000 0000 0000 0000 0000 0000 = 4294967295
//	High16 = (int)(Freq/4294967295);                  //2^32 = 4294967295
//	Freq -= (double)High16*4294967295;
//	Low32 = (u32)Freq;

//	FreqWord[0]=Low32;	     
//	FreqWord[1]=Low32>>8;
//	FreqWord[2]=Low32>>16;
//	FreqWord[3]=Low32>>24;
//	FreqWord[4]=High16;
//	FreqWord[5]=High16>>8;			
//} 

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_SetSine_double(double Freq,uint Shape)
//º¯Êý¹¦ÄÜ:AD9854ÕýÏÒ²¨²úÉú³ÌÐò
//Èë¿Ú²ÎÊý:Freq   ÆµÂÊÉèÖÃ£¬È¡Öµ·¶Î§Îª0~1/2*SYSCLK
//         Shape  ·ù¶ÈÉèÖÃ. Îª12 Bit,È¡Öµ·¶Î§Îª(0~4095) 
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_SetSine_double(double Freq,u16 Shape)
//{
//	u8 count=0;
//	u8 i=0;

//	Freq_double_convert(Freq);		           //ÆµÂÊ×ª»»
//	 
//	for(count=6;count>0;)	                    //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//    if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }
//	
//	AD9854_WR_Byte(SHAPEI);						  //ÉèÖÃIÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);							//ÉèÖÃQÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));

//	AD9854_UDCLK_Set;                    //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_InitFSK(void)
//º¯Êý¹¦ÄÜ:AD9854µÄFSK³õÊ¼»¯
//Èë¿Ú²ÎÊý:ÎÞ
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_InitFSK(void)
//{
//	AD9854_SP_Clr;
//	AD9854_WR_Clr;                 // ½«¶Á¡¢Ð´¿ØÖÆ¶Ë¿ÚÉèÎªÎÞÐ§
//	AD9854_UDCLK_Clr;
//	AD9854_RST_Set;                // ¸´Î»AD9854
//	Delay_10us (10);
//	AD9854_RST_Clr;
//	SPI_IO_RST_Clr;
//	AD9854_RD_Clr;

//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//¹Ø±Õ±È½ÏÆ÷
//	AD9854_WR_Byte(CLK_Set);					//ÉèÖÃÏµÍ³Ê±ÖÓ±¶Æµ       
//	AD9854_WR_Byte(0x02);							//ÉèÖÃÏµÍ³ÎªÄ£Ê½0£¬ÓÉÍâ²¿¸üÐÂ
//	AD9854_WR_Byte(0x60);      //ÉèÖÃÎª¿Éµ÷½Ú·ù¶È£¬È¡Ïû²åÖµ²¹³¥

//	AD9854_UDCLK_Set;                  //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_SetFSK(ulong Freq1,ulong Freq2)
//º¯Êý¹¦ÄÜ:AD9854µÄFSKÉèÖÃ
//Èë¿Ú²ÎÊý:Freq1   FSKÆµÂÊ1   
//         Freq2   FSKÆµÂÊ2
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_SetFSK(u32 Freq1,u32 Freq2)
//{
//  u8 count=6;
//	u8 i=0,j=0;

//	const u16 Shape=4000;	      //·ù¶ÈÉèÖÃ. Îª12 Bit,È¡Öµ·¶Î§Îª(0~4095)
//	
//	Freq_convert(Freq1);               //ÆµÂÊ×ª»»1
//	
//	for(count=6;count>0;)	          //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }
//	
//	Freq_convert(Freq2);               //ÆµÂÊ×ª»»2

//	for(count=6;count>0;)	          //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//		if(j==0)
//			AD9854_WR_Byte(FREQ2);
//		AD9854_WR_Byte(FreqWord[--count]);
//		j++;
//  }

//	AD9854_WR_Byte(SHAPEI);						  //ÉèÖÃIÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);							//ÉèÖÃQÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));

//	AD9854_UDCLK_Set;                    //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;		
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_InitBPSK(void)
//º¯Êý¹¦ÄÜ:AD9854µÄBPSK³õÊ¼»¯
//Èë¿Ú²ÎÊý:ÎÞ
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_InitBPSK(void)
//{
//	AD9854_SP_Clr;
//	AD9854_WR_Clr;                 // ½«¶Á¡¢Ð´¿ØÖÆ¶Ë¿ÚÉèÎªÎÞÐ§
//	AD9854_UDCLK_Clr;
//	AD9854_RST_Set;                // ¸´Î»AD9854
//	Delay_10us (10);
//	AD9854_RST_Clr;
//	SPI_IO_RST_Clr;
//	AD9854_RD_Clr;

//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//¹Ø±Õ±È½ÏÆ÷
//	AD9854_WR_Byte(CLK_Set);					//ÉèÖÃÏµÍ³Ê±ÖÓ±¶Æµ       
//	AD9854_WR_Byte(0x08);							//ÉèÖÃÏµÍ³ÎªÄ£Ê½0£¬ÓÉÍâ²¿¸üÐÂ
//	AD9854_WR_Byte(0x60);

//	AD9854_UDCLK_Set;                //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_SetBPSK(uint Phase1,uint Phase2)
//º¯Êý¹¦ÄÜ:AD9854µÄBPSKÉèÖÃ
//Èë¿Ú²ÎÊý:Phase1   µ÷ÖÆÏàÎ»1
//         Phase2	µ÷ÖÆÏàÎ»2
//³ö¿Ú²ÎÊý:ÎÞ
//ËµÃ÷£º   ÏàÎ»Îª14Bit£¬È¡Öµ´Ó0~16383£¬¹ÈÓê½¨ÒéÔÚÓÃ±¾º¯ÊýµÄÊ±ºò½«Phase1ÉèÖÃÎª0£¬
//         ½«Phase1ÉèÖÃÎª8192£¬180¡ãÏàÎ»
//====================================================================================
//void AD9854_SetBPSK(u16 Phase1,u16 Phase2)
//{
//	u8 count;
//  u8 i=0;
//	const u32 Freq=60000;
//  const u16 Shape=4000;

//	AD9854_WR_Byte(PHASE1);										//ÉèÖÃÏàÎ»1
//	AD9854_WR_Byte(Phasea>>8);
//	AD9854_WR_Byte((u8)(Phasea&0xff));

//	AD9854_WR_Byte(PHASE2);										//ÉèÖÃÏàÎ»2
//	AD9854_WR_Byte(Phaseb>>8);
//	AD9854_WR_Byte((u8)(Phaseb&0xff));
//	Freq_convert(Freq);                       //ÆµÂÊ×ª»»

//	for(count=6;count>0;)	                         //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }

//	AD9854_WR_Byte(SHAPEI);						  			//ÉèÖÃIÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);										//ÉèÖÃQÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));

//	AD9854_UDCLK_Set;                         //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;	
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_InitOSK(void)
//º¯Êý¹¦ÄÜ:AD9854µÄOSK³õÊ¼»¯
//Èë¿Ú²ÎÊý:ÎÞ
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_InitOSK(void)
//{																				
//	AD9854_SP_Clr;
//	AD9854_WR_Clr;                 // ½«¶Á¡¢Ð´¿ØÖÆ¶Ë¿ÚÉèÎªÎÞÐ§
//	AD9854_UDCLK_Clr;
//	AD9854_RST_Set;                // ¸´Î»AD9854
//	Delay_10us (10);
//	AD9854_RST_Clr;
//	SPI_IO_RST_Clr;
//	AD9854_RD_Clr;

//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);						//¹Ø±Õ±È½ÏÆ÷
//	AD9854_WR_Byte(CLK_Set);				//ÉèÖÃÏµÍ³Ê±ÖÓ±¶Æµ       
//	AD9854_WR_Byte(0x00);						//ÉèÖÃÏµÍ³ÎªÄ£Ê½0£¬ÓÉÍâ²¿¸üÐÂ
//	AD9854_WR_Byte(0x70);	          //ÉèÖÃÎª¿Éµ÷½Ú·ù¶È£¬È¡Ïû²åÖµ²¹³¥,Í¨¶ÏÕûÐÎÄÚ²¿¿ØÖÆ

//	AD9854_UDCLK_Set;               //¸üÐÂAD9854Êä³ö
//	AD9854_UDCLK_Clr;
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_SetOSK(uchar RateShape)
//º¯Êý¹¦ÄÜ:AD9854µÄOSKÉèÖÃ
//Èë¿Ú²ÎÊý: RateShape    OSKÐ±ÂÊ,È¡ÖµÎª4~255£¬Ð¡ÓÚ4ÔòÎÞÐ§
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_SetOSK(u8 RateShape)
//{
//	u8 count;
//  u8 i=0;
//	const u32 Freq=60000;			  //ÉèÖÃÔØÆµ
//  const u16  Shape=4000;			//·ù¶ÈÉèÖÃ. Îª12 Bit,È¡Öµ·¶Î§Îª(0~4095)

//	Freq_convert(Freq);                       //ÆµÂÊ×ª»»

//	for(count=6;count>0;)	                    //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }

//	AD9854_WR_Byte(SHAPEI);						  		  //ÉèÖÃIÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);										//ÉèÖÃQÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));  	 
//       
//	AD9854_WR_Byte(RAMPO);										//ÉèÖÃOSKÐ±ÂÊ
//	AD9854_WR_Byte(RateShape);			          //ÉèÖÃOSKÐ±ÂÊ

//	AD9854_UDCLK_Set;                         //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;	
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_InitAM(void)
//º¯Êý¹¦ÄÜ:AD9854µÄAM³õÊ¼»¯
//Èë¿Ú²ÎÊý:ÎÞ
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_InitAM(void)
//{
//	u8 count;
//  u8 i=0;
//	const u32 Freq=60000;			 //ÉèÖÃÔØÆµ

//	AD9854_SP_Clr;
//	AD9854_WR_Clr;                 // ½«¶Á¡¢Ð´¿ØÖÆ¶Ë¿ÚÉèÎªÎÞÐ§
//	AD9854_UDCLK_Clr;
//	AD9854_RST_Set;                // ¸´Î»AD9854
//	Delay_10us (10);
//	AD9854_RST_Clr;
//	SPI_IO_RST_Clr;
//	AD9854_RD_Clr;

//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//¹Ø±Õ±È½ÏÆ÷
//	AD9854_WR_Byte(CLK_Set);					//ÉèÖÃÏµÍ³Ê±ÖÓ±¶Æµ       
//	AD9854_WR_Byte(0x00);							//ÉèÖÃÏµÍ³ÎªÄ£Ê½0£¬ÓÉÍâ²¿¸üÐÂ
//	AD9854_WR_Byte(0x60);						  //ÉèÖÃÎª¿Éµ÷½Ú·ù¶È£¬È¡Ïû²åÖµ²¹³

//	Freq_convert(Freq);                            //ÆµÂÊ×ª»»

//	for(count=6;count>0;)	                         //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }

//	AD9854_UDCLK_Set;                             //¸üÐÂAD9854Êä³ö
//	AD9854_UDCLK_Clr;
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_SetAM(uchar Shape)
//º¯Êý¹¦ÄÜ:AD9854µÄAMÉèÖÃ
//Èë¿Ú²ÎÊý:Shape   12Bit·ù¶È,È¡Öµ´Ó0~4095   
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_SetAM(u16 Shape)
//{
//	AD9854_WR_Byte(SHAPEI);						  				//ÉèÖÃIÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);											//ÉèÖÃQÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));

//	AD9854_UDCLK_Set;                                   //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;			
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_InitRFSK(void)
//º¯Êý¹¦ÄÜ:AD9854µÄRFSK³õÊ¼»¯
//Èë¿Ú²ÎÊý:ÎÞ
//³ö¿Ú²ÎÊý:ÎÞ
//====================================================================================
//void AD9854_InitRFSK(void)
//{
//	AD9854_SP_Clr;
//	AD9854_WR_Clr;                 // ½«¶Á¡¢Ð´¿ØÖÆ¶Ë¿ÚÉèÎªÎÞÐ§
//	AD9854_UDCLK_Clr;
//	AD9854_RST_Set;                // ¸´Î»AD9854
//	Delay_10us (10);
//	AD9854_RST_Clr;
//	SPI_IO_RST_Clr;
//	AD9854_RD_Clr;
//	AD9854_FDATA_Clr;

//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//¹Ø±Õ±È½ÏÆ÷
//	AD9854_WR_Byte(CLK_Set);					//ÉèÖÃÏµÍ³Ê±ÖÓ±¶Æµ       
//	AD9854_WR_Byte(0x24);							//ÉèÖÃÏµÍ³ÎªÄ£Ê½0£¬ÓÉÍâ²¿¸üÐÂ
//	AD9854_WR_Byte(0x20);		        //ÉèÖÃÎª¿Éµ÷½Ú·ù¶È£¬È¡Ïû²åÖµ²¹³¥,¹Ø±ÕSincÐ§Ó¦	

//	AD9854_UDCLK_Set;                   //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;
//}

//====================================================================================
//º¯ÊýÃû³Æ:void AD9854_SetRFSK(void)
//º¯Êý¹¦ÄÜ:AD9854µÄRFSKÉèÖÃ
//Èë¿Ú²ÎÊý:Freq_Low          RFSKµÍÆµÂÊ	   48Bit
//         Freq_High         RFSK¸ßÆµÂÊ	   48Bit
//         Freq_Up_Down		 ²½½øÆµÂÊ	   48Bit
//		     FreRate           Ð±ÂÊÊ±ÖÓ¿ØÖÆ  20Bit
//³ö¿Ú²ÎÊý:ÎÞ
//×¢£º     Ã¿Á½¸öÂö³åÖ®¼äµÄÊ±¼äÖÜÆÚÓÃÏÂÊ½±íÊ¾£¨FreRate +1£©*£¨System Clock £©£¬Ò»¸öÂö³å,
//         ÆµÂÊ ÉÏÉý»òÕßÏÂ½µ Ò»¸ö²½½øÆµÂÊ
//====================================================================================
//void AD9854_SetRFSK(u32 Freq_Low,u32 Freq_High,u32 Freq_Up_Down,u32 FreRate)
//{
//	u8 count=6;
//	u8 i=0,j=0,k=0;
//  const u16  Shape=3600;			   //·ù¶ÈÉèÖÃ. Îª12 Bit,È¡Öµ·¶Î§Îª(0~4095)

//	Freq_convert(Freq_Low);                             //ÆµÂÊ1×ª»»

//	for(count=6;count>0;)	                         //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }

//	Freq_convert(Freq_High);                             //ÆµÂÊ2×ª»»

//	for(count=6;count>0;)	                         //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//		if(j==0)
//			AD9854_WR_Byte(FREQ2);
//		AD9854_WR_Byte(FreqWord[--count]);
//		j++;
//  }

//	Freq_convert(Freq_Up_Down);                             //²½½øÆµÂÊ×ª»»

//	for(count=6;count>0;)	                               //Ð´Èë6×Ö½ÚµÄÆµÂÊ¿ØÖÆ×Ö  
//  {
//		if(k==0)
//			AD9854_WR_Byte(DELFQ);
//		AD9854_WR_Byte(FreqWord[--count]);
//		k++;
//  }

//	AD9854_WR_Byte(RAMPF);						  				//ÉèÖÃÐ±ÉýËÙÂÊ
//	AD9854_WR_Byte((u8)((FreRate>>16)&0x0f));
//	AD9854_WR_Byte((u8)(FreRate>>8));
//	AD9854_WR_Byte((u8)FreRate);				    

//	AD9854_WR_Byte(SHAPEI);						  				//ÉèÖÃIÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);											//ÉèÖÃQÍ¨µÀ·ù¶È
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((u8)(Shape&0xff));

//	AD9854_UDCLK_Set;                                //¸üÐÂAD9854Êä³ö
//  AD9854_UDCLK_Clr;	
//}
