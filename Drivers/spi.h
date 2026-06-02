#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

/* SPI Configuration */
#define SPI_SPEED_DIV   2           /* SPI clock = APB2 / 2 = 36MHz */

void SPI_Init(void);
void SPI_Write(uint8_t data);
uint8_t SPI_Read(void);
void SPI_WriteBytes(uint8_t* buf, uint16_t len);

#endif
