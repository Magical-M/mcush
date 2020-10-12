/* MCUSH designed by Peng Shulin, all rights reserved. */
#include "hal.h"

const uint8_t port_num = 7;
const GPIO_TypeDef * const ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG };


static void _set_mode( int port, int bits, GPIOMode_TypeDef mode )
{
    GPIO_InitTypeDef init;
    int i;
        
    init.GPIO_Speed = GPIO_Speed_50MHz;
    init.GPIO_Mode = mode;
    for( i=0; i<32; i++ )
    {
        if( bits & (1<<i) )
        { 
            init.GPIO_Pin = (uint16_t)(1<<i);
            GPIO_Init((GPIO_TypeDef*)ports[port], &init);
        }
    }
}


int hal_gpio_get_port_num(void)
{
    return port_num;
}


void hal_gpio_init(void)
{
    RCC_APB2PeriphClockCmd( \
        RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | \
        RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | \
        RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | \
        RCC_APB2Periph_GPIOG, ENABLE );
}


void hal_gpio_set_input(int port, int bits, int pull)
{
    _set_mode( port, bits, pull ? GPIO_Mode_IPU : GPIO_Mode_IPD );
}


void hal_gpio_set_output(int port, int bits, int open_drain)
{
    _set_mode( port, bits, open_drain ? GPIO_Mode_Out_OD : GPIO_Mode_Out_PP );
}


void hal_gpio_set(int port, int bits)
{
    GPIO_SetBits((GPIO_TypeDef*)ports[port], (uint16_t)bits);
}


void hal_gpio_clr(int port, int bits)
{
    GPIO_ResetBits((GPIO_TypeDef*)ports[port], (uint16_t)bits);
}


void hal_gpio_toggle(int port, int bits)
{
    int reset, set;
        
    reset = GPIO_ReadOutputData((GPIO_TypeDef*)ports[port]) & bits;
    set = ~reset & bits;
    
    if( set )
        GPIO_SetBits((GPIO_TypeDef*)ports[port], (uint16_t)set);
    if( reset )
        GPIO_ResetBits((GPIO_TypeDef*)ports[port], (uint16_t)reset);
}


int hal_gpio_get(int port, int bits)
{
    return GPIO_ReadInputData((GPIO_TypeDef*)ports[port]) & bits;
}



