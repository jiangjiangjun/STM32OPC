/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "stdio.h"
#include "open62541.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
UA_Boolean running = true;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTaskTCPHandle;
osThreadId myTaskLedHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void TCPTask(void const * argument);
void LedTask(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTaskTCP */
  osThreadDef(myTaskTCP, TCPTask, osPriorityIdle, 0, 10240);
  myTaskTCPHandle = osThreadCreate(osThread(myTaskTCP), NULL);

  /* definition and creation of myTaskLed */
  osThreadDef(myTaskLed, LedTask, osPriorityIdle, 0, 128);
  myTaskLedHandle = osThreadCreate(osThread(myTaskLed), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
                 
  /* init code for LWIP */
  MX_LWIP_Init();

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_TCPTask */
/**
* @brief Function implementing the myTaskTCP thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TCPTask */
void TCPTask(void const * argument)
{
  /* USER CODE BEGIN TCPTask */
  /* Infinite loop */
#if 0
    #if 0
	/* Create a client and connect */
	UA_Client *client = UA_Client_new();
    UA_ClientConfig *config = UA_Client_getConfig(client);
   
    config->localConnectionConfig.recvBufferSize = 1600;
    config->localConnectionConfig.sendBufferSize = 1600;
    config->localConnectionConfig.maxMessageSize = 1600;
    
	UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    
	UA_StatusCode status = UA_Client_connect(client, "opc.tcp://192.168.1.100:4840");
    
	if (status != UA_STATUSCODE_GOOD) {
		UA_Client_delete(client);
        for(;;)
        {
            HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
            osDelay(200);
        }
	}else{
        /* Read the value attribute of the node. UA_Client_readValueAttribute is a
         * wrapper for the raw read service available as UA_Client_Service_read. */
        UA_Variant value; /* Variants can hold scalar values and arrays of any type */
        UA_Variant_init(&value);
        for(;;)
        {
            status = UA_Client_readValueAttribute(client, UA_NODEID_STRING(1, "the.answer"), &value);
            if (status == UA_STATUSCODE_GOOD &&
                UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32])) {
                printf("the value is: %i\n", *(UA_Int32*)value.data);
            }
            osDelay(1000);
        }
        /* Clean up */
        UA_Variant_deleteMembers(&value);
        UA_Client_delete(client); /* Disconnects the client internally */
    }

    #else
    //config = UA_ServerConfig_new_customBuffer(4840, NULL, 8192, 8192);
    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));
    
    //Set the connection config
    UA_ConnectionConfig connectionConfig;
    connectionConfig.recvBufferSize = 16000; //16384
    connectionConfig.sendBufferSize = 16000; //16384
    connectionConfig.maxMessageSize = 16000; //16384

    //UA_ServerNetworkLayer nl = UA_ServerNetworkLayerTCP(connectionConfig, 4840, NULL);
    
    config->networkLayers->localConnectionConfig.recvBufferSize = 16000;
    config->networkLayers->localConnectionConfig.sendBufferSize = 16000;
    config->networkLayers->localConnectionConfig.maxMessageSize = 16000;
    UA_ServerConfig_setCustomHostname(config, UA_STRING("192.168.1.200"));
    
	/* Add a variable node */
	/* 1) Define the node attributes */
	UA_VariableAttributes attr = UA_VariableAttributes_default;
	attr.displayName = UA_LOCALIZEDTEXT("en-US", "the answer");
	UA_Int32 myInteger = 88;
	UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","the answer");
	attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","the answer");

	/* 2) Define where the node shall be added with which browsename */
	UA_NodeId newNodeId = UA_NODEID_STRING(1, "the.answer");
	UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_NodeId variableType = UA_NODEID_NULL; /* take the default variable type */
	UA_QualifiedName browseName = UA_QUALIFIEDNAME(1, "the answer");

    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	/* 3) Add the node */
	UA_Server_addVariableNode(server, newNodeId, parentNodeId, parentReferenceNodeId,
		browseName, variableType, attr, NULL, NULL);

	/* Run the server loop */
	UA_StatusCode status = UA_Server_run(server, &running);

	UA_Server_delete(server);
    #endif
#else
    for(;;)
    {
        osDelay(10);
    }
#endif
  /* USER CODE END TCPTask */
}

/* USER CODE BEGIN Header_LedTask */
/**
* @brief Function implementing the myTaskLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LedTask */
uint8_t test_buf[50000]__attribute__((at(0xC0100000)));
void LedTask(void const * argument)
{
  /* USER CODE BEGIN LedTask */
  //uint8_t test_buf[50000]__attribute__((at(0xC0100000)));
  uint32_t Buf_Addr;
  uint16_t i;
  
  Buf_Addr = (uint32_t)&test_buf[0];
  printf("ABCD");
  for(i=0;i<2000;i++){
      test_buf[i] = (uint8_t)i;
  }
  printf("SDFGH");
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
    printf("%#x \r\n",Buf_Addr);
    
    for(i=0;i<200;i++){
        printf("%d ",test_buf[i]);
    }
    printf("\r\n");
    osDelay(200);
  }
  /* USER CODE END LedTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
