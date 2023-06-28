/***********************************************************************//** 
 * \file  freertos_demo.c
 * \brief  freertos demo code
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-6-27 <td>V2.0 <td>LHY    <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include <csi_config.h>
#include <stdint.h>
#include <stdio.h>
#include <csi_kernel.h>
#include "FreeRTOS.h"
#include "task.h"
#include <iostring.h>
#include "demo.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


//任务1
k_task_handle_t task1_handle;           //任务1 句柄
#define TSK1_PRIO            11         //任务1 优先级
#define TASK1_STK_SIZE       (1*1024)   //任务1 分配的堆栈大小

//任务2
k_task_handle_t task2_handle;           //任务2 句柄
#define TSK2_PRIO            10         //任务2 优先级
#define TASK2_STK_SIZE       (1*1024)   //任务2 分配的堆栈大小

//开始任务
k_task_handle_t start_task_handle;      //开始任务 句柄
#define START_TSK_PRIO       5          //开始任务 优先级
#define START_TSK_STK_SIZE   1024       //开始任务 分配的堆栈大小


#define TEST_TIME_QUANTA 100

void task1(void)
{
	while(1)
	{
		my_printf("\r\ntask1 now!!!\r\n");
		
		csi_kernel_delay_ms(500);
	}
//    uint32_t uwRet;
//
//    printf("Enter TaskHi Handler.\r\n");
//
//    uwRet = csi_kernel_delay(100);
//
//    if (uwRet != 0) {
//        printf("Kernel delay return error.\r\n");
//    }
//
//    printf("TaskHi csi_kernel_delay Done.\r\n");
//
//    uwRet = csi_kernel_task_suspend(g_uwTskHi);
//
//    if (uwRet != 0) {
//        printf("Suspend TaskHi return error.\r\n");
//    }
//
//    printf("TaskHi csi_kernel_task_resume returned.\r\n");
//
//    printf("test kernel task successfully !\n");
//    csi_kernel_task_del(g_uwTskHi);
//    printf("example task delete self return error .\n");
}


void task2(void)
{
//	uint32_t TotalRunTime;
//	uint32_t ArraySize,x;
//	TaskStatus_t *StatusArray;

//第一步：xTaskGetSystemState()的使用
//		printf("/*******xTaskGetSystemState()**********/\r\n");
//		ArraySize = csi_kernel_task_get_count();		//获取任务数量
//		StatusArray=pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//内存申请
//		if(StatusArray!=NULL)					//内存申请成功
//		{
//			ArraySize=uxTaskGetSystemState((TaskStatus_t* 	)StatusArray, 	//任务信息存储数组
//										   (UBaseType_t		)ArraySize, 	//任务信息存储数组大小
//										   (uint32_t*		)&TotalRunTime);//保存系统运行时间
//			    printf("TaskName     Priority     TaskNumber\r\n");
//			for(x=0;x<ArraySize;x++)
//			{
//				//打印信息
//				printf("%s\t\t%d\t\t%d\r\n",				
//						StatusArray[x].pcTaskName,
//						(int)StatusArray[x].uxCurrentPriority,
//						(int)StatusArray[x].xTaskNumber);
//				
//			}
//		}
//		vPortFree(StatusArray);	//释放内存
//		printf("/**************************end**************************/\r\n");
		
		
		
		//	//第二步：vTaskGetInfo()的使用
//		TaskStatus_t TaskStatus;
//		
//		printf("/***********vTaskGetInfo()*************/\r\n");

		//获取此任务信息
//		vTaskGetInfo((TaskHandle_t	)task1_handle, 		//任务句柄
//					 (TaskStatus_t*	)&TaskStatus, 		//任务信息结构体
//					 (BaseType_t	)pdTRUE,			//允许统计任务堆栈历史最小剩余量大小
//					 (eTaskState	)eInvalid);			//运行状态
		//打印
//		printf("task name:                  %s\r\n",TaskStatus.pcTaskName);
//		printf("task no. :                  %d\r\n",(int)TaskStatus.xTaskNumber);
//		printf("task state:                 %d\r\n",TaskStatus.eCurrentState);
//		printf("task prio:                  %d\r\n",(int)TaskStatus.uxCurrentPriority);
//		printf("task base_prio:             %d\r\n",(int)TaskStatus.uxBasePriority);
//		printf("task stack_base_addr:       %#x\r\n",(int)TaskStatus.pxStackBase);
//		printf("the rest of usable satck:   %d\r\n",TaskStatus.usStackHighWaterMark);
//		printf("the rest of usable heap :   %d\r\n",xPortGetMinimumEverFreeHeapSize());
//		printf("/**************************end***************************/\r\n");
	while(1)
	{
		my_printf("\r\ntask2 now!!!\r\n");
		csi_kernel_delay_ms(500);
	}
}

void start_task(void)
{
	//进入临界区
	taskENTER_CRITICAL();           

	//创建任务1
    csi_kernel_task_new((k_task_entry_t)task1, "task1", NULL, TSK1_PRIO, TEST_TIME_QUANTA, NULL, TASK1_STK_SIZE, &task1_handle);

    if (task1_handle == NULL) 
	{
        csi_kernel_sched_resume(0);

        printf("Fail to create task1 !\r\n");
    }

	//创建任务2
    csi_kernel_task_new((k_task_entry_t)task2, "task2", NULL, TSK2_PRIO, TEST_TIME_QUANTA, NULL, TASK2_STK_SIZE, &task2_handle);

    if (task2_handle == NULL) 
	{
        csi_kernel_sched_resume(0);

        printf("Fail to create task2!\r\n");
    }

	//删除开始任务
    csi_kernel_task_del(csi_kernel_task_get_cur());
	
	//退出临界区
	taskEXIT_CRITICAL();            
}






void freertos_demo(void)
{

    my_printf("\r\n-->this is freertos task test demo!!!\r\n");		//print message
   
    //系统初始化
    csi_kernel_init();

	//创建开始任务
    csi_kernel_task_new((k_task_entry_t)start_task, "start_task", 0, START_TSK_PRIO, 0, 0, START_TSK_STK_SIZE, &start_task_handle);

	//任务调度开始
    csi_kernel_start();
	
}



