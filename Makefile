LIBCJSON_DIR=./cJSON/lib
LIBCURL_DIR=/usr/lib64
all:put get delete watch
put:put.c
	gcc $^ -o $@ -L$LIBCJSON_DIR -lcjson -L$LIBCURL_DIR -lcurl 
get:get.c
	gcc $^ -o $@ -L$LIBCJSON_DIR -lcjson -L$LIBCURL_DIR -lcurl 
delete:delete.c
	gcc $^ -o $@ -L$LIBCJSON_DIR -lcjson -L$LIBCURL_DIR -lcurl 
watch:watch.c
	gcc $^ -o $@ -pthread -L$LIBCJSON_DIR -lcjson -L$LIBCURL_DIR -lcurl 

clean:
	rm -rf put get delete watch
