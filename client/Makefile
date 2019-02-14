CC = g++
CFLAGS = -fpermissive -O2 -Wall -I . `pkg-config --cflags opencv`


LIB = -lpthread `pkg-config --libs opencv`

all: client

client: client.cpp csapp.o
	$(CC) $(CFLAGS) -o client client.cpp csapp.o $(LIB)

csapp.o: csapp.cpp
	$(CC) $(CFLAGS) -c csapp.cpp

clean:
	rm client
