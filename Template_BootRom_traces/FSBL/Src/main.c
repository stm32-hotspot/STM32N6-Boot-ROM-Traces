/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
//#include <unistd.h>
#include "main.h"
#include "common.h"
#include "com.h"
#include "rom_trace_parser.h"

#define TEXT_BUFFER_SIZE (128)
#define BIN_BUFFER_SIZE  (4*1024)

#if defined(__ICCARM__) || defined ( __CC_ARM ) || defined(__ARMCC_VERSION)
int io_putchar(int ch);
#define PUTCHAR_PROTOTYPE int io_putchar(int ch)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif /* __ICCARM__ */

/* Global variables ----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

PUTCHAR_PROTOTYPE
{
  COM_Transmit((uint8_t*)&ch, 1, TX_TIMEOUT);
  return ch;
}

/* Redirects printf to TFM_DRIVER_STDIO in case of ARMCLANG*/
#if defined(__ARMCC_VERSION)
FILE __stdout;

/* __ARMCC_VERSION is only defined starting from Arm compiler version 6 */
int fputc(int ch, FILE *f)
{
  /* Send byte to USART */
  io_putchar(ch);

  /* Return character written */
  return ch;
}
#elif defined(__GNUC__)
/* Redirects printf to TFM_DRIVER_STDIO in case of GNUARM */
int _write(int fd, char *str, int len)
{
  int i;

  for (i = 0; i < len; i++)
  {
    /* Send byte to USART */
    __io_putchar(str[i]);
  }

  /* Return the number of characters written */
  return len;
}
#elif defined(__ICCARM__)
size_t __write(int file, unsigned char const *ptr, size_t len)
{
  size_t idx;
  unsigned char const *pdata = ptr;

  for (idx=0; idx < len; idx++)
  {
    io_putchar((int)*pdata);
    pdata++;
  }
  return len;
}
#endif /*  __GNUC__ */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void MPU_Config(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void print_and_sort_sec_nsec_bootrom_traces(uint32_t buffer_size)
{
   COM_Init();

  /* 
  * Declaration
  */
  char            ascii_string_sec[TEXT_BUFFER_SIZE];
  uint32_t        timestamp_sec = 0;
  int32_t         ret_c_sec = ROM_TRACE_ERR_OK;
  uint8_t         newWordReady_sec = 0;   // [0 = no new word, 1 = new word ready]
  romTrace_Iterator_t  iterator_sec;

  char            ascii_string_Nsec[TEXT_BUFFER_SIZE];
  uint32_t        timestamp_Nsec;
  int32_t         ret_c_Nsec = ROM_TRACE_ERR_OK;
  uint8_t         newWordReady_Nsec = 1;  // [0 = no new word, 1 = new word ready]
  romTrace_Iterator_t  iterator_Nsec;

  /* 
  * Normal function
  */
  /* Initialize sec and Nsec iterator to read both traces */
    if ((romTrace_IterateInit(&iterator_sec, (uint8_t*)0x341037F0, buffer_size) == ROM_TRACE_ERR_OK) && \
      (romTrace_IterateInit(&iterator_Nsec,(uint8_t*)0x241077f0, buffer_size) == ROM_TRACE_ERR_OK))      
   {
    /* While the end of the buffer is not attemped and no error occur */
    while ((ret_c_sec != ROM_TRACE_ERR_END_OF_BUFFER) || (ret_c_Nsec != ROM_TRACE_ERR_END_OF_BUFFER))
    {
      /* Read the next word only if there the iterator is not finished AND the previous word is used */
      if((ret_c_sec != ROM_TRACE_ERR_END_OF_BUFFER) && !newWordReady_sec)
      {
        ret_c_sec = romTrace_IterateNext(&iterator_sec, ascii_string_sec, &timestamp_sec, TEXT_BUFFER_SIZE);
        if(ret_c_sec != ROM_TRACE_ERR_END_OF_BUFFER)   // if the next read is a success
          newWordReady_sec = 1;   // Set newWordReady flag 
      }
      if((ret_c_Nsec != ROM_TRACE_ERR_END_OF_BUFFER) && !newWordReady_Nsec)
      {
        ret_c_Nsec = romTrace_IterateNext(&iterator_Nsec, ascii_string_Nsec, &timestamp_Nsec, TEXT_BUFFER_SIZE);
        if(ret_c_Nsec != ROM_TRACE_ERR_END_OF_BUFFER)  // if the next read is a success
          newWordReady_Nsec = 1;  // Set newWordReady flag 
      }

      if(newWordReady_sec && newWordReady_Nsec)   // sec and nsec word ready
      {
        if(timestamp_sec < timestamp_Nsec)
        {
          if(ret_c_sec == ROM_TRACE_ERR_OK)
            fprintf(stdout, "%s\r\n", ascii_string_sec);
          newWordReady_sec = 0;   // Need to read a new word
        }
        else
        {
          if(ret_c_Nsec == ROM_TRACE_ERR_OK)
            fprintf(stdout, "%s\r\n", ascii_string_Nsec);
          newWordReady_Nsec = 0;  // Need to read a new word
        }
      }
      else if(newWordReady_sec)
      {
        if(ret_c_sec == ROM_TRACE_ERR_OK)
          fprintf(stdout, "%s\r\n", ascii_string_sec);
        newWordReady_sec = 0;   // Need to read a new word
      }
      else if(newWordReady_Nsec)
      {
        if(ret_c_Nsec == ROM_TRACE_ERR_OK)
          fprintf(stdout, "%s\r\n", ascii_string_Nsec);
        newWordReady_Nsec = 0;  // Need to read a new word
      }
    }
  }

  /* 
    * End of function
    */
  if(ret_c_sec != ROM_TRACE_ERR_END_OF_BUFFER || ret_c_Nsec != ROM_TRACE_ERR_END_OF_BUFFER)
  {
    if (ret_c_sec != ROM_TRACE_ERR_OK && ret_c_sec != ROM_TRACE_ERR_END_OF_BUFFER)
      fprintf(stdout, "%s\r\n", "\033[31mError while decoding secure traces\033[0m");
    if (ret_c_Nsec != ROM_TRACE_ERR_OK && ret_c_Nsec != ROM_TRACE_ERR_END_OF_BUFFER)
      fprintf(stdout, "%s\r\n", "\033[31mError while decoding non secure traces\033[0m");
  }
  else
  {
    fprintf(stdout, "%s\r\n", "\033[34m==================================================================\033[0m");
  }

  return ;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  
  print_and_sort_sec_nsec_bootrom_traces(2048);
  
  /* USER CODE BEGIN 1 */
  /* Enable and set up the MPU------------------------------------------------*/
  MPU_Config();
  /* USER CODE END 1 */

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
 /* ---------- MCO1 pin output example -------------------------*/
  /* Output CPU clock divided by 10 on MCO1 pin (PA8) */
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSA,  RCC_MCODIV_10);

  /* ---------- MCO2 pin output example -------------------------*/
  /* Enable VDDIO4 for PC.09 */
  HAL_PWREx_EnableVddIO4();
  /* Output SYS clock divided by 10 on MCO2 pin (PC9) */
  HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSB,  RCC_MCODIV_10);

  /* Init leds */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Toggle leds */
    HAL_Delay(200);
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(200);
    BSP_LED_Toggle(LED_RED);
  }
  /* USER CODE END 3 */
}
/* USER CODE BEGIN CLK 1 */

