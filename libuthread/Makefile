# Target library
lib := libuthread.a
CC := gcc
CFLAGS := -Wall -w
targets := libuthread.a
objs := queue.o uthread.o context.o preempt.o


ifneq ($(V),1)
Q = @
endif

all: $(lib)



libuthread.a: queue.o uthread.o context.o preempt.o
	@echo "CC	$@"
	@ar rcs $@ $^

queue.o: queue.c queue.h
	@echo "CC	$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

uthread.o: uthread.c uthread.h
	@echo "CC	$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

context.o: context.c context.h
	@echo "CC	$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

preempt.o: preempt.c preempt.h
	@echo "CC	$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "CLEAN"
	$(Q)rm -f $(targets) $(objs)