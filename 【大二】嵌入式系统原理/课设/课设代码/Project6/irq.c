#include "stdio.h"

#define GPKCON0 (*((volatile unsigned long *)0x7F008800))
#define GPKDATA (*((volatile unsigned long *)0x7F008808))
#define GPNCON (*((volatile unsigned long *)0x7F008830)) 
#define GPNDAT (*((volatile unsigned long *)0x7F008834)) 

#define GPFCON1 (*(volatile unsigned int *)0x7F0080A0)
#define GPFDAT1 (*(volatile unsigned int *)0x7F0080A4)

#define EINT0CON0 (*((volatile unsigned long *)0x7F008900)) 
#define EINT0MASK (*((volatile unsigned long *)0x7F008920)) 
#define EINT0PEND (*((volatile unsigned long *)0x7F008924)) 
#define PRIORITY (*((volatile unsigned long *)0x7F008280))  
#define SERVICE (*((volatile unsigned long *)0x7F008284))   
#define SERVICEPEND (*((volatile unsigned long *)0x7F008288)) 
#define VIC0IRQSTATUS (*((volatile unsigned long *)0x71200000)) 
#define VIC0FIQSTATUS (*((volatile unsigned long *)0x71200004)) 
#define VIC0RAWINTR (*((volatile unsigned long *)0x71200008)) 
#define VIC0INTSELECT (*((volatile unsigned long *)0x7120000c)) 
#define VIC0INTENABLE (*((volatile unsigned long *)0x71200010)) 
#define VIC0INTENCLEAR (*((volatile unsigned long *)0x71200014))
#define VIC0PROTECTION (*((volatile unsigned long *)0x71200020))
#define VIC0SWPRIORITYMASK (*((volatile unsigned long *)0x71200024))
#define VIC0PRIORITYDAISY (*((volatile unsigned long *)0x71200028)) 
#define VIC0ADDRESS (*((volatile unsigned long *)0x71200f00))


// 定时器基地址和相关寄存器定义，用于配置定时器0。
#define PWMTIMER_BASE (0x7F006000)
#define TCFG0 (*((volatile unsigned long *)(PWMTIMER_BASE + 0x00))) 
#define TCFG1 (*((volatile unsigned long *)(PWMTIMER_BASE + 0x04))) 
#define TCON (*((volatile unsigned long *)(PWMTIMER_BASE + 0x08))) 
#define TCNTB0 (*((volatile unsigned long *)(PWMTIMER_BASE + 0x0C)))
#define TCMPB0 (*((volatile unsigned long *)(PWMTIMER_BASE + 0x10)))
#define TCNTO0 (*((volatile unsigned long *)(PWMTIMER_BASE + 0x14)))
#define TINT_CSTAT (*((volatile unsigned long *)(PWMTIMER_BASE + 0x44)))

typedef void (isr)(void);
extern void asm_timer_irq();
extern void asm_k1_irq();

volatile int INDIVIDUAL, TEN; // 十位数，个位数
volatile int mode;            // 模式：0空闲，1输入完成闪烁，2走马灯，3K3整数倍，4K3非整数倍
volatile int current_week = 3; // 假设当前是星期三
volatile int input_timeout = 0; // 输入超时标志
volatile int input_complete = 0; // 输入完成标志
volatile int k4_pressed = 0;    // K4按键状态
volatile int led_pos = 0;       // LED位置（用于走马灯）
volatile int direction = 1;     // 走马灯方向：1向右，0向左
volatile int flash_count = 0;   // 闪烁计数
volatile int buzzer_count = 0;  // 蜂鸣器鸣叫计数

void dely_ms(volatile int time) {
	volatile int i, j;
	for (i = 0; i < time; i++) {
		for (j = 0; j < 0x7ff; j++) {}
	}
}

void timer_init(unsigned long utimer, unsigned long uprescaler, unsigned long udivider, unsigned long utcntb, unsigned long utcmpb) {
	unsigned long temp0;
	// 设置预分频系数为66
	temp0 = TCFG0;
	temp0 = (temp0 & (~(0xff00ff))) | (uprescaler << 0);
	TCFG0 = temp0;
	// 设置16分频
	temp0 = TCFG1;
	temp0 = (temp0 & (~(0xf << 4 * utimer)) & (~(1 << 20))) | (udivider << 4 * utimer);
	TCFG1 = temp0;
	// 设置计数器初值
	TCNTB0 = utcntb;
	// 设置比较值
	TCMPB0 = utcmpb;
	// 手动更新
	TCON |= 1 << 1;
	TCON &= ~(1 << 1);
	// 自动加载并启动定时器0
	TCON |= (1 << 0) | (1 << 3);
	// 使能定时器0中断
	temp0 = TINT_CSTAT;
	temp0 = (temp0 & (~(1 << utimer))) | (1 << utimer);
	TINT_CSTAT = temp0;
}

