int json_string(char *buf, char *name, char *val, int size);
int json_num(char *buf, char *name, double val, int size);
int http_post(char *buf, char *host, int port, 
	double sun, double temperature, double moisture, int size);

