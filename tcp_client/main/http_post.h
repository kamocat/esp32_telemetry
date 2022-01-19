#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define LOCATION_ID 20

extern QueueHandle_t http_queue;

struct QMsg{
	double ch1;
};

int json_string(char *buf, char *name, char *val, int size);
int json_num(char *buf, char *name, double val, int size);
int http_post(char *buf, char *host, int port, 
	struct QMsg *msg, int size);

