/***********************************************************************//** 
 * \file  freertos_demo.c
 * \brief  freertos demo code
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date      <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-6-27 <td>V1.0     <td>LHY     <td>initial
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


#define DEMO_CHOOSE 0                  //0---任务的创建和删除、挂起和恢复
                                       //1---消息队列
					                   //2---信号量
					                   //3---互斥锁
					 

//任务1
k_task_handle_t task1_handle;           //任务1 句柄
#define TSK1_PRIO             2         //任务1 优先级
#define TASK1_STK_SIZE       (1*512)    //任务1 分配的堆栈大小

//任务2
k_task_handle_t task2_handle;           //任务2 句柄
#define TSK2_PRIO             3         //任务2 优先级
#define TASK2_STK_SIZE       (1*512)    //任务2 分配的堆栈大小

//任务3
k_task_handle_t task3_handle;           //任务3 句柄
#define TSK3_PRIO             4         //任务3 优先级
#define TASK3_STK_SIZE       (1*512)    //任务3 分配的堆栈大小

//开始任务
k_task_handle_t start_task_handle;      //开始任务 句柄
#define START_TSK_PRIO       1          //开始任务 优先级
#define START_TSK_STK_SIZE   1024       //开始任务 分配的堆栈大小

#define TEST_TIME_QUANTA 100

#if (DEMO_CHOOSE == 1)
k_msgq_handle_t   NewQueue;	            //消息队列
#elif (DEMO_CHOOSE == 2)
k_sem_handle_t    g_usSem;              //信号量
#elif (DEMO_CHOOSE == 3)
k_mutex_handle_t g_Mutex;               //互斥量
#endif


/** \brief 任务1，在该任务里面，包括了任务的挂起和恢复，向消息队列写数据，释放信号量
 *  \param[in] none
 *  \return none
 */
void task1(void)
{
#if (DEMO_CHOOSE == 0)
	uint32_t uwRet;
	uint8_t count = 0;
	while(1)
	{
		count++;
		my_printf("task 1 run %d times!!!\r\n", count);
		
		if(count == 10)
		{
			uwRet = csi_kernel_task_suspend(task2_handle);        //任务挂起
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
			uwRet = csi_kernel_task_resume(task2_handle);         //任务恢复
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
	uint8_t count = 0;
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
			csi_kernel_msgq_put(NewQueue, tx, 0, -1);          //向消息队列写数据
		}
		else if(count == 10)
		{
			tx[0] = 5;
			tx[1] = 6;
			tx[2] = 7;
			csi_kernel_msgq_put(NewQueue, tx, 0, -1);         //向消息队列写数据
		}
		else if(count == 15)
		{
			tx[0] = 8;
			tx[1] = 9;
			tx[2] = 10;
			csi_kernel_msgq_put(NewQueue, tx, 0, -1);         //向消息队列写数据
		}
		else if(count == 20)
		{
			tx[0] = 11;
			tx[1] = 12;
			tx[2] = 13;
			csi_kernel_msgq_put(NewQueue, tx, 0, -1);         //向消息队列写数据
			count = 0;
		}
		else;
		
		csi_kernel_delay_ms(1000);
	}
	
#elif (DEMO_CHOOSE == 2)
	uint8_t count = 0;
	while(1)
	{
		count++;
		my_printf("task 1 run %d times!!!\r\n", count);
		
		if(count == 10)
		{
			if(0 != csi_kernel_sem_post(g_usSem))             //释放信号量
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
	
#elif (DEMO_CHOOSE == 3)

uint32_t i = 0;
    while(1)
	{
		csi_kernel_mutex_lock(g_Mutex,portMAX_DELAY);        //获取互斥锁
		my_printf("Low task running!\r\n");
		for(i=0;i<2000000;i++)
		{
		  taskYIELD();			
		}		
		csi_kernel_mutex_unlock(g_Mutex);                    //释放互斥锁
		csi_kernel_delay_ms(1000);
	}
		
#endif
}

/** \brief 任务2，在该任务里面，包括了消息队列，信号量获取
 *  \param[in] none
 *  \return none
 */
void task2(void)
{
#if (DEMO_CHOOSE == 0)
	while(1)
	{
		my_printf("Task 2 is working now!!!\r\n");
		csi_kernel_delay_ms(1000);
	}
#elif (DEMO_CHOOSE == 1)
	u_char rec[3] = {0};
	while(1)
	{
		my_printf("task 2 is working now!!!\r\n");
		csi_kernel_msgq_get(NewQueue, rec, -1);        //获取消息队列数据
		for(int i = 0; i < 3; i++)
		{
			my_printf("rec[%d] = %d\r\n", i, rec[i]);
		}
		
		csi_kernel_delay_ms(1000);
	}
	
#elif (DEMO_CHOOSE == 2)
	while(1)
	{
		if(0 != csi_kernel_sem_wait(g_usSem, -1))     //获取信号量
		{
			my_printf("task 2 take sem fail!!!\r\n");
		}
		else 
		{
			my_printf("task 2 take sem sucess,task 2 is working now!!!\r\n");
		}
	}
	
#elif (DEMO_CHOOSE == 3)
	while(1)
	{
		my_printf("Middle task running!\r\n");
		csi_kernel_delay_ms(1000);
	}

#endif
}

#if (DEMO_CHOOSE == 3)
/** \brief 任务3，在该任务里面，任务3获取互斥锁得到运行，执行完操作释放互斥锁
 *  \param[in] none
 *  \return none
 */
void task3(void)
{
	while(1)
	{
		csi_kernel_delay_ms(500);
		my_printf("High task Pend Semaphore\r\n");
		csi_kernel_mutex_lock(g_Mutex, portMAX_DELAY);      //获取互斥锁
		my_printf("High task running!\r\n");
		csi_kernel_mutex_unlock(g_Mutex);                   //释放互斥锁
		csi_kernel_delay_ms(500);
	}
}
#endif

/** \brief 开始任务，在该任务里面，包括了信号量，消息队列的初始化，其他任务的创建，以及开始任务自身的删除操作
 *  \param[in] none
 *  \return none
 */
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
	
#elif (DEMO_CHOOSE == 3)
	//创建互斥量
	g_Mutex = csi_kernel_mutex_new();
	if(g_Mutex == NULL)
	{
		printf("fail to create mutex.\n");
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
	
	
#if (DEMO_CHOOSE == 3)	
		//创建task 3
	csi_kernel_task_new((k_task_entry_t)task3, "task3", NULL, TSK3_PRIO, TEST_TIME_QUANTA, NULL, TASK3_STK_SIZE, &task3_handle);

    if (task3_handle == NULL) 
	{
        csi_kernel_sched_resume(0);

        my_printf("Fail to create task 3!\r\n");
    }
#endif	
	

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


/** \brief freeRTOS示例代码，包含了任务的任务的创建，删除，挂起，恢复，信号量，互斥锁，消息队列的示例demo
 *         可以通过宏DEMO_CHOOSE来进行选择相应功能示例demo
 *  \param[in] none
 *  \return none
 */
void freertos_demo(void)
{

    my_printf("\r\n-->this is freertos task test demo!!!\r\n");	
   
    //系统初始化
    csi_kernel_init();

	//创建开始任务
    csi_kernel_task_new((k_task_entry_t)start_task, "start_task", 0, START_TSK_PRIO, 0, 0, START_TSK_STK_SIZE, &start_task_handle);

	//任务调度开始
    csi_kernel_start();
	
}



