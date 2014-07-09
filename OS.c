/*----------------------------------------------------------
 *	OS.C
 *	系统内核函数
-----------------------------------------------------------*/
#include "OS.h"

//定义任务数组
sTask OS_tasks[OS_MAX_TASKS];

//私有函数声明
static void OS_sleep(void);

/*----------------------------------------------------------
*	
-----------------------------------------------------------*/

//-------------------系统初始化函数-------------------------
//准备调度器以及配置系统时钟
void OS_init(void){
	uint8 i;
	for(i = 0; i < OS_MAX_TASKS; i++){
		OS_task_delete(i);	//会系统报错因为这时候任务队列为空
	}
	
	//设置系统时钟//1ms@11.0592MHz
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = (65536 - OSC_FREQ / OSC_PER_INST / 1000);		//设置定时初值
	TH0 = (65536 - OSC_FREQ / OSC_PER_INST / 1000) >> 8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}

//-------------------系统开始函数---------------------------
//Note:Usually called after all tasks  are added,to keep the tasks sync.
//Note:Only the scheduler interrupt should be enable.
void OS_start(void){
	EA = 1;
}

//--------------------调度函数----------------------------
//定时器中断函数
void OS_task_update(void) interrupt ISR_T0{
	uint8 i;
	TR0 = 0;
	TL0 = (65536 - OSC_FREQ / OSC_PER_INST / 1000);		//设置定时值
	TH0 = (65536 - OSC_FREQ / OSC_PER_INST / 1000) >> 8;
	TR0 = 1;
	for(i = 0; i < OS_MAX_TASKS; i++){
		//Check if there is a task.
		if(OS_tasks[i].pTask){
			if(OS_tasks[i].delay == 0){
				//The task should run.
				OS_tasks[i].runme ++;
				
				if(OS_tasks[i].period){//周期若为0则表示是只运行一次
					OS_tasks[i].delay = OS_tasks[i].period;
				}
			}
			else{
				//No yet to run,reduce the delay time.
				OS_tasks[i].delay --;
			}
		}
	}
}

//-------------------任务分派函数----------------------------
void OS_tasks_dispatch(void){
	uint8 i;
	for(i = 0; i < OS_MAX_TASKS; i++){
		if(OS_tasks[i].runme > 0){
			(*OS_tasks[i].pTask)();	//Run the task
			OS_tasks[i].runme--;		//
			
			//如果是周期函数则会再次运行，否则从任务数组中删除
			if(OS_tasks[i].period == 0){
				OS_task_delete(i);
			}
		}
	}
	//OS_report_status();
	OS_sleep();
}

//------------------任务添加函数------------------------------
uint8 OS_task_add(void (code *pTask)(),
				  const uint16 delay,
				  const uint16 period){
	uint8 i = 0;
	//try to find a location in the task array
	while((OS_tasks[i].pTask != 0) && (i < OS_MAX_TASKS)){
		i++;
	}
	
	if(i == OS_MAX_TASKS){
		//report Error  ,NOT add yet.
		return OS_MAX_TASKS;
	}
	
	OS_tasks[i].pTask  = pTask;
	OS_tasks[i].delay  = delay;
	OS_tasks[i].period = period;
	
	OS_tasks[i].runme  = 0;
	
	return i;
}

//------------------任务删除函数-------------------------------
bit OS_task_delete(const uint8 task_i){
	if(OS_tasks[task_i].pTask == 0){
		//No task at this location
		return 1;
	}
	
	OS_tasks[task_i].pTask  = 0x000;
	OS_tasks[task_i].delay  = 0;
	OS_tasks[task_i].period = 0;
	
	OS_tasks[task_i].runme  = 0;

	return 0;
}

//------------------报告状态函数------------------------------
//待添加


//------------------系统睡眠函数------------------------------
//Idle mode
//唤醒条件：1)任何一个中断	2)复位
void OS_sleep(){
	PCON |= 0x01;
}


