# Makefile
# Sistemas Operativos, DEI/IST/ULisboa 2017-18

heatSim: main.o matrix2d.o
	$(CC) $(CFLAGS) -pthread -o heatSim main.o matrix2d.o

main.o: main.c matrix2d.h
	$(CC) $(CFLAGS) -c -g main.c

matrix2d.o: matrix2d.c matrix2d.h
	$(CC) $(CFLAGS) -c -g matrix2d.c

clean:
	rm -f *.o heatSim

run:
	./heatSim 10 10.0 10.0 0.0 0.0 50 5 .2 


