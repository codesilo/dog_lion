defalt:all

all:fileman

LD=-lreadline -lhistory  -ltermcap
fileman:fileman.o
	gcc -g $^ -o $@ $(LD)
%.o:%.c
	gcc -c -g $< -o $@
.PHONY:clean
clean:
	-@rm fileman *.o

