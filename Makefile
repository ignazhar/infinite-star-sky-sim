all: compile link execute

compile:
	g++ -c main.cpp

link:
	g++ main.o events.cpp -o main -lsfml-window -lsfml-system -lsfml-graphics -lmingw32

execute:
	./main.exe