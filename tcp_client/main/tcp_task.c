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
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "addr_from_stdin.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "mdns.h"
#include "netdb.h"
#include "http_post.h"

#define PORT 7080
#define HOSTNAME "homebase"
#define HOST_IP "192.168.1.110"

static const char *TAG = "tcp_client";

int tcp_send(struct esp_ip4_addr addr, char * msg, char * rx_buffer, int rx_len){
	struct sockaddr_in dest_addr;
	dest_addr.sin_addr.s_addr = addr.addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	int sock =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sock < 0) {
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		shutdown(sock, 0);
		close(sock);
		return 0;
	}
	ESP_LOGI(TAG, "Socket created, connecting to %d.%d.%d.%d", IP2STR(&addr));
	int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
	if (err != 0) {
		ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
		shutdown(sock, 0);
		close(sock);
		return 0;
	}
	ESP_LOGI(TAG, "Successfully connected");
	err = send(sock, msg, strlen(msg), 0);
	if (err < 0) {
		ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
		shutdown(sock, 0);
		close(sock);
		return 0;
	}

	int len = recv(sock, rx_buffer, rx_len- 1, 0);
	// Error occurred during receiving
	if (len < 0) {
		ESP_LOGE(TAG, "recv failed: errno %d", errno);
		shutdown(sock, 0);
		close(sock);
		return 0;
	}
	// Data received
	else {
		rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
		ESP_LOGI(TAG, "Received %d bytes", len);
		ESP_LOGI(TAG, "%s", rx_buffer);
	}
	if (sock != -1) {
		ESP_LOGE(TAG, "Shutting down socket...");
		shutdown(sock, 0);
		close(sock);
	}
	return len;
}

void tcp_client_task(void *pvParameters)
{
	char rx_buffer[128];
	char * host_name = HOSTNAME;
	ESP_ERROR_CHECK( mdns_init() );

	/* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
	 * Read "Establishing Wi-Fi or Ethernet Connection" section in
	 * examples/protocols/README.md for more information about this function.
	 */
	ESP_ERROR_CHECK(example_connect());
	while(1){
		ESP_LOGI(TAG, "Looking for %s", host_name);
		struct esp_ip4_addr addr;
		addr.addr = 0;
#ifdef HOST_IP
        if(1){
          inet_pton(AF_INET, HOST_IP, &addr);            
#else
		esp_err_t err = mdns_query_a(host_name, 2000,  &addr);
		if(err){
			if(err == ESP_ERR_NOT_FOUND){
				ESP_LOGW(TAG, "%s: Host was not found!", esp_err_to_name(err));
			} else {
				ESP_LOGE(TAG, "Query Failed: %s", esp_err_to_name(err));
			}
		} else {
			ESP_LOGI(TAG, "Query A: %s.local resolved to: " IPSTR, host_name, IP2STR(&addr));
#endif
			while (1) {
				char tx_buffer[300];
				struct QMsg m;
				if( !xQueueReceive(http_queue, &m, 1000*60*10 / portTICK_PERIOD_MS)){
					ESP_LOGI(TAG, "Queue timed out");
					break; // Took too long. Maybe turn off wifi for power save?
				}
				http_post(tx_buffer, host_name, PORT, &m, sizeof(tx_buffer));
				ESP_LOGI(TAG, "Built HTTP message");
				tcp_send(addr, tx_buffer, rx_buffer, sizeof(rx_buffer));
				ESP_LOGI(TAG, "RECEIVED: %s", rx_buffer);
			}
		}
	}

	vTaskDelete(NULL);
}