void irq_init(void) {
	// 配置GPN0-GPN3为外部中断功能
	GPNCON &= ~(0xff);
	GPNCON |= 0xaa; // 10101010：设置为EINT0-EINT3
	// 设置中断触发方式为下降沿
	EINT0CON0 &= ~(0xff);
	EINT0CON0 |= 0x33;
	// 解除中断屏蔽
	EINT0MASK &= ~(0x0f);
	// 设置为IRQ模式
	VIC0INTSELECT = 0;
	// 使能EINT0-EINT3中断
	VIC0INTENABLE |= (0x3); // bit0: eint0~3, bit1: eint4~11
	// 设置EINT0的中断服务例程
	isr** isr_array_0 = (isr**)(0x71200100);
	isr_array_0[0] = (isr*)asm_k1_irq;
	// 使能定时器0中断
	VIC0INTENABLE |= (1 << 23);
	// 设置定时器0的中断服务例程
	isr** isr_array_1 = (isr**)(0x7120015C);
	isr_array_1[0] = (isr*)asm_timer_irq;
	// 配置GPK4-GPK7为输出口（LED）
	GPKCON0 = 0x11110000;
	// 初始熄灭四个LED灯
	GPKDATA = 0xf0;


	// 配置GPF14为输出口（蜂鸣器）
	GPFCON1 |= 1 << 28;
	// 初始关闭蜂鸣器
	GPFDAT1 &= ~(1 << 29);
}

// 处理外部中断（按键中断）
void do_irq_1(void) {
	// 按下K1（EINT0）
	if (EINT0PEND & (1 << 0)) {
		dely_ms(20); // 软件消抖（20ms）
		if (!(GPNDAT & (1 << 0))) { // 确认按键仍按下
			INDIVIDUAL++; // 个位数加1
			GPKDATA = 0b11101111; // LED1亮（提示）
			dely_ms(500);
			GPKDATA = 0xff; // 熄灭所有LED
			input_timeout = 0; // 重置超时标志
			input_complete = 0; // 重置输入完成标志
			mode = 0; // 回到空闲模式
			timer_init(0, 65, 4, 62500 * 2, 0); // 启动2秒定时器
		}
	}
	// 按下K2（EINT1）
	else if (EINT0PEND & (1 << 1)) {
		dely_ms(20);
		if (!(GPNDAT & (1 << 1))) {
			TEN++; // 十位数加1
			GPKDATA = 0b11011111; // LED2亮（提示）
			dely_ms(500);
			GPKDATA = 0xff;
			input_timeout = 0;
			input_complete = 0;
			mode = 0;
			timer_init(0, 65, 4, 62500 * 2, 0); // 启动2秒定时器
		}
	}
	// 按下K3（EINT2）
	else if (EINT0PEND & (1 << 2)) {
		dely_ms(20);
		if (!(GPNDAT & (1 << 2))) {
			int num = TEN * 10 + INDIVIDUAL; // 计算输入的十进制数
			int temp = num;
			while (temp >= current_week) {
				temp -= current_week; // 判断是否为当前星期的整数倍
			}
			if (temp == 0) {
				mode = 3; // 是整数倍，进入模式3
				flash_count = 0; // 重置闪烁计数
				buzzer_count = 0; // 重置蜂鸣器计数
				timer_init(0, 65, 4, 62500, 0); // 1秒定时器
			}
			else {
				mode = 4; // 不是整数倍，进入模式4
				flash_count = 0;
				buzzer_count = 0;
				timer_init(0, 65, 4, 62500 * 2, 0); // 2秒定时器
			}
		}
	}
	// 按下K4（EINT3）
	else if (EINT0PEND & (1 << 3)) {
		dely_ms(500); // 检测短按
		if (!(GPNDAT & (1 << 3))) { // 按键仍按下
			dely_ms(1500); // 继续检测长按（总计2秒）
			if (!(GPNDAT & (1 << 3))) { // 长按
				mode = 0; // 回到空闲模式
				INDIVIDUAL = 0; // 清零个位数
				TEN = 0; // 清零十位数
				k4_pressed = 0; // 重置K4状态
				GPKDATA = 0xff; // 熄灭所有LED
				GPFDAT1 &= ~(1 << 6); // 关闭蜂鸣器
			}
		}
		else { // 短按
			mode = 2; // 进入走马灯模式
			k4_pressed = 1; // 设置K4按下标志
			led_pos = 3; // 从LED4开始
			direction = 0; // 初始方向向左
			timer_init(0, 65, 4, 62500 / 2, 0); // 0.5秒定时器
		}
	}
	// 清除中断挂起
	EINT0PEND = 0x0f;
	VIC0ADDRESS = 0;
}

