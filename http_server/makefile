all: main

main: main.o
	g++ -o main main.o -lssl -lcrypto

main.o: main.cpp httplib.h
	g++ -c main.cpp

rebuild: clean main

clean:
	rm main main.o