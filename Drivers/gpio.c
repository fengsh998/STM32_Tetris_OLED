#include "stm32f10x.h"
#include "gpio.h"

/**
 * GPIO_Init - Initialize GPIO pins for OLED SPI and buttons
 * PA2  = CS   (output, push-pull)
 * PA3  = DC   (output, push-pull)
 * PA4  = RES  (output, push-pull)
 * PA5  = SCL  (output, alternate function)
 * PA7  = SDA  (output, alternate function)
 * PB0-PB3 = Buttons (input, pull-up)
 */
void GPIO_Init(void)
{
    uint32_t temp;
    
    /* Enable GPIOA and GPIOB clocks */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
    
    /* --- Configure PA2 (CS) as output push-pull --- */
    temp = GPIOA->CRL;
    temp &= ~(0xF << (2*4));        /* Clear PA2 */
    temp |= (0x3 << (2*4));         /* General purpose output push-pull, max speed 50MHz */
    GPIOA->CRL = temp;
    GPIO_SetPin(GPIOA, 2);          /* Set CS high initially */
    
    /* --- Configure PA3 (DC) as output push-pull --- */
    temp = GPIOA->CRL;
    temp &= ~(0xF << (3*4));        /* Clear PA3 */
    temp |= (0x3 << (3*4));         /* General purpose output push-pull, max speed 50MHz */
    GPIOA->CRL = temp;
    GPIO_ResetPin(GPIOA, 3);        /* Set DC low initially */
    
    /* --- Configure PA4 (RES) as output push-pull --- */
    temp = GPIOA->CRL;
    temp &= ~(0xF << (4*4));        /* Clear PA4 */
    temp |= (0x3 << (4*4));         /* General purpose output push-pull, max speed 50MHz */
    GPIOA->CRL = temp;
    GPIO_SetPin(GPIOA, 4);          /* Set RES high initially */
    
    /* --- Configure PA5 (SCL) as alternate function output push-pull --- */
    temp = GPIOA->CRL;
    temp &= ~(0xF << (5*4));        /* Clear PA5 */
    temp |= (0xB << (5*4));         /* Alternate function output push-pull, max speed 50MHz */
    GPIOA->CRL = temp;
    
    /* --- Configure PA7 (SDA/MOSI) as alternate function output push-pull --- */
    temp = GPIOA->CRL;
    temp &= ~(0xF << (7*4));        /* Clear PA7 */
    temp |= (0xB << (7*4));         /* Alternate function output push-pull, max speed 50MHz */
    GPIOA->CRL = temp;
    
    /* --- Configure PB0-PB3 (Buttons) as input pull-up --- */
    /* PB0: Left button */
    temp = GPIOB->CRL;
    temp &= ~(0xF << (0*4));        /* Clear PB0 */
    temp |= (0x8 << (0*4));         /* Input floating */
    GPIOB->CRL = temp;
    
    /* PB1: Right button */
    temp = GPIOB->CRL;
    temp &= ~(0xF << (1*4));        /* Clear PB1 */
    temp |= (0x8 << (1*4));         /* Input floating */
    GPIOB->CRL = temp;
    
    /* PB2: Down button */
    temp = GPIOB->CRL;
    temp &= ~(0xF << (2*4));        /* Clear PB2 */
    temp |= (0x8 << (2*4));         /* Input floating */
    GPIOB->CRL = temp;
    
    /* PB3: Rotate button */
    temp = GPIOB->CRL;
    temp &= ~(0xF << (3*4));        /* Clear PB3 */
    temp |= (0x8 << (3*4));         /* Input floating */
    GPIOB->CRL = temp;
    
    /* Enable pull-up for buttons */
    GPIOB->ODR |= (0xF << 0);       /* Pull-up */
}

/**
 * GPIO_SetPin - Set GPIO pin to 1
 */
void GPIO_SetPin(GPIO_TypeDef* port, uint16_t pin)
{
    port->BSRR = (1 << pin);
}

/**
 * GPIO_ResetPin - Set GPIO pin to 0
 */
void GPIO_ResetPin(GPIO_TypeDef* port, uint16_t pin)
{
    port->BRR = (1 << pin);
}

/**
 * GPIO_ReadPin - Read GPIO pin state
 * Returns: 0 or 1
 */
uint8_t GPIO_ReadPin(GPIO_TypeDef* port, uint16_t pin)
{
    return (port->IDR >> pin) & 1;
}

/**
 * EXTI_Init - Initialize external interrupts for buttons
 */
void EXTI_Init(void)
{
    uint32_t temp;
    
    /* Enable AFIO clock */
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    
    /* Configure EXTI for PB0 (Left button) */
    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI0;
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PB;
    EXTI->IMR |= EXTI_IMR_MR0;      /* Enable interrupt */
    EXTI->FTSR |= EXTI_FTSR_TR0;    /* Falling edge trigger */
    
    /* Configure EXTI for PB1 (Right button) */
    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI1;
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PB;
    EXTI->IMR |= EXTI_IMR_MR1;
    EXTI->FTSR |= EXTI_FTSR_TR1;
    
    /* Configure EXTI for PB2 (Down button) */
    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI2;
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI2_PB;
    EXTI->IMR |= EXTI_IMR_MR2;
    EXTI->FTSR |= EXTI_FTSR_TR2;
    
    /* Configure EXTI for PB3 (Rotate button) */
    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI3;
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI3_PB;
    EXTI->IMR |= EXTI_IMR_MR3;
    EXTI->FTSR |= EXTI_FTSR_TR3;
    
    /* Enable NVIC interrupts */
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_EnableIRQ(EXTI3_IRQn);
    
    /* Set priority */
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_SetPriority(EXTI1_IRQn, 1);
    NVIC_SetPriority(EXTI2_IRQn, 1);
    NVIC_SetPriority(EXTI3_IRQn, 1);
}
