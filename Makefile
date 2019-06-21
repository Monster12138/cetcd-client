all:put get 
put:put.c
	gcc $^ -o $@ -L /usr/lib64 -l curl
get:get.c
	gcc $^ -o $@ -L /usr/lib64 -l curl
clean:
	rm -rf put get

