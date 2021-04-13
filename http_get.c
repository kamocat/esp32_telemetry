#include "util.h"
#include "http_get.h"

int stringkey(char *buf, char *name, char *val, int size){
	if (size < 6)
		return 0;	// Not enough space to bother
	// account for 2 added characters (= and &)
	int len = strlcpy(buf, name, size-2);
	buf[len++] = '=';
	// Account for remaining added character (&)
	len += strlcpy(buf+len, val, size-1-len);
	buf[len++] = '&'; // replace null byte with equals
	buf[len] = 0; // Null-terminate
	return len;
}
	
	
int numkey(char *buf, char *name, double val, int size){
	if (size < 6)
		return 0;	// Not enough space to bother
	// account for 2 added characters (= and &)
	int len = strlcpy(buf, name, size-2);
	buf[len++] = '=';
	// Account for remaining added character (&)
	len += ftostr(buf+len, val, size-1-len);
	buf[len++] = '&'; // replace null byte with equals
	buf[len] = 0; // Null-terminate
	return len;
}
int header(char *buf, char *host, int port, double sun, double temperature, double moisture, int size){
	int len = 0;
	len += strlcpy(buf+len, "GET /?", size-len);
	len += stringkey(buf+len, "name","frontyard", size-len);
	len += numkey(buf+len, "sun",sun, size-len );
	len += numkey(buf+len, "heat",temperature, size-len );
	len += numkey(buf+len, "wet",moisture, size-len );
	--len;	// Remove the trailing &
	len += strlcpy(buf+len, " HTTP/1.1\nHost: ", size-len);
	len += strlcpy(buf+len, host, size-len);
	len += strlcpy(buf+len, ":", size-len);
	len += itostr(buf+len, port, size-len);
	len += strlcpy(buf+len, "\nUser-Agent: esp32\n", size-len);
	len += strlcpy(buf+len, "Accept: text/html\n\n", size-len);
	return len;
}
	
