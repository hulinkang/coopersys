/*-----------------------------------------------------------------
		led.h
------------------------------------------------------------------*/

#ifndef _LED_H
#define _LED_H


sbit led_pin = P1^0;

#define LED_ON  1
#define LED_OFF 0

void led_flash_init(void);
void led_flash(void);


#endif