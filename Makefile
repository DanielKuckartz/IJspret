CC = g++

CompileParms = -c -Wall -Wextra -g -std=c++11

OBJS = main.o Torenveelterm.o

all: IJspret

clean:
			rm -f *.o Torenveelterm

IJspret: $(OBJS)
		$(CC) $(OBJS) -o Torenveelterm

%.o: %.cc $(wildcard *.h)
		$(CC) $(CompileParms) $<
