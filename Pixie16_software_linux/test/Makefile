PLXBASE = ./

CC = g++
CFLAGS = -c -Wall
INCFLAGS = -I../software/app -I../software/sys
LDFLAGS = -lm -ldl libPixie16App.a libPixie16Sys.a $(PLXBASE)PlxApi.a

all: Pixie16Boot Pixie16Test_RevF

Pixie16Boot : Pixie16Boot.o
	$(CC) Pixie16Boot.o -o Pixie16Boot $(LDFLAGS)

Pixie16Test_RevF : Pixie16Test_RevF.o
	$(CC) Pixie16Test_RevF.o -o Pixie16Test_RevF $(LDFLAGS)

Pixie16Boot.o : Pixie16Boot.c
	$(CC) $(CFLAGS) $(INCFLAGS) Pixie16Boot.c

Pixie16Test_RevF.o : Pixie16Test_RevF.c
	$(CC) $(CFLAGS) $(INCFLAGS) Pixie16Test_RevF.c

clean:
	rm -f Pixie16Boot.o Pixie16Test_RevF.o

cleanall:
	rm -f Pixie16Boot.o Pixie16Test_RevF.o Pixie16Boot Pixie16Test_RevF

