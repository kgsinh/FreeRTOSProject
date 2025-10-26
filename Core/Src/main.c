#include<stdio.h>
#include "main.h"
#include "cmsis_os.h"
#include "led.h"
#include "button.h"
#include "pwm.h"


/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);



int __io_putchar(int ch);
void vGreenLedControllerTask(void *pvParameters);
void vBlueLedControllerTask(void *pvParameters);
void vRedLedControllerTask(void *pvParameters);
void vButtonControllerTask(void *pvParameters);
//void vPatternGeneratorTask(void *pvParameters);
//void vMonitorTask(void *pvParameters);

typedef uint32_t TaskProfiler;

TaskProfiler BlueTaskProfiler, RedTaskProfiler,GreenTaskProfiler;
TaskHandle_t xBlueTaskHandle, xRedTaskHandle, xGreenTaskHandle;

int main(void)
{

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  led_gpio_init();
  button_gpio_init();
  pwm_init();
  set_pwm_duty_cycle(70); // Set initial duty cycle to 50%
  set_pwm_brightness(500); // Set initial brightness to 50%

  xTaskCreate(vGreenLedControllerTask,
		  	  "Green Led",
			  128,
			  NULL,
			  2,
			  &xGreenTaskHandle);

  xTaskCreate(vBlueLedControllerTask,
		  	  "PWM Blue Led",
			  128,
			  NULL,
			  2,
			  &xBlueTaskHandle);

  xTaskCreate(vRedLedControllerTask,
 		  	  "Red Led",
 			  128,
 			  NULL,
 			  2,
 			  &xRedTaskHandle);

  if(xTaskCreate(vButtonControllerTask,
		  	  "Button Controller",
			  256,
			  NULL,
			  4,
			  &xButtonTaskHandle)!= pdPASS)
  {
	  printf("Button Controller task creation failed.\n\r");
  }

//  xTaskCreate(vPatternGeneratorTask,
//		  	  "Pattern Generator",
//			  100,
//			  NULL,
//			  2,
//			  NULL);
//
//  xTaskCreate(vMonitorTask,
//		  	  "Monitor Task",
//			  100,
//			  NULL,
//			  1,
//			  NULL);

  button_enable_interrupt();
  vTaskStartScheduler();


  while (1)
  {

  }

}

void vGreenLedControllerTask(void *pvParameters)
{
	while(1)
	{
		GreenTaskProfiler++;

		led_on(10);
		vTaskDelay(500);
		led_off(10);
		vTaskDelay(500);
	}
}

void vBlueLedControllerTask(void *pvParameters)
{
	while(1)
	{
		BlueTaskProfiler++;

		pwm_fade();
		vTaskDelay(100);
	}
}

void vRedLedControllerTask(void *pvParameters)
{
	while(1)
	{
		RedTaskProfiler++;

		led_on(12);
		vTaskDelay(2000);
		led_off(12);
		vTaskDelay(2000);
	}
}

void vButtonControllerTask(void *pvParameters)
{
    printf("=== BUTTON TASK STARTED ===\n\r");

    // Heartbeat to prove task is running
    uint32_t counter = 0;

    while(1)
    {
        // Wait with timeout to show task is alive
        uint32_t notification = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000)); // 5 second timeout

        if(notification > 0)
        {
            printf("*** BUTTON NOTIFICATION RECEIVED ***\n\r");

            vTaskDelay(pdMS_TO_TICKS(50));

            led_off(10);
            set_pwm_duty_cycle(0);
            led_off(12);
            vTaskSuspend(xGreenTaskHandle);
            vTaskSuspend(xBlueTaskHandle);
            vTaskSuspend(xRedTaskHandle);

            printf("LEDs turned off by button\n\r");
        }
        else
        {
            // Timeout - show task is alive
            printf("Button task alive, counter: %lu\n\r", ++counter);
        }
    }
}

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2,(uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
