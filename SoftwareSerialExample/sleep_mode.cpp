#include <avr/power.h>
#include <avr/sleep.h>

#include "sleep_mode.h"

void sleepNow()
{

set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here

sleep_enable(); // enables the sleep bit in the mcucr register
// so sleep is possible. just a safety pin

power_adc_disable();
power_spi_disable();
power_timer0_disable();
power_timer1_disable();
power_timer2_disable();
power_twi_disable();

sleep_mode(); // here the device is actually put to sleep!!

// THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
sleep_disable(); // first thing after waking from sleep:
// disable sleep...

power_all_enable();

}

