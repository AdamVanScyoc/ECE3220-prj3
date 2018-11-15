//Name:         
//Instructor:   Brygg Ullmer
//Class:        3220
//Date:         

// Base code by Bidur Bohara (LSU) in collaboration with Brygg Ullmer

//#include <QCoreApplication>
#include <vector>
#include <math.h>

#include "cscbitmap.h"
#include "tile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sobel_x[3][3] = { { 1, 0,-1},
                      { 2, 0,-2},
                      { 1, 0,-1}};

int sobel_y[3][3] = { { 1, 2, 1},
                      { 0, 0, 0},
                      {-1,-2,-1}};

/// Declaration of functions.
void* findEdge(const unsigned int w, // Total width of image
                const unsigned int h, // Total height of image
                const unsigned int y, // y starting point
                unsigned char * imageData,
                unsigned char * imageDataSobeled);

// Qt entry point!
// Create and run a thread inside main function.
// Also assign region of image to a thread, and 
// call thread join for synchronization.
int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    unsigned int numThreads = 1; // TODO As-is, this expects that the 2nd argument will always
                        // be the thread count (not the filename). It does no error checking
    char* bmpFile;
    if( argc < 3)
    {
        printf("Filename and Thread Count arguments required!\n");
        return 0;
    }
    else
    {
        bmpFile = argv[1]; 
        numThreads = atoi(argv[2]);
    }
    
    /// Open and read bmp file.
    Bitmap *image = new Bitmap();
    unsigned char*data = image->readGrayBitmap(bmpFile);
    // Memory to hold output image data
    unsigned char* outData = new unsigned char[image->bmpSize];


    std::vector<class Tile> tiles;

    for (unsigned int yy = 0, index = 0; yy  < numThreads; yy++, index++)
    {
            // Create vector element representing the current tile.
           tiles.emplace_back(image->bmpWidth,
                                  (int)ceil((double)(image->bmpHeight / (int)numThreads)),
                                  yy);

            // Copy pixels from this subsection of the main image to this tile.
            unsigned int csy = 0; // y coord of current square

            // Iterate through each line of this subsection of the main image.
            for (csy = 0; csy < tiles[index].h; csy++)
            {
                // TODO change 640
                memcpy(&(tiles[index].image_unsobeled[csy*640]), &data[csy*640], 640);
               
            }
          
           // TODO call to findEdge w/ multithreading here
           // TODO modify findEdge to accept a vector of pixels (i.e. image_sobeled) as argument)
           tiles[index].image_sobeled = (unsigned char *)memset(&tiles[index].image_sobeled[0], 0, tiles[index].size);
           findEdge(tiles[index].w, tiles[index].h, tiles[index].y,
                   &tiles[index].image_unsobeled[0], &tiles[index].image_sobeled[0]);

           // Add sobeled horizontal slice to the main output image
           outData = (unsigned char *) memcpy(&outData[(tiles[index].y - 0) * tiles[index].h * tiles[index].w], &tiles[index].image_sobeled[0], tiles[index].h*tiles[index].w);

    }

    for (unsigned int index = 0; index < numThreads; index++)
    {
        // Stitch horizontal slices together
        // Write image data passed as argument to a bitmap file
        image->writeGrayBmp(tiles[index].image_sobeled);
    }

    return 0;
}


// TODO make findEdge work on an arbitrary tile of a larger image
// TODO make findEdge thread-safe
/// Function that implements Sobel operator.
/// Returns image data after applying Sobel operator to the original image.
/// Reimplement findEdge such that it will run in a single thread
/// and can process on a region/group of pixels
void* findEdge(const unsigned int w, // Total width of image
                const unsigned int h, // Total height of image
                const unsigned int y,
                unsigned char * imageData,
                unsigned char * imageDataSobeled)
                //std::vector<unsigned char> image_unsobeled,
                //std::vector<unsigned char> imageDataSobeled)
{
    int gradient_X = 0;
    int gradient_Y = 0;
    int value = 0;

    // TODO need to make this thread safe; possibly by changing the global
    // variable imageData and/or imageDataSobeled to a protected member variable of a class
    // that implements thread-safety
    
    // The FOR loop apply Sobel operator
    // to bitmap image data in per-pixel level.

//    for(unsigned int y = starty + 1; y < starty + h-1; ++y)
//        for(unsigned int x = startx + 1; x < startx + w-1; ++x)
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
