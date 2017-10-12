#
#
#
#
target  = test.out
sources = $(shell find ./ -name "*.c")
objects = $(subst .c,.o,$(sources))
CC = gcc
cflag = -g -o

$(target):$(objects)
	$(CC) $(cflag) $@ $^

.PHONY:clean cleandist
clean:
	rm -rf $(objects)
cleandist:
	rm -rf $(objects) $(target)
