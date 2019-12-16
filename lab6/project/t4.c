//*****************************************************************************
//
// qei.c - Example to demonstrate QEI on Tiva Launchpad
//This setup uses QEI0 P6/PD7,
//You can also use QEI1 PC5/PC6
//
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/qei.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#define PWM_FREQUENCY 1000
volatile int qeiVelocity;

int main(void)
{
    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Enable QEI Peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);

    //Unlock GPIOD7 - Like PF0 its used for NMI -
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

    //Set Pins to be PHA0 and PHB0
    GPIOPinConfigure(GPIO_PD6_PHA0);
    GPIOPinConfigure(GPIO_PD7_PHB0);

    //Set GPIO pins for QEI. PhA0 -> PD6, PhB0 ->PD7.
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    //DISable peripheral and int before configuration
    QEIDisable(QEI0_BASE);
    QEIIntDisable(QEI0_BASE,QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);

    // Configure quadrature encoder, use an arbitrary top limit of 1000
    QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 1920 );

    // Enable the quadrature encoder.
    QEIEnable(QEI0_BASE);
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, SysCtlClockGet());
    QEIVelocityEnable(QEI0_BASE);
    //Set position to a middle value so we can see if things are working
    //QEIPositionSet(QEI0_BASE, 500);
    uint32_t ui32ADC0Value[4];
        volatile uint32_t ui32Load;
        volatile uint32_t ui32PWMClock;
        volatile uint32_t ui32MotorPWMDutyCycle;


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
            qeiVelocity = QEIVelocityGet(QEI0_BASE);
            SysCtlDelay (1000);
        }

}
