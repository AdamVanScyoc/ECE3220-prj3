pr3 : main.cpp cscbitmap.cpp cscbitmap.h 
	g++ -o pr3 main.cpp cscbitmap.cpp cscbitmap.h -g -lm

clean :
	rm -rf pr3 1.bmp
