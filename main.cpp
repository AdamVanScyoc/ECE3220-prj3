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
                std::vector<unsigned char> image_unsobeled,
                std::vector<unsigned char> image_sobeled);

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
    for (unsigned int yy = 0, index = 0; yy  < (int)sqrt(numSquares); yy++)
    {
        for (unsigned int xx = 0; xx < (unsigned int)sqrt(numSquares); xx++)
        {

            // Create vector element representing the current tile.
            tiles.emplace_back((int)ceil((double)(image->bmpWidth / (int)sqrt(numSquares))),
                                  (int)ceil((double)(image->bmpHeight / (int)sqrt(numSquares))),
                                  xx, yy);

            // Copy pixels from this subsection of the main image to this tile.
            // Divide image into 500x500 pixel squares
            unsigned int csx = 0, // x coordinate of current square
                     csy = 0, // y coord of current square
                     ox = 0, // x coordinate of overall image - including offset of last tile
                     oy = 0; // y " "

            // Iterate through each pixel of this subsection of the main image.
            for (csy = yy*500, oy = 0; csy < yy*500 + 500; csy++, oy++)   // TODO this may segfault if final tile is less than 500 pixels tall
                for (csx = xx*500, ox = 0; csx < xx*500 + 500; csx++, ox++) // TODO this may segfault if final tile is less than 500 pixels wide
                {
                    tiles[index].image_unsobeled[ox*oy] = inData[csy*image->bmpWidth + csx];
                    // TODO call to findEdge w/ multithreading here
                    // TODO modify findEdge to accept a vector of pixels (i.e. image_sobeled) as argument)
                   findEdge(tiles[index].w, tiles[index].h, tiles[index].x, tiles[index].y,
                           tiles[index].image_unsobeled, tiles[index].image_sobeled);
                }
        }
    }


    /*
    // TODO [notes from Adam] add logic here to divide the image into AT LEAST n squares*, and pass off 
    // the application of the Sobel Operator to a worker thread for each.
    // Use thread-safe operations like locks on the data in each square
    //
    // *the best way to go about this would be to divide the image into some even-square
    // number of squares (i.e. 1, 4, 9, 16, ...). If for example, the number of threads requested was 5,
    // divide the image into 9 squares by dividing the width into 3 segments, and the heigth into 3 segments
   

    // TODO need condition variables for the threads in the thread pool that are 
    // either available or in use.
    // Divide image into 500x500 pixel squares
    unsigned int csx = 0, // x coordinate of current square
                 csy = 0, // y coord of current square
                 ox = 0, // x coordinate of overall image - including offset of last tile
                 oy = 0, // y " "
                 yize = 0, // size of square (in pixels) in vertical direction
                 xsize = 0; // size of square (in pixels) in horiz. direction
    //unsigned char curSquare[500][500];
    //unsigned char * curSquare = malloc(sizeof(unsigned char)*500*500);
    unsigned char * curSquare = new unsigned char [500*500];
    if (numSquares >= 1)
    {
        // Iterate through squares in Y-direction
        for (unsigned int sy = 0; sy < (unsigned int)ceil((double)(image->bmpHeight/500)); sy++)
        {
            // Iterate through squares in X-direction
            for (unsigned int sx = 0; sx < (unsigned int)ceil((double)(image->bmpWidth/500)); sx++)
            {
                // Zero out temporary current square array
                curSquare = memset(&curSquare[0], 0, 500*500);

                // Iterate row-by-row through the current square
                ysize = ((image->bmpHeight - sy*500) >= 500 ? 500 : image->bmpHeight % 500);
                for (oy = sy*500, csy = 0; oy < ysize; oy++, csy++)
                {
                    // Iterate pixel-by-pixel for the current row
                    xsize =  ((image->bmpWidth - sx*500) >= 500 ? 500 : image->bmpWidth % 500);
                    for (ox = sx*500, csx = 0; ox < xsize; ox++, csx++)
                    {
                        curSquare[csy*csx] = data[oy*ox];
                    }
                }

                inData = memcpy(&inData[0], &curSquare[0], 500*500);

                // TODO implement with a new thread from the thread pool
                // (which is numThreads in size)
                //findEdge(xsize, ysize, sx*500, sy*500);
            }
        }

    }
    delete[] curSquare;
    //free(curSquare);

    */


    //findEdge(image->bmpWidth, image->bmpHeight);
    
    // Write image data passed as argument to a bitmap file
    image->writeGrayBmp(&image_sobeled[0]);
    image_sobeled.clear();
    delete[] data;

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
                std::vector<unsigned char> image_unsobeled,
                std::vector<unsigned char> image_sobeled)
{
    int gradient_X = 0;
    int gradient_Y = 0;
    int value = 0;

    // TODO need to make this thread safe; possibly by changing the global
    // variable inData and/or image_sobeled to a protected member variable of a class
    // that implements thread-safety
    
    // The FOR loop apply Sobel operator
    // to bitmap image data in per-pixel level.

//    for(unsigned int y = starty + 1; y < starty + h-1; ++y)
//        for(unsigned int x = startx + 1; x < startx + w-1; ++x)
    for(unsigned int y = 1; y < h-1; ++y)
        for(unsigned int x = 1; x < w-1; ++x)
        {
            // Compute gradient in +ve x direction
            gradient_X = sobel_x[0][0] * inData[ (x-1) + (y-1) * w ]
                    + sobel_x[0][1] * inData[  x    + (y-1) * w ]
                    + sobel_x[0][2] * inData[ (x+1) + (y-1) * w ]
                    + sobel_x[1][0] * inData[ (x-1) +  y    * w ]
                    + sobel_x[1][1] * inData[  x    +  y    * w ]
                    + sobel_x[1][2] * inData[ (x+1) +  y    * w ]
                    + sobel_x[2][0] * inData[ (x-1) + (y+1) * w ]
                    + sobel_x[2][1] * inData[  x    + (y+1) * w ]
                    + sobel_x[2][2] * inData[ (x+1) + (y+1) * w ];

            // Compute gradient in +ve y direction
            gradient_Y = sobel_y[0][0] * inData[ (x-1) + (y-1) * w ]
                    + sobel_y[0][1] * inData[  x    + (y-1) * w ]
                    + sobel_y[0][2] * inData[ (x+1) + (y-1) * w ]
                    + sobel_y[1][0] * inData[ (x-1) +  y    * w ]
                    + sobel_y[1][1] * inData[  x    +  y    * w ]
                    + sobel_y[1][2] * inData[ (x+1) +  y    * w ]
                    + sobel_y[2][0] * inData[ (x-1) + (y+1) * w ]
                    + sobel_y[2][1] * inData[  x    + (y+1) * w ]
                    + sobel_y[2][2] * inData[ (x+1) + (y+1) * w ];

            value = (int)ceil(sqrt( gradient_X * gradient_X +
                                    gradient_Y * gradient_Y));
            // offset the x and y coordinates of the pixels that were being sobeled
            // with the supplied offset so that we can stitch together the tiles 
            // in the final image
            image_sobeled[ x + y*w ] = 255 - value;
        }
    // Visual Studio requires this to be present; and should not 
    // cause issues for other compilers. 
    // Thanks to Thomas Peters.
    return 0;
}
