/*
 * SwitchTypeMain.c
 * Developed in MicrochipStudio and for the ATmega328P Xplained Mini
 *	
 *	PD2 external Interrupt INT0 =glassScaleZ	
 *	
 *	PD3 external Interrupt INT0 =glassScaleA
 *	
 *	PC0 = stepPin (stepper motor)
 *	
 *	PC1 = directionPin (stepper motor)
 *
 *	PC2 = enablePin (stepper motor)
 *
 *	PB5 = Sensor mechanic
  *
  *	PB4 = Sensor Photoelectric barrier

 *  Created: 10/11/2022 04:41:29
 *  Author: Winter/Venier
 */

#include "avrhal/usart-string.h"
#include "avrhal/usart.h"
#include "utils/bit.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <util/atomic.h>
#include <util/delay.h>

const uint8_t stepPin = PC0;
const uint8_t directionPin = PC1;
const uint8_t enablePin = PC2;
const uint8_t zRef = PD2;
static volatile uint64_t counter = 0;
static volatile uint64_t MMcounter = 0;
bool isr;
bool home = true;

static inline void Toggle(bool Home) //controls the direction of the stepper motor
{
    if (!Home) {
        BIT_CLR(PORTC, directionPin);
    }
    if (Home) {
        BIT_SET(PORTC, directionPin);
    }
    _delay_ms(2);
    BIT_TOGGLE(PORTC, stepPin);
}
static inline void setupStepper()
{
    BIT_SET(DDRC, stepPin);
    BIT_SET(DDRC, enablePin);
    BIT_SET(DDRC, directionPin);
    BIT_SET(PORTC, enablePin);
    BIT_SET(PORTC, directionPin);
}

static inline void setupExtInterrupt()
{

    //Pin PD2 (INT0)
    BIT_SET(EICRA, ISC00); // enable rising edge
    BIT_SET(EICRA, ISC01);
    BIT_SET(EIMSK, INT0); //enable Pin PD2 ext interrupt

    //Pin PD3 (INT0)
    BIT_SET(EICRA, ISC10); // enable rising edge
    BIT_SET(EICRA, ISC11);
    BIT_SET(EIMSK, INT1); //enable Pin PD3 ext interrupt
}

ISR(INT0_vect) //service Routine for reference Point
{
    isr = true;
    home = false;
    counter = 0;
    MMcounter = 0;
}

ISR(INT1_vect) //service Routine for length measuring
{

    counter++;
    if (counter == 200) { //equals to 1mm
        MMcounter++;
        counter = 0;
    }
}

int main(void)
{

    uint8_t count = 0;
    uint8_t i = 0;
    usartSetup(USART_B9600);
    setupStepper();
    BIT_CLR(DDRB, PB4); //PB4 Input
    //BIT_SET(PORTB, PB4); //Activate PullUp Resistor necessary for Photoelectric barrier (Open-Collector)
    sei();
    setupExtInterrupt();
    while (1) {

        if (BIT_IS_SET(PINB, PB4)) { //BIT_IS_SET(PINB, PB4)==normally opened BIT_IS_SET(!PINB, PB4)==normally closed
            for (i = 0; i < 100; i++) {
                if (i == 0) {
                    usartStringPrint("%d,", MMcounter);
                    usartStringPrintln("%d", counter); //must later be multiplied by 0.005 because of the resolution of the glass dipstick of 0.005 We did it externally because of overflow problems
                    count++;
                    home = true;
                }
                Toggle(home);
            }
        }

        Toggle(home);
        if (isr) {
            // usartStringPrintln("referencePoint");	//check for reference point
            isr = false;
        }
        if (count == 100) {
            usartStringPrintln("\n");
            usartStringPrintln("Programm finished");
            return 0;
        }
    }
}
