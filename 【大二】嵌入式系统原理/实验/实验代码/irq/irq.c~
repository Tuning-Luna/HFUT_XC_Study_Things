#include "stdio.h"
#define GPKCON0              (*((volatile unsigned long *)0x7F008800))
#define GPKDATA              (*((volatile unsigned long *)0x7F008808))
#define GPNCON               (*((volatile unsigned long *)0x7F008830))
#define GPNDAT               (*((volatile unsigned long *)0x7F008834))
#define EINT0CON0            (*((volatile unsigned long *)0x7F008900))
#define EINT0MASK            (*((volatile unsigned long *)0x7F008920))
#define EINT0PEND            (*((volatile unsigned long *)0x7F008924))
#define PRIORITY             (*((volatile unsigned long *)0x7F008280))
#define SERVICE              (*((volatile unsigned long *)0x7F008284))
#define SERVICEPEND          (*((volatile unsigned long *)0x7F008288))
#define VIC0IRQSTATUS        (*((volatile unsigned long *)0x71200000))
#define VIC0FIQSTATUS        (*((volatile unsigned long *)0x71200004))
#define VIC0RAWINTR          (*((volatile unsigned long *)0x71200008))
#define VIC0INTSELECT        (*((volatile unsigned long *)0x7120000c))
#define VIC0INTENABLE        (*((volatile unsigned long *)0x71200010))
#define VIC0INTENCLEAR       (*((volatile unsigned long *)0x71200014))
#define VIC0PROTECTION       (*((volatile unsigned long *)0x71200020))
#define VIC0SWPRIORITYMASK   (*((volatile unsigned long *)0x71200024))
#define VIC0PRIORITYDAISY    (*((volatile unsigned long *)0x71200028))

#define VIC0ADDRESS          (*((volatile unsigned long *)0x71200f00))

typedef void (isr) (void);
extern void asm_k1_irq();

void irq_init(void)
{
    /* 配置GPN0~5为中断功能 */
    GPNCON &= ~(0xff);
    GPNCON |= 0xaa;

    /* 设置中断触发方式为下降沿触发 */
    EINT0CON0 &= ~(0xff);
    EINT0CON0 |= 0x33;

    /* 禁止屏蔽中断 */
    EINT0MASK &= ~(0x0f);

    // Select INT_EINT0 mode as irq  
    VIC0INTSELECT = 0;

    /* 在中断控制器中使能这些中断 */
    VIC0INTENABLE |= (0x3); /* bit0: eint0~3, bit1: eint4~11 */ 

    isr** isr_array = (isr**)(0x71200100);
    isr_array[0] = (isr*)asm_k1_irq;

    /* 将GPK4-GPK7配置为输出口 */
    GPKCON0 = 0x11110000;

    /* 熄灭四个LED灯 */
    GPKDATA = 0xf0;
}

void delay(void) {
    volatile int i, j;
    for(i = 0; i < 1000; i++) {
        for(j = 0; j < 1000; j++);
    }
}

void do_irq(void)
{
    int i = 0;
     GPKDATA = 0xf0;
	delay();
    /* 判别是哪个中断 */
    if (EINT0PEND & (1<<0)) // K1: GPN0
    {	

        // LED1到LED4依次被点亮
        for(i = 0; i < 4; i++) {
            GPKDATA &= ~(1 << (i + 4));
            delay();
        }
	

    }
    else if (EINT0PEND & (1<<1)) // K2: GPN1
    {

        // LED4到LED1依次被点亮
        for(i = 0; i < 4; i++) {
            GPKDATA &= ~(1 << (7 - i));
            delay();
        }

    }
    else if (EINT0PEND & (1<<2)) // K3: GPN2
    {

        // LED1到LED4依次被点亮，且每个时刻只有一个LED亮
        for(i = 0; i < 4; i++) {
            GPKDATA = 0xf0; // 先熄灭所有LED
            GPKDATA &= ~(1 << (i + 4));
            delay();
        }

    }
    else if (EINT0PEND & (1<<3)) // K4: GPN3
    {

        // LED4到LED1依次被点亮，且每个时刻只有一个LED亮
        for(i = 0; i < 4; i++) {
            GPKDATA = 0xf0; // 先熄灭所有LED
            GPKDATA &= ~(1 << (7 - i));
            delay();
        }

    }
    
    /* 清中断 */
    EINT0PEND = 0x3f;
    VIC0ADDRESS = 0;
}


