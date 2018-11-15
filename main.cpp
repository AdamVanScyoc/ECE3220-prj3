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
                const unsigned int x, // x starting point 
                const unsigned int y, // y starting point
                unsigned char * imageData,
                unsigned char * imageDataSobeled);
                //std::vector<unsigned char> image_unsobeled,
                //std::vector<unsigned char> image_sobeled);

/// Memory to hold input image data
unsigned char* inData;
unsigned char* outData;
std::vector<unsigned char> image_sobeled;

// Qt entry point!
// Create and run a thread inside main function.
// Also assign region of image to a thread, and 
// call thread join for synchronization.
int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    int numThreads = 1; // TODO As-is, this expects that the 2nd argument will always
                        // be the thread count (not the filename). It does no error checking
    int numSquares = 1;
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

    image_sobeled.clear();
    image_sobeled.resize(image->bmpSize, 255);


    inData = data;

    // Compute the smallest perfect square that is as-large-as, or just larger than, 
    // the number of requested threads.
    numSquares = (int)pow(ceil(sqrt((double)numThreads)), 2);

    // Create vector of numSquares number of tiles.
    std::vector<class Tile> tiles;

    unsigned char * image_unsobeled = new unsigned char[640*625];

    for (unsigned int yy = 0, index = 0; yy  < 1/*(unsigned int)sqrt(numSquares)*/; yy++, index++)
    {
        for (unsigned int xx = 0; xx < 1/*(unsigned int)sqrt(numSquares)*/; xx++)
        {

            // Create vector element representing the current tile.
           tiles.emplace_back((int)ceil((double)(image->bmpWidth / (int)sqrt(numSquares))),
                                  (int)ceil((double)(image->bmpHeight / (int)sqrt(numSquares))),
                                  xx, yy);
           tiles[index] = Tile(640, 625, xx, yy);

            // Copy pixels from this subsection of the main image to this tile.
            // Divide image into 500x500 pixel squares
            unsigned int csx = 0, // x coordinate of current square
                     csy = 0, // y coord of current square
                     ox = 0, // x coordinate of overall image - including offset of last tile
                     oy = 0; // y " "

            // Iterate through each pixel of this subsection of the main image.
            for (csy = 0; csy < 500; csy++)
            {
               //memcpy(&(tiles[index].image_unsobeled[csy*500]), &inData[csy*image->bmpWidth], 500);
                //memcpy(&image_unsobeled[csy*640], &inData[csy*640], 640);
                memcpy(&(tiles[index].image_unsobeled[csy*640]), &inData[csy*640], 640);
               
            }
            /*
            for (csy = yy*500, oy = 0; csy < yy*500 + 500; csy++, oy++)   // TODO this may segfault if final tile is less than 500 pixels tall
                // Copy sub-section of image line-by-line
               memcpy(&tiles[index].image_unsobeled[oy], &inData[csy], 500);



                for (csx = xx*500, ox = 0; csx < xx*500 + 500; csx++, ox++) // TODO this may segfault if final tile is less than 500 pixels wide
                {
                    // ox is the pixel x-coord relative to the tile
                    // csx is the pixel y-coord relative to the whole image
                    tiles[index].image_unsobeled[ox + oy*500] = inData[csy*image->bmpWidth + csx];
                }




            */

           
           // TODO call to findEdge w/ multithreading here
           // TODO modify findEdge to accept a vector of pixels (i.e. image_sobeled) as argument)
           //tiles[index].image_sobeled.clear();
           //findEdge(tiles[index].w, tiles[index].h, tiles[index].x, tiles[index].y,
           //        &tiles[index].image_unsobeled[0], &tiles[index].image_sobeled[0]);

            // Write image data passed as argument to a bitmap file
           image->writeGrayBmp(&tiles[index].image_unsobeled[0]);
           //image->writeGrayBmp(&image_unsobeled[0]);
        }
    }

    //findEdge(image->bmpWidth, image->bmpHeight);
    
    //image->writeGrayBmp(&image_unsobeled[0]);

    return 0;
    //return a.exec();
}


// TODO make findEdge work on an arbitrary tile of a larger image
// TODO make findEdge thread-safe
/// Function that implements Sobel operator.
/// Returns image data after applying Sobel operator to the original image.
/// Reimplement findEdge such that it will run in a single thread
/// and can process on a region/group of pixels
void* findEdge(const unsigned int w, // Total width of image
                const unsigned int h, // Total height of image
                const unsigned int x,
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
