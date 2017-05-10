#include "vTaskMagMeasure.h"
#include "cmsis_os.h"
#include "usart.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueMagMeasureDataHandle;	/* save data */
extern osMessageQId xQueueRunTokenMagMeasureHandle;	/* execute token */
/* Semaphore */

void vTaskMagMeasure(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Mag Measure Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		(unsigned char *)pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
#endif
	
#pragma pack(1)
	static Mag_Measure_Data xCurrentMagMeasureData = {0};
#pragma pack()
	mpu_init();
	mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);mpu_set_sample_rate(DEFAULT_MPU_HZ);
	dmp_load_motion_driver_firmware();
	dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
	dmp_enable_feature(hal.dmp_features);
	dmp_set_fifo_rate(DEFAULT_MPU_HZ);                        
	run_self_test();
	mpu_set_dmp_state(1);
	/* Infinite loop */
	for (;;)
	{
		osMessagePeek(xQueueRunTokenMagMeasureHandle, osWaitForever);
		/* ----------------------------------------------------------------------------- */
		/* add codes here */
		
		/* ----------------------------------------------------------------------------- */
#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);	
		
		#ifdef _DebugTaskMagMeasure
		osDelay(random(5000));
		osMessagePut(xQueueMagMeasureDataHandle, (uint32_t)&xCurrentMagMeasureData, osWaitForever);
		#endif
#endif
		osThreadYield();
	}
}
