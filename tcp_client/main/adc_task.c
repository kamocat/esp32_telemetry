#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "http_post.h"

static char * TAG = "ADC";

void adc_sample_task(void *pvParameters)
{
	struct QMsg m;
	m.ch1 = 1;
	int i = 0;
	while(1){
		++i;
		m.ch1 += 1;
		xQueueSendToBack(http_queue, &m, (TickType_t) 10);
		ESP_LOGI(TAG, "Queued message %d", i);
		vTaskDelay(10000 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}
