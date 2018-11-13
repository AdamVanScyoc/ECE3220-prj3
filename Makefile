pr3 : main.cpp cscbitmap.cpp cscbitmap.h tile.cpp
	g++ -o pr3 main.cpp cscbitmap.cpp tile.cpp  -g -lm -Wall -fpermissive -std=c++11

clean :
	rm -rf pr3 1.bmp
