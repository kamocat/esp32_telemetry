#include "freertos/task.h"
#include "lwip/sockets.h"
int tcp_send(struct esp_ip4_addr addr, char * msg, char * rx_buffer, int rx_len);
void tcp_client_task(void *pvParameters);
