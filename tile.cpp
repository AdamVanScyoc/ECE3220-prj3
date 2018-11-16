#include "tile.h"
#include "cscbitmap.h"

#include <pthread.h>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sobel_x[3][3] = { { 1, 0,-1},
                      { 2, 0,-2},
                      { 1, 0,-1}};

int sobel_y[3][3] = { { 1, 2, 1},
                      { 0, 0, 0},
                      {-1,-2,-1}};

Tile::Tile(unsigned int w, unsigned int h, unsigned int y)
{
    Tile::w = w;
    Tile::h = h;
    Tile::y = y;        // relative y-coord of this tile (relative to main image)
    Tile::size = w*h;
    
    Tile::image_sobeled = new unsigned char[Tile::size];
    Tile::image_unsobeled = new unsigned char[Tile::size];
}
//
// TODO make findEdge thread-safe
/// Function that implements Sobel operator.
/// Returns image data after applying Sobel operator to the original image.
/// Reimplement findEdge such that it will run in a single thread
/// and can process on a region/group of pixels
void* Tile::findEdge(const unsigned int w, // Total width of image
                const unsigned int h, // Total height of image
                unsigned char * imageData,
                unsigned char * imageDataSobeled)
{
    int gradient_X = 0;
    int gradient_Y = 0;
    int value = 0;

    // TODO need to make this thread safe; possibly by changing the global
    // variable imageData and/or imageDataSobeled to a protected member variable of a class
    // that implements thread-safety
    
    // The FOR loop apply Sobel operator
    // to bitmap image data in per-pixel level.
    for(unsigned int y = 1; y < h-1; ++y)
        for(unsigned int x = 1; x < w-1; ++x)
        {
            // Compute gradient in +ve x direction
            gradient_X = sobel_x[0][0] * imageData[ (x-1) + (y-1) * w ]
                    + sobel_x[0][1] * imageData[  x    + (y-1) * w ]
                    + sobel_x[0][2] * imageData[ (x+1) + (y-1) * w ]
                    + sobel_x[1][0] * imageData[ (x-1) +  y    * w ]
                    + sobel_x[1][1] * imageData[  x    +  y    * w ]
                    + sobel_x[1][2] * imageData[ (x+1) +  y    * w ]
                    + sobel_x[2][0] * imageData[ (x-1) + (y+1) * w ]
                    + sobel_x[2][1] * imageData[  x    + (y+1) * w ]
                    + sobel_x[2][2] * imageData[ (x+1) + (y+1) * w ];

            // Compute gradient in +ve y direction
            gradient_Y = sobel_y[0][0] * imageData[ (x-1) + (y-1) * w ]
                    + sobel_y[0][1] * imageData[  x    + (y-1) * w ]
                    + sobel_y[0][2] * imageData[ (x+1) + (y-1) * w ]
                    + sobel_y[1][0] * imageData[ (x-1) +  y    * w ]
                    + sobel_y[1][1] * imageData[  x    +  y    * w ]
                    + sobel_y[1][2] * imageData[ (x+1) +  y    * w ]
                    + sobel_y[2][0] * imageData[ (x-1) + (y+1) * w ]
                    + sobel_y[2][1] * imageData[  x    + (y+1) * w ]
                    + sobel_y[2][2] * imageData[ (x+1) + (y+1) * w ];

            value = (int)ceil(sqrt( gradient_X * gradient_X +
                                    gradient_Y * gradient_Y));
            // offset the x and y coordinates of the pixels that were being sobeled
            // with the supplied offset so that we can stitch together the tiles 
            // in the final image
            imageDataSobeled[ x + y*w ] = 255 - value;
        }
    // Visual Studio requires this to be present; and should not 
    // cause issues for other compilers. 
    // Thanks to Thomas Peters.
    return 0;
}
