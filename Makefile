main: main.cpp column.cpp column.h table.cpp table.h database.cpp database.h math_cal.cpp math_cal.h
	g++ -g main.cpp database.cpp table.cpp column.cpp math_cal.cpp -o main -std=c++14

clean:
	del *.exe