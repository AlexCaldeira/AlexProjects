# Makefile, versao 3
# Sistemas Operativos, DEI/IST/ULisboa 2017-18

CC       = gcc
CFLAGS   = -g -std=gnu99 -Wall -pedantic -pthread

.PHONY: all clean zip

all: heatSim_p3

heatSim_p3: p3_main.o matrix2d.o 
	$(CC) $(CFLAGS) -o $@ $+

p3_main.o: p3_main.c matrix2d.h 
	$(CC) $(CFLAGS) -o $@ -c $<

matrix2d.o: matrix2d.c matrix2d.h
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -f *.o heatSim_p3

zip: heatSim_p3_solucao.zip

run:
	./heatSim_p3 10 10.0 10.0 10.0 10.0 1000 10 .2 matrix 5

heatSim_p3_solucao.zip: Makefile p3_main.c matrix2d.h  matrix2d.c matrix2d.h 
	zip $@ $+

