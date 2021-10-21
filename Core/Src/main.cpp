/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <stdint.h>
#include <cstring>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;



/* USER CODE BEGIN PV */
// static const uint8_t EMC2101_ADDR = 100_1100b; 	// EMC2101 address on SMBus
// static const uint8_t REG_INT_TEMP = 00h; 		// register address for Internal temperature
// static const uint8_t REG_EXT_TEMP_HB = 01h;		// register address for Ext temperature High byte
// static const uint8_t REG_EXT_TEMP_LB = 10h;		// register address for Ext temperature Low byte
// static const uint8_t REG_PROD_ID = FDh;			// register address for Product ID
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

class EMCdata {
	
	public:
	// constructor
	EMCdata(uint8_t EMC2101_ADDR,  uint8_t REG_INT_TEMP,
	uint8_t REG_EXT_TEMP_HB,  uint8_t REG_EXT_TEMP_LB,
	uint8_t REG_PROD_ID):EMC2101_ADDR_(EMC2101_ADDR), REG_INT_TEMP_(REG_INT_TEMP), REG_EXT_TEMP_HB_(REG_EXT_TEMP_HB),
	REG_EXT_TEMP_LB_(REG_EXT_TEMP_LB),REG_PROD_ID_(REG_PROD_ID){}

	// destructor
	~EMCdata(){}

	// private:
	HAL_StatusTypeDef ret;
	char mssg_[10];    // message 
	uint8_t buf_; 	// buffer for internal temp
	uint8_t itemp;    // store internal temp
	uint8_t buf2_;	// buffer for product id
	uint8_t prod_;    // store product id
	uint8_t buf3_;	// buffer for ext temp HB
	uint8_t etempHB_;    // store ext temp HB
	uint8_t buf4_;	// buffer for ext temp LB
	uint8_t etempLB_;    // store ext temp LB
	uint16_t val_;		// int temp value
	uint16_t val2_;	// ext temp value
	float temp_c_;	// int temp value in decimal
	float temp_c2_;   // ext temp value in decimal


	uint8_t EMC2101_ADDR_;// = 100_1100b; 	// EMC2101 address on SMBus
	uint8_t REG_INT_TEMP_;// = 00h;	 		// register address for Internal temperature
	uint8_t REG_EXT_TEMP_HB_;// = 01h;		// register address for Ext temperature High byte
	uint8_t REG_EXT_TEMP_LB_;// = 10h;		// register address for Ext temperature Low byte
	uint8_t REG_PROD_ID_;// = FDh;			// register address for Product ID




	// get internal temperature
    void getIntTemp(){
	    
        // Tell EMC2101 that we want to read from the temperature register
	    buf_ = REG_INT_TEMP_;
	    ret = HAL_I2C_Master_Transmit(&hi2c2, EMC2101_ADDR_, &buf_, sizeof(buf_), HAL_MAX_DELAY);
	    if ( ret != HAL_OK ) {
	      strcpy(mssg_, "Error Tx\r\n");
	    } else {

	      // Read 1 bytes from the internal temperature register
	      ret = HAL_I2C_Master_Receive(&hi2c2, EMC2101_ADDR_, &itemp, 1, HAL_MAX_DELAY);
	      if ( ret != HAL_OK ) {
	        strcpy(mssg_, "Error Rx\r\n");
	      } else {

	        //type cast of uint8_t to uint16_t
	        val_ = (uint16_t)itemp;

	        // Convert to 2's complement, since temperature can be negative
	        if ( val_ > 0x7F ) {
          		val_ = 0x10000 - val_;
	        }

	        // Convert to float temperature value (Celsius)
	        temp_c_ = val_ * 1.00; // internal temp are in 1 deg steps

	        // Convert temperature to decimal format
	        temp_c_ *= 100;
	        sprintf(mssg_,
	              "%u.%u C\r\n",
	              ((unsigned int)temp_c_ / 100),
	              ((unsigned int)temp_c_ % 100));
	      }
	    }

		std::cout << mssg_ << std::endl;

	    // Wait
	    HAL_Delay(500);

    }
	

