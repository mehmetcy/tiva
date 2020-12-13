#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "inc/tm4c123gh6pm.h"

void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

int
main(void)
{
    //clock set
   SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   //configure PWM clock
   SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

   // Enabling periphs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins

    //configure pwms
    GPIOPinConfigure(0x00050405);
    GPIOPinConfigure(0x00050805);
    GPIOPinConfigure(0x00050C05);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //Configure PWM Options
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    //Set the Period
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 320);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 320);

    //Set PWM duty-50% ( /2)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,100);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,100);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,100);

    // Enable the PWM generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Turn on the Output pins
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);

    //Fade
    bool fadeUp = true;
    unsigned long increment = 10;
    unsigned long pwmNow = 100;
    while(1)
    {
        delayMS(20);
        if (fadeUp) {
            pwmNow += increment;
            if (pwmNow >= 320) { fadeUp = false; }
        }
        else {
            pwmNow -= increment;
            if (pwmNow <= 10) { fadeUp = true; }
        }
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,pwmNow);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,pwmNow);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,pwmNow);
    }

}
