#include <vector>

#ifndef TILE_H
#define TILE_H

// TODO make thread-safe w/ mutexes and/or condition variables where appropriate
class Tile
{
    public:
        Tile(unsigned int w, unsigned int h, unsigned int y);
        // TODO
        // ~Tile(); if necessary to free allocated memory (image_sobeled[] ?)
        void * findEdge();
        //std::vector<unsigned char> image_unsobeled;  // Store pixels to-be sobeled.
        //std::vector<unsigned char> image_sobeled;  // Stores final, sobeled pixels
        unsigned char * image_sobeled,
                      * image_unsobeled;
        unsigned int w, h;
        unsigned int y, // y coord " "
                     size; // number of pixels in image

    private:
        unsigned char * inData;
};

#endif // TILE_H
