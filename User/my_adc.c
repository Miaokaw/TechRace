#include "my_adc.h"

uint16_t AD_Buf[8] = {0}; // 两个通道采集数据存在这个数组里面

void MY_ADC_Init(void){
	HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&AD_Buf, 8); // �?????启ADC的DMA，采集的数据直接放入 AD_Buf这个数组里，操作�?????单�??
}
