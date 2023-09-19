GCC = gcc
CFLAGS = -g -Wall -Wshadow

SRCS = master.c slave.c

%: %.c
	$(GCC) $(CFLAGS) $< -o $@ -lrt

all: master slave

clean:
	rm -f master slave cstest logfile.*
