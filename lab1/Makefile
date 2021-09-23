.PHONY: all clean

CFLAGS+=-O3 -Wall -pthread

CC=gcc

FILES = transformMat.o \
		utils.o \
		main.o

# FILES = utils.o


DRIVERS = lab1_ruochenj_jiang.2091 \


all: $(DRIVERS)

clean:
	rm -f $(DRIVERS)
	rm -f utils.o main.o

$(DRIVERS): $(FILES)
	$(CC) $(CFLAGS) -o $(DRIVERS) $(FILES)

