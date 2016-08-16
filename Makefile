CC=g++
CFLAGS=-g -std=c++11
FILE=main.cpp adder.cpp multiplier.cpp sys_graph.cpp sys_statistics.cpp

all: quality

quality: 
	$(CC) $(CFLAGS) $(FILE) -o quality

clean:
	rm -rf quality