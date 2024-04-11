all: run

compile:
	g++ -c .\main.cpp -v

link:
	g++ main.o -o main.exe -lsfml-graphics -lsfml-window -lsfml-system

run: compile link
	.\main.exe