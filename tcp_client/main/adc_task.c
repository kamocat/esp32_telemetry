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
#include "adc_task.h"

static char * TAG = "ADC";
const double atten = 1.0 / (4096 * OVERSAMPLE);

void adc_sample_task(void *pvParameters)
{
	adc1_config_width(ADC_WIDTH_BIT_12);	
	adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_6); // measure up to 1750mV
	
	struct QMsg m;
	m.ch1 = 1;
	int i = 0;
	while(1){
		++i;
        int32_t val = 0;
        for(int i = 0; i < OVERSAMPLE; ++i){
		  val += adc1_get_raw(ADC1_CHANNEL_4);
        }
		m.ch1 = val * atten;
		ESP_LOGI(TAG, "Read ADC value %f", m.ch1);
		xQueueSendToBack(http_queue, &m, (TickType_t) 10);
		ESP_LOGI(TAG, "Queued message %d", i);
		vTaskDelay(1000*60*5/ portTICK_PERIOD_MS);	// 5 minutes
	}
	vTaskDelete(NULL);
}
