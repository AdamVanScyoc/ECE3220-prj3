# CPSC/ECE3220-prj3
This is a submission for project 3 of CPSC/ECE 3220 at Clemson University.

## Assumptions
We used the c++11 thread library therefore the program must be compiled with the `-pthread` and `-std=c++11` flag. Please use the included makefile.

We also assume the memory of the machine to be atleast twice the size of the image, since we are copying each section of the image for each thread, effectively creating a second image. This made syncronization easy. However this is not the ideal solution due to the space inefficiency.

We also assumed there to be 2 arguments. The first being the file name and the second being the number of threads created. We assume the file name to be either ending in `.txt` or `.bmp` all other file names are rejected. For `.txt` files, the program will assume that it contains a list of `.bmp` files to be processed. These could be either relative path or absolute path. Each of them will be processed with x number of threads, where x is the number in `argv[2]`.

After the program processes the image, they will be stored in the same directory as the orginal image with "_processed" appended to the end of the filename. Therefore we also assume the program/user have read and write permission in that/those directory/directories.
