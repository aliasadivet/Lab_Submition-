#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"

#define PWM_FREQUENCY 1000

int main(void)
{
    uint32_t ui32ADC0Value[4];
    volatile uint32_t ui32Load;
    volatile uint32_t ui32PWMClock;
    volatile uint32_t ui32MotorPWMDutyCycle;

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 64);
    ROM_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    ROM_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
    ROM_ADCSequenceEnable(ADC0_BASE, 1);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3);
    ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3, 4);
    ROM_GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
    ROM_GPIOPinConfigure(GPIO_PE4_M1PWM2);

    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, ui32Load);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 0 * ui32Load / 1000);
    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_1);

    while(1)
    {
        ROM_ADCIntClear(ADC0_BASE, 1);
        ROM_ADCProcessorTrigger(ADC0_BASE, 1);

        while(!ROM_ADCIntStatus(ADC0_BASE, 1, false)) { }

        ROM_ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
        ui32MotorPWMDutyCycle = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;

        ui32MotorPWMDutyCycle = (uint32_t)(((float)ui32MotorPWMDutyCycle / 4096.0) *     ui32Load);

        ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2,  ui32MotorPWMDutyCycle +1);
    }
}
