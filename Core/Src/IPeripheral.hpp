/***************************************************************************
 *                                                                         *
 *   Copyright (C) Jan Mannsbart	                                   	     *
 *                                                                         *
 ***************************************************************************/

#pragma once

namespace ePeripheralType
{
    enum ePeripheralType
    {
        I2C_DEVICE,
        SPI_DEVICE,
        OTHER,
        NOT_SET
    };
}

namespace ePeripheralError
{
    enum ePeripheralError
    {
        NO_ERROR,
        ERROR_NOT_INITIALIZED,
        ERROR_DEVICE_ADR_NOT_FOUND,
        ERROR_WHILE_INITIALIZING
    };
}

namespace ePeripheralEventMajor
{
    enum ePeripheralEventMajor
    {
        EVENT_PERIPHERAL,
        EVENT_DATA,
        NO_EVENT = 0xFF
    };
}

class IPeripheral
{
public:
    virtual ePeripheralType::ePeripheralType GetPeripheralType() = 0;
    virtual ePeripheralError::ePeripheralError InitDevice() = 0;
    virtual bool IsDeviceInitialized() = 0;
};
