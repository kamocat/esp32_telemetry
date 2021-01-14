all: run

test:
	gcc http_get.c test_http_get.c -g -ggdb -o exec_test

run: test
	./exec_test
