all: run

util:
	gcc util.c test_util.c -g -ggdb -o test_util
	./test_util

get:
	gcc util.c http_get.c test_http_get.c -g -ggdb -o test_get
	./test_get

post:
	gcc util.c http_post.c test_http_post.c -g -ggdb -o test_post
	./test_post

run: util get post 

clean:
	rm -rf test_util test_get test_post
