/**
 * MC Project:
 * An image which changes depending on which key (W, S, A, or D) the user presses on the computer.
 * When the user presses the W key, an arrow pointing upwards appears on the pendulum.
 *
 * Date: 20220530
 *
 * authors: Paola A. Carreño, Soodeh Mousavi, Parisa O.
 *
 */

#include "inc/tm4c1294ncpdt.h"
#include "int_handler.h"
#include <stdio.h>
#include <stdint.h>
#define STEPS 1000

//....
void softwareDelay (int steps){
    int i;
    for(i = 0; i < steps; i++);
}

//GPIO handler
void ISR_PortLHandler(void)// IRQ number 53, vector number 69
{
    GPIO_PORTL_ICR_R |= 0x0001;  // clear interrupt

    TIMER0_CTL_R &= ~0x0001;  // disable timer
    TIMER0_CTL_R |= 0x0001; // enable Timer 0A

     int i;
         for(i= 0; i<58 ;i++ ){
             TIMER0_CTL_R |= 0x0001;
             while((TIMER0_RIS_R & 0x01)==0);
                 TIMER0_ICR_R |= 0x01;
                 GPIO_PORTM_DATA_R = 0x00;
             if(i==27){
                 GPIO_PORTM_DATA_R = 0x00;
             }else if(i==28) {
                 GPIO_PORTM_DATA_R = 0x20;
             }else if(i==29) {
                 GPIO_PORTM_DATA_R = 0x40;
             }else if(i==30) {
                 GPIO_PORTM_DATA_R = 0xBF;
             }else if(i==31) {
                 GPIO_PORTM_DATA_R = 0x40;
             }else if(i==32) {
                 GPIO_PORTM_DATA_R = 0x20;
             }else if(i==33) {
                 GPIO_PORTM_DATA_R = 0x00;
             }
         }
}

//GPIO pin configuration
void GPIO_port_config(void)
{
    //configure port M
    SYSCTL_RCGCGPIO_R |= (1<<11); // =>0000 1000 0000 0000 Enable and provide a clock to GPIO Port M in Run mode
    while(!(SYSCTL_PRGPIO_R & (1<<11)));// Wait until GPIO Port M Peripheral Ready
    GPIO_PORTM_DEN_R = 0xFF; // => Enable GPIO Port M pins 0 to  7
    GPIO_PORTM_DIR_R = 0xFF; // => Set GPIO Port M pins 0 to 7 to Output

    //configure port L
    SYSCTL_RCGCGPIO_R |= (1<<10); // =>0000 0100 0000 0000 Enable and provide a clock to GPIO Port L in Run mode
    while(!(SYSCTL_PRGPIO_R & (1<<10)));// Wait until GPIO Port L Peripheral Ready
    GPIO_PORTL_DEN_R |= 0x01; // enable pin L(0)
    GPIO_PORTL_DIR_R &= ~0x01 ; // PortL(0) as input

}

//GPIO interrupt configuration
void GPIO_portL_interrupt_config(void)
{
    GPIO_PORTL_IS_R |= 0x01; // sensitive to level-sensitive
    GPIO_PORTL_IEV_R |= 0x01; // mode: high level
    GPIO_PORTL_ICR_R |= 0x01; // clear interrupt in PORTL(0)
    GPIO_PORTL_IM_R |= 0x01; // GPIO interrupt Mask register-> 1: establish an interrupt for portL

    //enable PORTL interrupt (Int#53/Vec#69) in NVIC
    NVIC_EN1_R |= (1<<21); //53 - 32
}

//Timer configuration
void timer0A_config(void)
{
    SYSCTL_RCGCTIMER_R |= 0x01;
    while(!(SYSCTL_PRTIMER_R & 0x01));
    TIMER0_CTL_R &= ~0x0001; // disable Timer 0A
    TIMER0_CFG_R = 0x4; // 16 bit selected
    TIMER0_TAMR_R = 0x01; //One-mode timer : Capture mode enable
    TIMER0_TAILR_R = 16000-1; //(unsigned long) load value 1ms
}

//UART configuration
void U4Rx_config(void)
{
    // Initializing PK(0)
    SYSCTL_RCGCGPIO_R |= (1<<9); // Enable and provide a clock to GPIO Port L in Run mode
    while(!(SYSCTL_PRGPIO_R & (1<<9)));// Wait until GPIO Port L Peripheral Ready
    GPIO_PORTM_DEN_R |= 0x01; // => Enable GPIO PL(0)
    GPIO_PORTM_DIR_R |= 0x00; // => Set GPIO PL(0)to input
    GPIO_PORTM_AFSEL_R |= 0x01;// Set it to alternate function
    GPIO_PORTM_PCTL_R |= 0x01;// PK(O) alternate function is U4Rx

    // Initializing UART4
    SYSCTL_RCGCUART_R |= (1<<4);
    while(!(SYSCTL_PRUART_R & (1<<4));
    UART4_CTL_R &= ~0x01; //Disable for configuration

    // Initializing bitrate and format
    UART4_IBRD_R |= 0;
    UART4_FBRD_R |= 0;
    UART4_LCRH_R |= 0;
    UART4_CTL_R |= 0;

}

//UART interrupt config
void U4Rx_interrupt_config(void)
{

}
//UART handler



void main(void)
{
    GPIO_port_config();
    GPIO_portL_interrupt_config();
    timer0A_config();
    while(1){

    }
}
