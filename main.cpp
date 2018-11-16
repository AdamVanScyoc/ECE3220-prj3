//Name:			Willy Li and Adam Van Scyoc
//Instructor:   Brygg Ullmer
//Class:        3220
//Date:			11/16/2018

// Base code by Bidur Bohara (LSU) in collaboration with Brygg Ullmer

//#include <QCoreApplication>
#include <vector>
#include <math.h>

#include "cscbitmap.h"
#include "tile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

#include <thread>

#include <ctime>

int sobel_x[3][3] = { { 1, 0, -1},
	{ 2, 0, -2},
	{ 1, 0, -1}
};

int sobel_y[3][3] = { { 1, 2, 1},
	{ 0, 0, 0},
	{ -1, -2, -1}
};

/// Declaration of functions.
void* findEdge(const unsigned int w, // Total width of image
               const unsigned int h, // Total height of image
               unsigned char * imageData,
               unsigned char * imageDataSobeled);

int processImage(const char* bmpFile, unsigned int numThreads);

// Qt entry point!
// Create and run a thread inside main function.
// Also assign region of image to a thread, and
// call thread join for synchronization.
int main(int argc, char *argv[])
{
	std::clock_t start, end;
	double duration;
	unsigned int numThreads;
	const char* bmpFile;
	if ( argc < 3) {
		printf("Usage: %s [file] [number of threads]\n", argv[0]);
		return 0;
	}
	char* inFile = argv[1];
	numThreads = atoi(argv[2]);
	if (numThreads < 1) {
		numThreads = 1;
	}
	char* ext = strrchr(inFile, '.');
	if (strcmp(ext, ".bmp") == 0) {
		start = std::clock();
		bmpFile = inFile;
		processImage(bmpFile, numThreads);
		end = std::clock();
		duration = (end - start) / (double)CLOCKS_PER_SEC;
		printf("It took %f seconds with %d threads\n", duration, numThreads);
	} else if (strcmp(ext, ".txt") == 0) {
		start = std::clock();
		std::string filename;
		std::ifstream ifs(inFile);
		std::thread t[numThreads];
		while (getline(ifs, filename)) {
			processImage(filename.c_str(), numThreads);
		}
		end = std::clock();
		duration = (end - start) / (double)CLOCKS_PER_SEC;
		printf("It took %f seconds with %d threads\n", duration, numThreads);
	} else {
		printf("Expected .txt or .bmp file");
		return 0;
	}
	return 0;
}

int processImage(const char* bmpFile, unsigned int numThreads) {

	/// Open and read bmp file.
	Bitmap *image = new Bitmap();
	unsigned char*data = image->readGrayBitmap(bmpFile);
	// Memory to hold output image data
	unsigned char* outData = new unsigned char[image->bmpSize];
	std::string outFilename = bmpFile;
	std::string suffix = "_processed";
	outFilename.insert(outFilename.length() - 4, suffix);

	unsigned int imageWidth = image->bmpWidth;
	unsigned int imageHeight = image->bmpHeight;
	unsigned int tileHeight = imageHeight / numThreads;

	if (numThreads < 2) {
		findEdge(imageWidth, imageHeight, data, outData);
	} else {
		std::vector<class Tile> tiles;
		std::thread t[numThreads];

		for (unsigned int index = 0; index < numThreads; index++) {
			// Create vector element representing the current tile.
			if (index + 1 == numThreads) {
				tiles.emplace_back(imageWidth, imageHeight - tileHeight * index, index);
			} else {
				tiles.emplace_back(imageWidth, tileHeight, index);
			}

			tiles[index].image_unsobeled = new unsigned char[tiles[index].size];
			tiles[index].image_sobeled = new unsigned char[tiles[index].size];
			memcpy(&(tiles[index].image_unsobeled[0]), &data[index * tileHeight * imageWidth], tiles[index].size);

			t[index] = std::thread(findEdge, tiles[index].w, tiles[index].h, &tiles[index].image_unsobeled[0], &tiles[index].image_sobeled[0]);

		}
		for (unsigned int index = 0; index < numThreads; index++) {
			t[index].join();
			if (index + 1 == numThreads) {
				memcpy(&outData[index * tileHeight * imageWidth], &tiles[index].image_sobeled[0], tiles[index].size);
			} else {
				memcpy(&outData[index * tileHeight * imageWidth], &tiles[index].image_sobeled[0], tiles[index].size);
			}
		}
	}

	image->writeGrayBmp(outData, outFilename.c_str());

	return 0;
}

void* findEdge(const unsigned int w, // Total width of image
               const unsigned int h, // Total height of image
               unsigned char * imageData,
               unsigned char * imageDataSobeled) {
	int gradient_X = 0;
	int gradient_Y = 0;
	int value = 0;

	// The FOR loop apply Sobel operator
	// to bitmap image data in per-pixel level.
	for (unsigned int y = 1; y < h - 1; ++y) {
		for (unsigned int x = 1; x < w - 1; ++x) {
			// Compute gradient in +ve x direction
			gradient_X = sobel_x[0][0] * imageData[ (x - 1) + (y - 1) * w ]
			             + sobel_x[0][1] * imageData[  x    + (y - 1) * w ]
			             + sobel_x[0][2] * imageData[ (x + 1) + (y - 1) * w ]
			             + sobel_x[1][0] * imageData[ (x - 1) +  y    * w ]
			             + sobel_x[1][1] * imageData[  x    +  y    * w ]
			             + sobel_x[1][2] * imageData[ (x + 1) +  y    * w ]
			             + sobel_x[2][0] * imageData[ (x - 1) + (y + 1) * w ]
			             + sobel_x[2][1] * imageData[  x    + (y + 1) * w ]
			             + sobel_x[2][2] * imageData[ (x + 1) + (y + 1) * w ];

			// Compute gradient in +ve y direction
			gradient_Y = sobel_y[0][0] * imageData[ (x - 1) + (y - 1) * w ]
			             + sobel_y[0][1] * imageData[  x    + (y - 1) * w ]
			             + sobel_y[0][2] * imageData[ (x + 1) + (y - 1) * w ]
			             + sobel_y[1][0] * imageData[ (x - 1) +  y    * w ]
			             + sobel_y[1][1] * imageData[  x    +  y    * w ]
			             + sobel_y[1][2] * imageData[ (x + 1) +  y    * w ]
			             + sobel_y[2][0] * imageData[ (x - 1) + (y + 1) * w ]
			             + sobel_y[2][1] * imageData[  x    + (y + 1) * w ]
			             + sobel_y[2][2] * imageData[ (x + 1) + (y + 1) * w ];

			value = (int)ceil(sqrt( gradient_X * gradient_X +
			                        gradient_Y * gradient_Y));
			// offset the x and y coordinates of the pixels that were being sobeled
			// with the supplied offset so that we can stitch together the tiles
			// in the final image
			imageDataSobeled[ x + y * w ] = 255 - value;
		}
	}
	// Visual Studio requires this to be present; and should not
	// cause issues for other compilers.
	// Thanks to Thomas Peters.
	return 0;
}