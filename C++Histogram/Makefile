all: run

compile:
	g++ -c .\main-parallel.cpp -v -Wall

link:
	g++ main-parallel.o -o main.exe -lsfml-graphics -lsfml-window -lsfml-system

run: compile link
	.\main.exe