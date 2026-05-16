#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
io_bank controls pin configurations
Its starting memory address can be found in the "Memory Map" secction of the DataSheet specifically
as APB registers

IO_BANK0_BASE 0x40028000
*/

#define IO_BANK0_BASE 0x40028000

/*
SIO is a mode of the pin that allows the processor to quickly flip the pins of the board
on and off. (It has a deidcated section in the datasheet)

SIO_BASE 0xd0000000
*/
#define SIO_BASE 0xd0000000

/*
We use "User Bank" in order to demux the pin aka tell it which function we want it to perform(UART, I2C, SPI, SIO)

I will use GPIO22 which is the 29 pin
GPIO22_CTRL Offset: 0x0b4
To set to it to the SIO function we wrtie 0x05 to its control register.
*/

#define GPIO22_CTRL_OFFSET 0x0b4

/*
We have to set if the GPIO pin outputs or inputs data. We want it to output so we need to set
GPIO_OE for that pin to 1

GPIO_OE register is at offset 0x030 and it has 31 bits for the 31 gpio pins so we will set
bit 22 to 1
*/

#define GPIO_OE_OFFSET 0x030

/*
To now toggle the voltage on that pin we need to change the bit for that specific pin in the
GPIO_OUT_XOR register to 1 and it will continously cycle between 0 and 1
GPIO_OUT_XOR is at offset 0x028 and again uses bits for 0 to 31 
*/

#define GPIO_OUT_XOR_OFFSET 0x028

int main(){

    //Set GPIO22 to SIO mode
    *(volatile uint32_t*)(IO_BANK0_BASE + GPIO22_CTRL_OFFSET) = 0x05;
    //Enable output mode on GPIO22
    *(volatile uint32_t*)(SIO_BASE + GPIO_OE_OFFSET) = (1 << 22);

    while(1){
        //Enable a wait so we perceive the change in LED.
        for(volatile int i = 0; i < 1000000; i++){}
        
        //Repeatedly set the current to high or low on GPIO22 using xor (could have also used GPIO_OUT_XOR)
        *(volatile uint32_t*)(SIO_BASE + GPIO_OUT_XOR_OFFSET) = (1 << 22);
    }
}