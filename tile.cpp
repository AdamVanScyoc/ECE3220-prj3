#include "tile.h"

Tile::Tile(unsigned int w, unsigned int h, unsigned int x, unsigned int y)
{
    Tile::w = w;
    Tile::h = h;
    Tile::x = x;
    Tile::y = y;
    Tile::size = w*h;
    
    // Resize vector to store the pixels in this tile.

    Tile::image_sobeled.resize( Tile::size, 255);
    Tile::image_unsobeled.resize( Tile::size, 255);
}
