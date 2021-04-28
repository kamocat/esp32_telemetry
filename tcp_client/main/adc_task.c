#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "http_post.h"
#include "driver/adc.h"

static char * TAG = "ADC";
const double atten = 1.0 / 4096;

void adc_sample_task(void *pvParameters)
{
	adc1_config_width(ADC_WIDTH_BIT_12);	
	adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_11); // measure up to 2600mV
	adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_11);
	adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_11);
	adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_11);
	
	struct QMsg m;
	m.ch1 = 1;
	int i = 0;
	while(1){
		++i;
		int val = adc1_get_raw(ADC1_CHANNEL_4);
		m.ch1 = val * atten;
		val = adc1_get_raw(ADC1_CHANNEL_5);
		m.ch2 = val * atten;
		val = adc1_get_raw(ADC1_CHANNEL_6);
		m.ch3 = val * atten;
		val = adc1_get_raw(ADC1_CHANNEL_7);
		m.ch4 = val * atten;
		ESP_LOGI(TAG, "Read ADC value %f", m.ch1);
		xQueueSendToBack(http_queue, &m, (TickType_t) 10);
		ESP_LOGI(TAG, "Queued message %d", i);
		vTaskDelay(1000*60*5/ portTICK_PERIOD_MS);	// 5 minutes
	}
	vTaskDelete(NULL);
}
