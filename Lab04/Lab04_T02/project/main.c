

/**
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

int main(void)
{
    uint32_t ui32Period;

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R   = 1;
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
    ui32Period = ((SysCtlClockGet() / 10)*43) /100;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
    ui32Period = SysCtlClockGet();
    TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();

    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1)
    {
        uint32_t value = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
        if(!value)
        {
            while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0));
            TimerDisable(TIMER0_BASE, TIMER_A);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x04);
            ui32Period = SysCtlClockGet();
            TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);
            TimerEnable(TIMER1_BASE, TIMER_A);
        }

    }
}

void Timer1IntHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    TimerEnable(TIMER0_BASE, TIMER_A);
}


void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Read the current state of the GPIO pin and
    // write back the opposite state
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
        uint32_t ui32Period = ((SysCtlClockGet() / 10)*57) /100;
        TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
        uint32_t ui32Period = ((SysCtlClockGet() / 10)*43) /100;
        TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
    }
}
