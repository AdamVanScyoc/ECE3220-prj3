#include "tile.h"

Tile::Tile(unsigned int w, unsigned int h, unsigned int y)
{
    Tile::w = w;
    Tile::h = h;
    Tile::y = y;
    Tile::size = w*h;
    
    // Resize vector to store the pixels in this tile.

    Tile::image_sobeled = new unsigned char[Tile::size];

    Tile::image_unsobeled = new unsigned char[Tile::size];
}
