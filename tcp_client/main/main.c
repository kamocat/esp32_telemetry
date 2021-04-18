/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "tcp_task.h"
#include "http_post.h"
#include "adc_task.h"

// Declared extern in http_post.h
QueueHandle_t http_queue;

void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	
	http_queue = xQueueCreate(2, sizeof( struct QMsg ) );
	if( 0 == http_queue ){
		ESP_LOGD("MAIN", "Failed to create http queue");
	}

	xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
	xTaskCreate(adc_sample_task, "adc", 4096, NULL, 4, NULL);
}