/*         The system Clock is configured as follows :
              CPU Clock source               = IC1_CK
              System bus Clock source        = IC2_IC6_IC11_CK
              CPUCLK (sysa_ck) (Hz)          = 600000000
              SYSCLK AXI (sysb_ck) (Hz)      = 400000000
              SYSCLK NPU (sysc_ck) (Hz)      = 300000000
              SYSCLK AXISRAM3/4/5/6 (sysd_ck) (Hz) = 400000000
              HCLKx(Hz)                      = 200000000
              PCLKx(Hz)                      = 200000000
              AHB Prescaler                  = 2
              APB1 Prescaler                 = 1
              APB2 Prescaler                 = 1
              APB4 Prescaler                 = 1
              APB5 Prescaler                 = 1
              PLL1 State                     = ON
              PLL1 clock source              = HSI
              PLL1 M                         = 4
              PLL1 N                         = 75
              PLL1 P1                        = 1
              PLL1 P2                        = 1
              PLL1 FRACN                     = 0
              PLL2 State                     = BYPASS
              PLL2 clock source              = HSI
              PLL3 State                     = BYPASS
              PLL3 clock source              = HSI
              PLL4 State                     = BYPASS
              PLL4 clock source              = HSI
*/
/* USER CODE END CLK 1 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the System Power Supply
  */
  if (HAL_PWREx_ConfigSupply(PWR_EXTERNAL_SOURCE_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable HSI */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Get current CPU/System buses clocks configuration and if necessary switch
 to intermediate HSI clock to ensure target clock can be set
  */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct);
  if ((RCC_ClkInitStruct.CPUCLKSource == RCC_CPUCLKSOURCE_IC1) ||
     (RCC_ClkInitStruct.SYSCLKSource == RCC_SYSCLKSOURCE_IC2_IC6_IC11))
  {
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK);
    RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 4;
  RCC_OscInitStruct.PLL1.PLLN = 75;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_CPUCLK|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_PCLK5
                              |RCC_CLOCKTYPE_PCLK4;
  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_IC1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 2;
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 3;
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 4;
  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 3;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void MPU_Config(void)
{
  MPU_Region_InitTypeDef default_config = {0};
  MPU_Attributes_InitTypeDef attr_config = {0};
  uint32_t primask_bit = __get_PRIMASK();
  __disable_irq();

  /* disable the MPU */
  HAL_MPU_Disable();

  /* create an attribute configuration for the MPU */
  attr_config.Attributes = INNER_OUTER(MPU_NOT_CACHEABLE);
  attr_config.Number = MPU_ATTRIBUTES_NUMBER0;

  HAL_MPU_ConfigMemoryAttributes(&attr_config);

  /* Create a non cacheable region */
  /*Normal memory type, code execution allowed */
  default_config.Enable = MPU_REGION_ENABLE;
  default_config.Number = MPU_REGION_NUMBER0;
  default_config.BaseAddress = __NON_CACHEABLE_SECTION_BEGIN;
  default_config.LimitAddress =  __NON_CACHEABLE_SECTION_END;
  default_config.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  default_config.AccessPermission = MPU_REGION_ALL_RW;
  default_config.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  default_config.AttributesIndex = MPU_ATTRIBUTES_NUMBER0;
  HAL_MPU_ConfigRegion(&default_config);

  /* enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

  /* Exit critical section to lock the system and avoid any issue around MPU mechanisme */
  __set_PRIMASK(primask_bit);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Prevent unused argument(s) compilation warning */
  UNUSED(file);
  UNUSED(line);

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
