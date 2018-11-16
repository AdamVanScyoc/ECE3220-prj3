//Name:         
//Instructor:   Brygg Ullmer
//Class:        3220
//Date:         

// Base code by Bidur Bohara (LSU) in collaboration with Brygg Ullmer

//#include <QCoreApplication>
#include <vector>
#include <math.h>
#include <thread>

#include "cscbitmap.h"
#include "tile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Wrapper for tile:FindEdge() so that it can be passed as a function pointer to pthread_create
void * findEdgeWrapper(Tile t);

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
                                (unsigned int)ceil(((double)image->bmpHeight)/((double)numThreads)),
                                  yy);

            // Copy pixels from this subsection of the main image to this tile.
            unsigned int csy = 0; // y coord of current square

            // Iterate through each line of this subsection of the main image.
            for (csy = 0; csy < tiles[index].h; csy++)
            {
                // TODO change 640
                if (csy == 0) 
                    memcpy(&(tiles[index].image_unsobeled[0]), &data[0 + yy*tiles[index].h*tiles[index].w], 640);
                else
                    memcpy(&(tiles[index].image_unsobeled[csy*640]), &data[csy*640 + yy*tiles[index].h*tiles[index].w], 640);
            }
          
           // TODO call to findEdge w/ multithreading here
           //tiles[index].findEdge();
           //tiles[index].newThread(tiles[index].image_unsobeled, tiles[index].image_unsobeled);
           tiles[index].newThread(&tiles[index]);
           

           // Add sobeled horizontal slice to the main output image
           // TODO error check pthread_create
            memcpy(&outData[tiles[index].y * (tiles[index].h - 1) * tiles[index].w], &tiles[index].image_sobeled[0], tiles[index].h*tiles[index].w);
    }

    image->writeGrayBmp(outData);

    return 0;
}

