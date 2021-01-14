#include "http_get.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main( int argc, char **argv){
	char buf[200];
	int size_rem = sizeof(buf);
	int len;
	
	len = itostr(buf, -123456789, size_rem);
	printf("Converted int to %d-length string: %s\r\n", len, buf);
	
	len = ftostr(buf, 123.456789E-23, size_rem);
	printf("Converted float to %d-length string: %s\r\n", len, buf);
	
	char * ptr = stringkey(buf, &size_rem, "label", "testing");
	ptr = numkey(ptr, &size_rem, "temperature", 6.5);
	ptr = numkey(ptr, &size_rem, "light", 14e50);
	ptr = numkey(ptr, &size_rem, "other", 91e-50);

	len = strnlen(buf, sizeof(buf));
	printf("Made string of length %d with %d bytes remaining:\r\n", len, size_rem);
	puts(buf);
	
	for( int i = 30; i; --i ){
		int j = i;
		numkey(buf, &j, "Test", -987.6543212345e-50);
		puts(buf);
	}
	
	for( int i = 20; i>5; --i ){
		int j = i;
		numkey(buf, &j, "Trim", 12345.678901);
		puts(buf);
	}
		
	return 0;
}