	// get external temperature
    void getExtTemp(){
	    
		int flag1{0}, flag2{0};
        // Tell EMC2101 that we want to read from the ext temperature HB register
	    buf3_ = REG_EXT_TEMP_HB_;
	    ret = HAL_I2C_Master_Transmit(&hi2c2, EMC2101_ADDR_, &buf3_, sizeof(buf3_), HAL_MAX_DELAY);
	    if ( ret != HAL_OK ) {
	      strcpy(mssg_, "Error Tx\r\n");
	    } else {
			// Read 1 bytes from the ext temperature HB register
			ret = HAL_I2C_Master_Receive(&hi2c2, EMC2101_ADDR_, &etempHB_, 1, HAL_MAX_DELAY);
			if ( ret != HAL_OK ) {
				strcpy(mssg_, "Error Rx\r\n");
			} else {
				flag1 = 1;
			}
		}
		
		// Tell EMC2101 that we want to read from the ext temperature LB register
	    buf4_ = REG_EXT_TEMP_LB_;
	    ret = HAL_I2C_Master_Transmit(&hi2c2, EMC2101_ADDR_, &buf4_, sizeof(buf4_), HAL_MAX_DELAY);
	    if ( ret != HAL_OK ) {
	      strcpy(mssg_, "Error Tx\r\n");
	    } else {
			// Read 1 bytes from the ext temperature LB register
			ret = HAL_I2C_Master_Receive(&hi2c2, EMC2101_ADDR_, &etempLB_, 1, HAL_MAX_DELAY);
			if ( ret != HAL_OK ) {
				strcpy(mssg_, "Error Rx\r\n");
			} else {
				flag2 = 1;
			}
			
		}
		
		// if data is recieved from both HB and LB 
		if (flag1 == flag2)
		{
	        //concatenating High byte and low byte to form a 11 bit and storing that into 16 bit.
	        val2_ = ((uint16_t)etempHB_ << 3) | ((uint16_t)etempLB_ >> 5);

	        // Convert to 2's complement, since temperature can be negative
	        if ( val2_ > 0x7FF ) {
          		val_ = 0x10000 - val_;

	        }

	        // Convert to float temperature value (Celsius)
	        temp_c2_ = val2_ * 0.125; // ext temp are in 0.125 deg steps

	        // Convert temperature to decimal format
	        temp_c2_ *= 100;
	        sprintf(mssg_,
	              "%u.%u C\r\n",
	              ((unsigned int)temp_c2_ / 100),
	              ((unsigned int)temp_c2_ % 100));

	    }

		std::cout << mssg_ << std::endl;

	    // Wait
	    HAL_Delay(500);

    }


	//get Product ID
	void getProdID(){
		// Tell EMC2101 that we want to read from the temperature register
	    buf2_ = REG_EXT_TEMP_HB_;
	    ret = HAL_I2C_Master_Transmit(&hi2c2, EMC2101_ADDR_, &buf2_, 1, HAL_MAX_DELAY);
	    if ( ret != HAL_OK ) {
	      strcpy(mssg_, "Error Tx\r\n");
	    } else {

	      // Read 1 bytes from the product id register
	      ret = HAL_I2C_Master_Receive(&hi2c2, EMC2101_ADDR_, &prod_, 1, HAL_MAX_DELAY);
	      if ( ret != HAL_OK ) {
	        strcpy(mssg_, "Error Rx\r\n");
	      } else {

			//// No need to do this as type casting any octa or hexa into int will give the answer in deci!! :)
			//   int size = sizeof(prod_);
			//   int dec = 0;
			//   for(int i = size-1; i >= 0; i--){
			// 	  dec += int(prod_[i])*pow(2,(size-1)-i);
			//   }

			std::cout << "Product ID in decimal : " << int(prod_) << std::endl;
		  }
		}


	}
	
	
};




/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */

	static const uint8_t emc2101_addr = 0x4c<<1; 	// EMC2101 address on SMBus in binary
	static const uint8_t reg_int_temp = 0x00; 		// register address for Internal temperature
	static const uint8_t reg_ext_temp_hb = 0x01;		// register address for Ext temperature High byte
	static const uint8_t reg_ext_temp_lb = 0x10;		// register address for Ext temperature Low byte
	static const uint8_t reg_prod_id = 0xfd;			// register address for Product ID
	
	//creating obj1 of class EMCdata and inputting the addresses parameteres
	EMCdata obj1(emc2101_addr, reg_int_temp, reg_ext_temp_hb, reg_ext_temp_lb, reg_prod_id);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	
	while (1) {

		obj1.getIntTemp(); 	// prints the internal temeperature
		obj1.getExtTemp();	// prints the external temeperature
		obj1.getProdID();	// prints the Product ID

		
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the peripherals clocks
	 */
	PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x0010061A;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
