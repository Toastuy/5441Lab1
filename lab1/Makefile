.PHONY: all clean

CFLAGS+=-O3 -g -Wall -pthread
#CXXFLAGS+=-g -O0 -Wall

CC=gcc

FILES = transformMat.o \
		utils.o

# FILES = utils.o


DRIVERS = main \

DRIVER_OBJS = $(DRIVERS)

all: $(DRIVERS)

clean:
	rm -f $(DRIVER_OBJS)
	rm -f utils.o

$(DRIVER_OBJS): $(FILES)

