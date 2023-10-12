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


#define DEMO_CHOOSE 1//0---任务的创建和删除、挂起和回复
                     //1---消息队列
					 //2---信号量
					 //3---互斥锁
					 //4---事件组
					 //5---任务通知
					 //6---流和消息缓冲区
					 //7---相对延时和绝对延时


//任务1
k_task_handle_t task1_handle;           //任务1 句柄
#define TSK1_PRIO            3         //任务1 优先级
#define TASK1_STK_SIZE       (1*1024)   //任务1 分配的堆栈大小

//任务2
k_task_handle_t task2_handle;           //任务2 句柄
#define TSK2_PRIO            2         //任务2 优先级
#define TASK2_STK_SIZE       (1*1024)   //任务2 分配的堆栈大小

//开始任务
k_task_handle_t start_task_handle;      //开始任务 句柄
#define START_TSK_PRIO       1          //开始任务 优先级
#define START_TSK_STK_SIZE   1024       //开始任务 分配的堆栈大小


#define TEST_TIME_QUANTA 100

#if (DEMO_CHOOSE == 1)
k_msgq_handle_t   NewQueue;	//消息队列
#elif (DEMO_CHOOSE == 2)
k_sem_handle_t    g_usSem;  //信号量
#endif


void task1(void)
{
	uint8_t count = 0;
	
#if (DEMO_CHOOSE == 0)
	uint32_t uwRet;
	while(1)
	{
		count++;
		my_printf("task 1 run %d times!!!\r\n", count);
		
		if(count == 10)
		{
			uwRet = csi_kernel_task_suspend(task2_handle);
			if(uwRet != 0)
			{
				my_printf("task 2 is suspended return error\r\n");
			}
			else 
			{
				my_printf("task 2 is suspended\r\n");
			}
		}
		else if(count == 20)
		{
			uwRet = csi_kernel_task_resume(task2_handle);
			if(uwRet != 0)
			{
				my_printf("task 2 is resumed return error\r\n");
			}
			else 
			{
				my_printf("task 2 is resumed\r\n");
			}
			
			count = 0;
		}
		else;
		
		csi_kernel_delay_ms(1000);
	}
#elif (DEMO_CHOOSE == 1)

	u_char tx[3] = {0};
	while(1)
	{
		count++;
		my_printf("task 1 run %d times!!!\r\n", count);
		
		if(count == 5)
		{
			tx[0] = 1;
			tx[1] = 2;
			tx[2] = 3;
			csi_kernel_msgq_put(NewQueue, tx, 0, -1);
		}
		else if(count == 10)
		{
			tx[0] = 5;
			tx[1] = 6;
			tx[2] = 7;
			csi_kernel_msgq_put(NewQueue, tx, 0, -1);
			//count = 0;
		}
		else if(count == 15)
		{
				tx[0] = 8;
				tx[1] = 9;
				tx[2] = 10;
				csi_kernel_msgq_put(NewQueue, tx, 0, -1);
		}
		else if(count == 20)
		{
				tx[0] = 11;
				tx[1] = 12;
				tx[2] = 13;
				csi_kernel_msgq_put(NewQueue, tx, 0, -1);
				count = 0;
		}
		else;
		
		csi_kernel_delay_ms(1000);
		
	}
	
#elif (DEMO_CHOOSE == 2)

	while(1)
	{
		count++;
		my_printf("task 1 run %d times!!!\r\n", count);
		
		if(count == 10)
		{
			if(0 != csi_kernel_sem_post(g_usSem))
			{
				my_printf("---task 1 give sem fail!!!\r\n");
			}
			else 
			{
				my_printf("---task 1 give sem sucess!!!\r\n");
			}
			
			count = 0;
		}
		
		csi_kernel_delay_ms(1000);
	}
		
#endif
	



}


void task2(void)
{
#if (DEMO_CHOOSE == 0)
	while(1)
	{
		my_printf("task 2 is working now!!!\r\n");
		csi_kernel_delay_ms(1000);
	}
#elif (DEMO_CHOOSE == 1)
	u_char rec[3] = {0};
	while(1)
	{
		my_printf("task 2 is working now!!!\r\n");
		csi_kernel_msgq_get(NewQueue, rec, -1);
		for(int i = 0; i < 3; i++)
		{
			my_printf("rec[%d] = %d\r\n", i, rec[i]);
		}
		
		csi_kernel_delay_ms(1000);
	}
	
#elif (DEMO_CHOOSE == 2)

	while(1)
	{
		if(0 != csi_kernel_sem_wait(g_usSem, -1))
		{
			my_printf("task 2 take sem fail!!!\r\n");
		}
		else 
		{
			my_printf("task 2 take sem sucess,task 2 is working now!!!\r\n");
		}
	}
#endif
}

void start_task(void)
{
	//进入临界区
	taskENTER_CRITICAL(); 

#if (DEMO_CHOOSE == 1)
	//创建队列
	NewQueue = csi_kernel_msgq_new(5, 3*sizeof(u_char)); 
	if(NewQueue == NULL)
	{
		printf("fail to create queue.\n");
	}
#elif (DEMO_CHOOSE == 2)
	//创建二值信号量
	
	g_usSem = csi_kernel_sem_new(1, 0);
    if (g_usSem == NULL) 
	{
        printf("fail to create semaphore.\n");
    }
        
#endif

	//创建task 1
    csi_kernel_task_new((k_task_entry_t)task1, "task1", NULL, TSK1_PRIO, TEST_TIME_QUANTA, NULL, TASK1_STK_SIZE, &task1_handle);

    if (task1_handle == NULL) 
	{
        csi_kernel_sched_resume(0);

        my_printf("Fail to create task 1!\r\n");
    }

	//创建task 2
	csi_kernel_task_new((k_task_entry_t)task2, "task2", NULL, TSK2_PRIO, TEST_TIME_QUANTA, NULL, TASK2_STK_SIZE, &task2_handle);

    if (task2_handle == NULL) 
	{
        csi_kernel_sched_resume(0);

        my_printf("Fail to create task 2!\r\n");
    }

	//删除开始任务
    if(0 != csi_kernel_task_del(csi_kernel_task_get_cur()))
	{
		my_printf("Fail to delete start_task!\r\n");
	}
	else 
	{
		my_printf("start_task is deleted!\r\n");
	}

	
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



