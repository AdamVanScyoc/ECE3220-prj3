#include <vector>
#include <pthread.h>

#ifndef TILE_H
#define TILE_H

// TODO make thread-safe w/ mutexes and/or condition variables where appropriate
class Tile
{
    public:
        Tile(unsigned int w, unsigned int h, unsigned int y);
        Tile(unsigned int w, unsigned int h, unsigned int y, unsigned char * image_unsobeled, unsigned char * image_sobeled);
        int newThread(unsigned char * image_unsobeled, unsigned char * image_sobeled);
        //void* findEdge();
        int newThread(Tile * t);

        unsigned char * image_sobeled,
                      * image_unsobeled;
        unsigned int w, h;
        unsigned int y, // y coord " "
                     size; // number of pixels in image

    private:
        unsigned char * inData;
};

#endif // TILE_H
