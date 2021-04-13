#include "http_get.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int loud_compare(char*a, char*b, int size){
	if(strncmp(a,b,size)){
		printf("Expected \"%s\" but got \"%s\"\r\n", b, a);
		return 1;
	}
	return 0;
}

int test_key(void){
	char buf[200];
	int size = sizeof(buf);
	char * golden;
	int fail = 0;
	
	printf("Testing stringkey and numkey...");
	int len = 0;
	len = stringkey(buf, "label", "testing", size-len);
	golden = "label=testing&";
	fail += loud_compare(buf, golden, size);
	
	len = numkey(buf, "temperature", 6.5, 19);
	golden = "temperature=6.500&";
	fail += loud_compare(buf, golden, size);
	
	if(fail)
		puts("FAIL");
	else
		puts("PASS");
	return fail;
}

int test_header(void){
	char buf[200];
	header(buf, "gosling.local", 9999, 45, 2.6, 0.0023, sizeof(buf));
	puts(buf);
	return 0;
}

int main( int argc, char **argv){
	int fail = 0;
	fail += test_key();
	fail += test_header();
	printf("%d cases failed\r\n", fail);
	
	return 0;
}
