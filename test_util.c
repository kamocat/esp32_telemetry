#include "util.h"
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

int test_itostr(void){
	char buf[50];
	int size = sizeof(buf);
	int len;
	int fail = 0;
	char * golden; 	// Intended result of test
	
	printf("Testing itostr()...");
	len = itostr(buf, -123456789, size);
	golden = "-123456789";
	fail += loud_compare(buf, golden, size);
	
	len = itostr(buf, 0, size);
	golden = "0";
	fail += loud_compare(buf, golden, size);
	
	len = itostr(buf, 876543210, 5);
	golden = "8765";
	fail += loud_compare(buf, golden, size);
	
	if(fail)
		puts("FAIL");
	else
		puts("PASS");
	return fail;
}

int test_ftostr(void){
	char buf[50];
	int size = sizeof(buf);
	int len;
	int fail = 0;
	char * golden;
	
	printf("Testing ftostr()...");
	len = ftostr(buf, 0.0, size);
	golden = "0.0";
	fail += loud_compare(buf, golden, size);
	
	len = ftostr(buf, 123.456789E-23, size);
	golden = "1.234567889E-21"; // Slightly different due to representation issues
	fail += loud_compare(buf, golden, size);
	
	len = ftostr(buf, 857E56, 9);
	golden = "85.70E57";
	fail += loud_compare(buf, golden, size);
	
	len = ftostr(buf, 1.359, 6);
	golden = "1.359";
	fail += loud_compare(buf, golden, size);

	if(fail)
		puts("FAIL");
	else
		puts("PASS");
	return fail;
}

int main( int argc, char **argv){
	int fail = 0;
	fail += test_itostr();
	fail += test_ftostr();
	printf("%d cases failed\r\n", fail);
	
	return 0;
}
