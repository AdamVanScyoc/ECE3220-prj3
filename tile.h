#include <vector>
#include <pthread.h>

#ifndef TILE_H
#define TILE_H

// TODO make thread-safe w/ mutexes and/or condition variables where appropriate
class Tile
{
    public:
        Tile(unsigned int w, unsigned int h, unsigned int y);
        void* findEdge(const unsigned int w, // Total width of image
                const unsigned int h, // Total height of image
                unsigned char * imageData,
                unsigned char * imageDataSobeled);

        unsigned char * image_sobeled,
                      * image_unsobeled;
        unsigned int w, h;
        unsigned int y, // y coord " "
                     size; // number of pixels in image
        pthread_t thread;

    private:
        unsigned char * inData;
};

#endif // TILE_H
