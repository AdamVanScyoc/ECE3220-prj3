pr3 : main.cpp cscbitmap.cpp cscbitmap.h 
	g++ -o pr3 main.cpp cscbitmap.cpp cscbitmap.h -g

clean :
	rm -rf pr3 1.bmp
