# Makefile, versao 02
# Sistemas Operativos, DEI/IST/ULisboa 2017-18

CFLAGS= -g -Wall -pedantic
CC=gcc

heatSim: main.o matrix2d.o
	$(CC) $(CFLAGS) -o heatSim main.o matrix2d.o

main.o: main.c matrix2d.h
	$(CC) $(CFLAGS) -c main.c

matrix2d.o: matrix2d.c matrix2d.h
	$(CC) $(CFLAGS) -c matrix2d.c

clean:
	rm -f *.o heatSim

zip:
	zip heatSim_ex01_solucao.zip main.c matrix2d.c matrix2d.h Makefile

run:
	./heatSim 5 10.0 10.0 0.0 0.0 10

