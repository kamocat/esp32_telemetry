#include "http_get.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main( int argc, char **argv){
	char buf[200];
	int size = sizeof(buf);
	int len;
	
	len = itostr(buf, -123456789, size);
	printf("Converted int to %d-length string: %s\r\n", len, buf);
	
	len = ftostr(buf, 123.456789E-23, size);
	printf("Converted float to %d-length string: %s\r\n", len, buf);
	
	len = 0; // Reset to beginning of string again
	len = stringkey(buf+len, size-len, "label", "testing");
	len += numkey(buf+len, size-len, "temperature", 6.5);
	len += numkey(buf+len, size-len, "light", 14e50);
	len += numkey(buf+len, size-len, "other", 91e-50);

	printf("Made string of length %d:\r\n", len);
	puts(buf);
	
	for( int i = 30; i; --i ){
		numkey(buf, i, "Test", -987.6543212345e-50);
		puts(buf);
	}
	
	for( int i = 20; i>5; --i ){
		numkey(buf, i, "Trim", 12345.678901);
		puts(buf);
	}
		
	return 0;
}
