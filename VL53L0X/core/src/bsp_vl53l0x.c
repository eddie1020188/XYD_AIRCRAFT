#include "bsp_vl53l0x.h"

extern vu16 Distance;
VL53L0X_DeviceInfo_t vl53l0x_dev_info;

void VL53l0x_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(VL_XSHUT_RCC,ENABLE);
	GPIO_InitStruct.GPIO_Pin = VL_XSHUT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(VL_XSHUT_IOx,&GPIO_InitStruct);	
}

VL53L0X_Error VL53L0x_Init(VL53L0X_Dev_t *pDev)
{
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	VL53L0X_Dev_t *pMyDevice = pDev;
	uint8_t VhvSettings;
	uint8_t PhaseCal;
	uint32_t refSpadCount;
	uint8_t isApertureSpads;
	
	pMyDevice->I2cDevAddr = 0x52;        //I2C地址(上电默认0x52)
	pMyDevice->comms_type = 1;           //I2C通信模式
	pMyDevice->comms_speed_khz = 400;    //I2C通信速率
	
	VL53l0x_GPIO_Init();
	VL53L0X_IIC_Init();
	
	XSHUT_LOW;
	delay_ms(30);
	XSHUT_HIGH;
	delay_ms(30);	

	Status = VL53L0X_DataInit(pMyDevice);//device init
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		printf("datainit  failed!  \r\n");
		return Status;
	}
	
	Status = VL53L0X_StaticInit(pMyDevice);
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		printf("static  init  failed!\r\n");
		return Status;
	}
	
	Status = VL53L0X_PerformRefCalibration(pMyDevice, &VhvSettings, &PhaseCal);
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		printf("static  init  failed!\r\n");
		return Status;
	}
	
	Status = VL53L0X_PerformRefSpadManagement(pMyDevice, &refSpadCount, &isApertureSpads);
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		printf("static  init  failed!\r\n");
		return Status;
	}
	
	Status = VL53L0X_SetDeviceMode(pMyDevice,VL53L0X_DEVICEMODE_SINGLE_RANGING);
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		printf("SetDeviceMode failed!\r\n");
		return Status;
	}	
	
	return Status;
}

//VL53L1X single ranging
//dev: device I2C param struct
//pdata: result struct
VL53L0X_Error VL53L1_single_test(VL53L0X_Dev_t *dev,VL53L0X_RangingMeasurementData_t *pdata)
{
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	u8 isDataReady = 0;
	uint32_t LoopNb=0;
	
	Status = VL53L0X_StartMeasurement(dev);
	if(Status != VL53L0X_ERROR_NONE)
		return Status;
	
	do {
		Status = VL53L0X_GetMeasurementDataReady(dev, &isDataReady);
		if (Status != 0)
			break; /* the error is set */

		if (isDataReady == 1)
			break; /* done note that status == 0 */

		LoopNb++;
		if (LoopNb >= VL53L0X_DEFAULT_MAX_LOOP) {
			Status = VL53L0X_ERROR_TIME_OUT;
			break;
		}

		VL53L0X_PollingDelay(dev);
	} while (1);
	
    if(isDataReady == 1)
	{
		Status = VL53L0X_GetRangingMeasurementData(dev, pdata);
		Distance = pdata->RangeMilliMeter;
	}
	Status = VL53L0X_ClearInterruptMask(dev,0);
	return Status;
}
   
