#ifndef __BSP_VL53L0X_H
#define __BSP_VL53L0X_H

#include "vl53l0x_IIC.h"
#include "vl53l0x_def.h"
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"

#define VL_XSHUT_RCC  RCC_AHB1Periph_GPIOA
#define VL_XSHUT_PIN  GPIO_Pin_4
#define VL_XSHUT_IOx  GPIOA  

#define XSHUT_HIGH    GPIO_SetBits(VL_XSHUT_IOx,VL_XSHUT_PIN)
#define XSHUT_LOW     GPIO_ResetBits(VL_XSHUT_IOx,VL_XSHUT_PIN)

VL53L0X_Error VL53L0x_Init(VL53L0X_Dev_t *pDev);
VL53L0X_Error VL53L0x_set_mode(VL53L0X_Dev_t *dev, uint8_t mode);
VL53L0X_Error VL53L1_single_test(VL53L0X_Dev_t *dev,VL53L0X_RangingMeasurementData_t *pdata);

#endif

