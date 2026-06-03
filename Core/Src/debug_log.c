#include "debug_log.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* STM32F103 USART1 寄存器基址 */
#define USART1_BASE     0x40013800
#define RCC_BASE        0x40021000
#define GPIOA_BASE      0x40010800

/* USART1 寄存器 */
#define USART1_SR       *(volatile uint32_t *)(USART1_BASE + 0x00)
#define USART1_DR       *(volatile uint32_t *)(USART1_BASE + 0x04)
#define USART1_BRR      *(volatile uint32_t *)(USART1_BASE + 0x08)
#define USART1_CR1      *(volatile uint32_t *)(USART1_BASE + 0x0C)
#define USART1_CR2      *(volatile uint32_t *)(USART1_BASE + 0x10)
#define USART1_CR3      *(volatile uint32_t *)(USART1_BASE + 0x14)

/* RCC 寄存器 */
#define RCC_APB2ENR     *(volatile uint32_t *)(RCC_BASE + 0x18)

/* GPIOA 寄存器 */
#define GPIOA_CRH       *(volatile uint32_t *)(GPIOA_BASE + 0x04)

/* USART1_SR 位定义 */
#define USART_SR_TXE    (1 << 7)
#define USART_SR_RXNE   (1 << 5)

/* USART1_CR1 位定义 */
#define USART_CR1_UE    (1 << 13)
#define USART_CR1_TE    (1 << 3)
#define USART_CR1_RE    (1 << 2)

/* RCC_APB2ENR 位定义 */
#define RCC_APB2ENR_USART1EN (1 << 14)
#define RCC_APB2ENR_GPIOA_EN (1 << 2)

#define LOG_BUFFER_SIZE 512
static char log_buffer[LOG_BUFFER_SIZE];

/**
 * @brief USART1初始化 - 寄存器方式
 * PA9: TX (复用推挽输出)
 * PA10: RX (浮空输入)
 * 波特率: 115200 bps
 * 数据位: 8
 * 停止们: 1
 * 时钟: 72 MHz
 */
void debug_uart_init(void)
{
    uint32_t temp;

    volatile int delay;

    /* 延迟等待时钟稳定 */
    for (delay = 0; delay < 10000; delay++);

    /* 1. 使能GPIOA和USART1时钟 */
    RCC_APB2ENR |= RCC_APB2ENR_GPIOA_EN;
    RCC_APB2ENR |= RCC_APB2ENR_USART1EN;

    /* 再延迟 */
    for (delay = 0; delay < 10000; delay++);

    /* 2. 配置PA9为TX - 复用推挽输出(50MHz) */
    /* GPIOA_CRH: PA9在bit[6:9] */
    temp = GPIOA_CRH;
    temp &= ~(0x0F << 4);      /* 清除PA9配置位 */
    temp |= (0x0B << 4);       /* 配置为复用推挽输出50MHz (1011) */
    GPIOA_CRH = temp;

    /* 3. 配置PA10为RX - 浮空输入 */
    /* GPIOA_CRH: PA10在bit[8:11] */
    temp = GPIOA_CRH;
    temp &= ~(0x0F << 8);      /* 清除PA10配置位 */
    temp |= (0x04 << 8);       /* 配置为浮空输入 (0100) */
    GPIOA_CRH = temp;

    /* 先禁用 USART1 */
    USART1_CR1 &= ~USART_CR1_UE;
    for (delay = 0; delay < 1000; delay++);
    /* 4. 配置USART1波特率 */
    /* 波特率 = fck / (16 * USARTDIV) */
    /* 115200 = 72MHz / (16 * USARTDIV) */
    /* USARTDIV = 72000000 / (16 * 115200) = 39.0625 */
    /* 整数部分: 39, 小数部分: 0.0625 * 16 = 1 */
    USART1_BRR = (39 << 4) | 1;  /* 或直接写 0x271 */

    /* 5. 配置USART1控制寄存器 */
    /* 8位数据位, 1个停止位, 无奇偶校验 */
    USART1_CR1 = 0;
    USART1_CR1 |= USART_CR1_TE;  /* 使能发送器TX */
    USART1_CR1 |= USART_CR1_RE;  /* 使能接收器RX */

    /* 6. 配置CR2 (停止位) */
    USART1_CR2 = 0;  /* 1个停止位 */

    /* 7. 配置CR3 */
    USART1_CR3 = 0;  /* 无硬件流控 */

    /* 8. 使能USART1 */
    USART1_CR1 |= USART_CR1_UE;

    for (delay = 0; delay < 10000; delay++);
}

/**
 * @brief UART发送单个字节
 */
void debug_putchar(char ch)
{
    /* 等待发送缓冲区为空 */
    while (!(USART1_SR & USART_SR_TXE));

    /* 将数据写入DR寄存器 */
    USART1_DR = (uint32_t)ch & 0xFF;
}

/**
 * @brief UART发送字符串
 */
void debug_puts(const char *str)
{
    while (*str)
    {
        debug_putchar(*str);
        if (*str == '\n')
            debug_putchar('\r');
        str++;
    }
}

/**
 * @brief 格式化打印（如printf）
 */
void debug_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int len = vsnprintf(log_buffer, LOG_BUFFER_SIZE, format, args);
    va_end(args);

    if (len > 0) {
    	debug_puts(log_buffer);
    }
}

/**
 * @brief 带日志级别的打印
 */
void debug_log(uint8_t level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int len = vsnprintf(log_buffer, LOG_BUFFER_SIZE, format, args);
    va_end(args);

    const char *level_str[] = {"[DEBUG]", "[INFO ]", "[WARN ]", "[ERROR]"};
    debug_puts(level_str[level]);
    debug_putchar(' ');

	if (len > 0) {
		debug_puts(log_buffer);
	}

    debug_putchar('\n');
}

/**
 * @brief 十六进制数据打印
 */
void debug_hex_dump(const uint8_t *data, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        if (i % 16 == 0)
            debug_printf("\r\n%04X: ", i);
        debug_printf("%02X ", data[i]);
    }
    debug_printf("\r\n");
}
