all:put get delete watch
put:put.c
	gcc $^ -o $@ -L /usr/lib64 -l curl
get:get.c
	gcc $^ -o $@ -L /usr/lib64 -l curl
delete:delete.c
	gcc $^ -o $@ -L /usr/lib64 -l curl
watch:watch.c
	gcc $^ -o $@ -pthread -L /usr/lib64 -l curl

clean:
	rm -rf put get delete watch
