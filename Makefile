main: main.cpp column.cpp column.h table.cpp table.h database.cpp database.h
	g++ -g main.cpp database.cpp table.cpp column.cpp -o main -std=c++14

clean:
	del *.exe