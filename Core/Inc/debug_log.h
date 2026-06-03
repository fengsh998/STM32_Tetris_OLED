#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#include <stdint.h>

/* 日志级别定义 */
#define LOG_DEBUG   0
#define LOG_INFO    1
#define LOG_WARN    2
#define LOG_ERROR   3

/* 函数声明 */
void debug_uart_init(void);
void debug_putchar(char ch);
void debug_puts(const char *str);
void debug_printf(const char *format, ...);
void debug_log(uint8_t level, const char *format, ...);
void debug_hex_dump(const uint8_t *data, uint32_t len);

/* 便捷宏定义 */
#define LOG_D(fmt, ...) debug_log(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...) debug_log(LOG_INFO, fmt, ##__VA_ARGS__)
#define LOG_W(fmt, ...) debug_log(LOG_WARN, fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) debug_log(LOG_ERROR, fmt, ##__VA_ARGS__)

#endif