// 处理定时器中断
void do_irq_2(void) {
	int num = TEN * 10 + INDIVIDUAL; // 计算输入的十进制数
	// 处理输入超时（2秒无输入）
	if (!input_complete && input_timeout) {
		input_complete = 1; // 设置输入完成
		mode = 1; // 进入输入完成闪烁模式
		flash_count = 0; // 重置闪烁计数
		timer_init(0, 65, 4, 62500, 0); // 1秒定时器
	}
	// 模式1：输入完成闪烁
	else if (mode == 1) {
		if (flash_count < INDIVIDUAL) {
			GPKDATA = 0b11101111; // LED1亮
			dely_ms(1000);
			GPKDATA = 0xff; // LED1灭
			flash_count++; // 增加闪烁计数
		}
		else if (flash_count < INDIVIDUAL + TEN) {
			GPKDATA = 0b11011111; // LED2亮
			dely_ms(1000);
			GPKDATA = 0xff; // LED2灭
			flash_count++;
		}
		else {
			mode = 0; // 回到空闲模式
			flash_count = 0;
		}
	}
	// 模式2：双向走马灯
	else if (mode == 2) {
		GPKDATA = 0xff; // 熄灭所有LED
		GPKDATA &= ~(1 << (led_pos + 4)); // 点亮当前LED
		if (direction == 1) { // 向右
			if (led_pos == 3) { // 到达LED4
				led_pos = 2; // 移到LED3
				direction = 0; // 改变方向为向左
			}
			else {
				led_pos++; // 向右移动
			}
		}
		else { // 向左
			if (led_pos == 0) { // 到达LED1
				led_pos = 1; // 移到LED2
				direction = 1; // 改变方向为向右
			}
			else {
				led_pos--; // 向左移动
			}
		}
	}
	// 模式3：K3按下且是整数倍
	else if (mode == 3) {
		if (flash_count < num) {
			GPKDATA = 0b00001111; // LED1-LED4亮
			dely_ms(1000);
			GPKDATA = 0xff; // LED1-LED4灭
			dely_ms(1000);
			flash_count++;
		}
		else if (buzzer_count < num) {
			beep_time(2000);
			dely_ms(1000);
			buzzer_count++;
		}
		else {
			GPKDATA = 0b01101111; // LED1和LED4常亮
			mode = 0; // 回到空闲模式
			flash_count = 0;
			buzzer_count = 0;
		}
	}
	// 模式4：K3按下但不是整数倍
	else if (mode == 4) {
		if (flash_count < num) {
			GPKDATA = 0b00001111; // LED1-LED4亮
			dely_ms(2000);
			GPKDATA = 0xff; // LED1-LED4灭
			dely_ms(2000);
			flash_count++;
		}
		else if (buzzer_count < num) {

			beep_time(2000);

			dely_ms(2000);
			buzzer_count++;
		}
		else {
			GPKDATA = 0b10011111; // LED2和LED3常亮
			mode = 0; // 回到空闲模式
			flash_count = 0;
			buzzer_count = 0;
		}
	}
	// 设置输入超时标志
	if (!input_complete) {
		input_timeout = 1;
	}
	// 清除定时器中断
	unsigned long uTmp = TINT_CSTAT;
	TINT_CSTAT = uTmp;
	VIC0ADDRESS = 0x0;
}

// 蜂鸣器相关三个函数
void beep_on() {
	GPFDAT1 |= (1 << 14);
}
void beep_off() {
	GPFDAT1 &= ~(1 << 14);
}
void beep_time(int time) {
	beep_on();
	dely_ms(time);
	beep_off();
}