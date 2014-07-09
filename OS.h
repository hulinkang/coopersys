/*--------------------------------------------------------
 * OS.h
 * 操作系统的头文件
 * 包含系统相关函数声明、任务数据结构、最大任务数宏定义
----------------------------------------------------------*/
 
#ifndef _OS_H
#define _OS_H
 

#include "main.h"

//-----------------最大任务数------------------------------
#define OS_MAX_TASKS 1

//-----------------任务数据类型定义-------------------------
//Store in DATA area
typedef data struct{ 
	void (code *pTask)(void);  // void (void)
	uint16 delay;	//Delay ticks until the function will be run.
	uint16 period;	//Interval ticks  between subsequent runs.
	uint8  runme; 	//Incremented (by scheduler) when task is due to execute.
}sTask;


//------------------公共函数声明----------------------------
void 	OS_init(void);
void 	OS_start(void);

void	OS_tasks_dispatch(void);
uint8 	OS_task_add(void (code *)(void), const uint16, const uint16);
bit 	OS_task_delete(const uint8 );
//void	OS_report_status(void);


#endif