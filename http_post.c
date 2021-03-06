#include "util.h"
#include "http_post.h"

// FIXME: Doesn't escape to ensure valid json
int json_string(char *buf, char *name, char *val, int size){
	if (size < 10)
		return 0;	// Not enough space to bother
	// account for 2 added characters (= and &)
	int len = 0;
	buf[len++] = '"';
	len += strlcpy(buf+len, name, size-len);
	len += strlcpy(buf+len, "\":\"", size-len);
	len += strlcpy(buf+len, val, size-len);
	len += strlcpy(buf+len, "\",", size-len);
	return len;
}

int json_num(char *buf, char *name, double val, int size){
	if (size < 8)
		return 0;	// Not enough space to bother
	int len = 0;
	buf[len++] = '"';
	len += strlcpy(buf+len, name, size-len);
	len += strlcpy(buf+len, "\":", size-len);
	len += ftostr(buf+len, val, size-len-1);
	len += strlcpy(buf+len, ",", size-len);
	return len;
}

int http_post(char *buf, char *host, int port, double sun, double temperature, double moisture, int size){
	int len = 0;
	len += strlcpy(buf+len, "POST /", size-len);
	--len;	// Remove the trailing &
	len += strlcpy(buf+len, " HTTP/1.1\nHost: ", size-len);
	len += strlcpy(buf+len, host, size-len);
	len += strlcpy(buf+len, ":", size-len);
	len += itostr(buf+len, port, size-len);
	len += strlcpy(buf+len, "\nUser-Agent: esp32\n", size-len);
	len += strlcpy(buf+len, "Accept: text/html\n", size-len);
	len += strlcpy(buf+len, "Content-Type: application/json\n", size-len);
	len += strlcpy(buf+len, "Content-Length:     \n\n", size-len);
	int body_start = len;	// Keep as a placeholder
	len += strlcpy(buf+len, "{", size-len);
	len += json_string(buf+len, "name","frontyard", size-len);
	len += json_num(buf+len, "sun",sun, size-len );
	len += json_num(buf+len, "heat",temperature, size-len );
	len += json_num(buf+len, "wet",moisture, size-len );
	len += strlcpy(buf+len, "}", size-len);
	int tmp = body_start - 7;
	tmp += itostr(buf+tmp, len-body_start, 5);	// We set aside 5 spaces for the content length
	buf[tmp] = ' ';	// Replace null with space
	
	return len;
}
