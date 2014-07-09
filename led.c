#include "OS.h"
#include "led.h"

void led_flash_init(void){
	led_pin = 0;
}

void led_flash(void){
	led_pin = !led_pin;
}