CC = gcc
CFLAGS = -Wall -g
OBJS = main.o simulador.o processo.o memoria.o tlb.o

all: simulador

simulador: $(OBJS)
	$(CC) $(CFLAGS) -o simulador $(OBJS)

clean:
	rm -f *.o simulador
