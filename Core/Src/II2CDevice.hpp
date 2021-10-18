/***************************************************************************
 *                                                                         *
 *   Copyright (C) Jan Mannsbart	                                   	     *
 *                                                                         *
 ***************************************************************************/
#include "stm32g4xx_hal.h"

#ifdef HAL_I2C_MODULE_ENABLED

#pragma once

#include "IPeripheral.hpp"

class II2CDevice: public IPeripheral {
public:
	virtual ePeripheralType::ePeripheralType GetPeripheralType() {
		return ePeripheralType::I2C_DEVICE;
	};

	virtual ePeripheralError::ePeripheralError InitDevice() = 0;
	virtual bool IsDeviceInitialized() = 0;
	virtual int GetCurrentI2CAdr() = 0;
	virtual bool IsDeviceDetected() = 0;

	bool CheckDeviceAvailable(I2C_HandleTypeDef *hi2c, int deviceAdr) {
		uint8_t ret = HAL_I2C_IsDeviceReady(hi2c, (uint16_t) (deviceAdr << 1),
				3, 5);
		if (ret == HAL_OK) {
			return true;
		} else {
			return false;
		}
	}
	;
};

#endif
