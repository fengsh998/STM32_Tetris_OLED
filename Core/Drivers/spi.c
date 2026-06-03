#include "stm32f1xx.h"
#include "spi.h"

/**
 * SPI_Init - Initialize SPI1 for OLED communication
 * Mode: 4-wire SPI, Master
 * Speed: APB2 / 2 = 36MHz (with prescaler)
 * Clock polarity: CPOL = 0 (SCK idles low)
 * Clock phase: CPHA = 0 (data sampled on rising edge)
 * 走的是SPI硬件协议栈
 */
void SPI_Init(void)
{
    uint32_t temp;
    
    /* Enable SPI1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    
    /* Disable SPI before configuration */
    SPI1->CR1 = 0;
    
    /* Configure SPI1 Control Register 1 (CR1) */
    temp = 0;
    temp |= (0 << 0);               /* CPHA = 0 (data sampled on rising edge) */
    temp |= (0 << 1);               /* CPOL = 0 (SCK idles low) */
    temp |= (1 << 2);               /* MSTR = 1 (master mode) */
    temp |= (0x1 << 3);             /* BR = 0x1 (prescaler /4, APB2=72MHz -> 18MHz) */
    temp |= (1 << 6);               /* SPE = 1 (enable SPI) */
    temp |= (0 << 7);               /* LSBFIRST = 0 (MSB first) */
    temp |= (1 << 8);               /* SSI = 1 (internal slave select) */
    temp |= (1 << 9);               /* SSM = 1 (software slave select) */
    temp |= (0 << 10);              /* RXONLY = 0 (full-duplex) */
    temp |= (0 << 11);              /* DFF = 0 (8-bit data) */

    SPI1->CR1 = temp;
    
    /* Configure SPI1 Control Register 2 (CR2) */
    SPI1->CR2 = 0;
    SPI1->CR2 |= (1 << 2);          /* SSOE = 1 (SS output enable) */
}

/**
 * SPI_Write - Write single byte to SPI
 */
void SPI_Write(uint8_t data)
{
    /* Wait until transmit buffer is empty */
    while (!(SPI1->SR & SPI_SR_TXE));
    
    /* Write data to transmit buffer */
    SPI1->DR = data;
    
    /* Wait until transmit is complete */
    while (SPI1->SR & SPI_SR_BSY);
    
    /* Clear receive buffer */
    (void)SPI1->DR;
}

/**
 * SPI_Read - Read single byte from SPI
 */
uint8_t SPI_Read(void)
{
    /* Wait until transmit buffer is empty */
    while (!(SPI1->SR & SPI_SR_TXE));
    
    /* Send dummy byte */
    SPI1->DR = 0xFF;
    
    /* Wait until receive buffer has data */
    while (!(SPI1->SR & SPI_SR_RXNE));
    
    /* Read and return received data */
    return SPI1->DR;
}

/**
 * SPI_WriteBytes - Write multiple bytes to SPI
 */
void SPI_WriteBytes(uint8_t* buf, uint16_t len)
{
    uint16_t i;
    
    for (i = 0; i < len; i++) {
        SPI_Write(buf[i]);
    }
}
