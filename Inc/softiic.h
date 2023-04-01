#ifndef __SIIC_H
#define __SIIC_H
#include "stdint.h"
#include "stm32f4xx.h"

#define HIGH 1
#define LOW 0

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOI_ODR_Addr    (GPIOI_BASE+12)     
#define GPIOI_IDR_Addr    (GPIOI_BASE+8) 

#define SCL_IOGroup GPIOI
#define SCL_PIN 6
#define SDA_IOGroup GPIOI
#define SDA_PIN 7

#define SDA_IN() 			{SDA_IOGroup->CRL &= 0x0FFFFFFF;SDA_IOGroup->CRL |=(u32)8<<28;}
#define SDA_OUT() 			{SDA_IOGroup->CRL &= 0x0FFFFFFF;SDA_IOGroup->CRL |=(u32)3<<28;}

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入


#define SET_SCL				PIout(SCL_PIN)
#define READ_SCL			PIin(SCL_PIN)
#define SET_SDA				PIout(SDA_PIN)//输出数据
#define READ_SDA			PIin(SDA_PIN)//读数据

extern void sw_i2c_start();


#endif 
