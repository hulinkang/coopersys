#include "main.h"
#include "OS.h"
#include "led.h"

void main(void)
{

	OS_init();
	
	led_flash_init();
	
	OS_task_add(led_flash, 0, 100);
	
	OS_start();
	
	while(1)
	{
		OS_tasks_dispatch();
	}
}