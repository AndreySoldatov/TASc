### To check documentation:
1. clone
2. open docs/index.html

``` c
#include "tas_threads.h"
#include "tas_string.h"
#include "tas_args.h"
#include "tas_image.h"
#include "tas_time.h"

// Create source and result images
Img img;
Img result;

// Function that will be put to the threadPool
void *processPartOfImage(void *args) {
    // Each ThreadPool thread function has ThreadInfo as an argument
    // It holds the whole number of threads in the pool and the index of current
    // thread that is executing the function
    // It's used to separate work between threads
    ThreadInfo info = *((ThreadInfo*)args);

    size_t length = (img.pixels.length / info.threadCount);
    size_t start = (img.pixels.length / info.threadCount) * info.threadIndex;

    for (size_t i = start; i < start + length; i++) {
        imageSet(
            &result, 
            coord(i, result.width), 
            imageConvolutePixel3x3(img, coord(i, result.width), &kernelEdgeDetection)
        );
    }
    
    return NULL;
}

int main (int argc, char ** argv) {
    // Convet argv to StrVec array, that holds all the cli arguments
    StrVec args = strVecFromCStrArray(argc, argv);
    // Checks if the program has flag N, that in this case needed to
    // specify the number of working threads
    if(!argsContainFlagWithMessage(args, "N")) {
        error_exit("No \"N\" flag (Used to specify the number of working threads);\n");
    }

    Str threadCountMsg = argsGetMessageFromFlag(args, "N");
    size_t threadCount = strtoul(threadCountMsg.data, NULL, 10);
    strDelete(&threadCountMsg);

    strVecDelete(&args);

    printf("Working threads: %lu\n", threadCount);

    // Load image from the disk
    // this is this: https://images.freeimages.com/images/large-previews/d41/bear-combat-2-1332988.jpg image
    // if you are interested
    img = imageFromFile("./bears.ppm");
    // Allocating resulting image with width and height of original
    // And filled with CYAN color
    // (Filling is not necessary, but why the hell not)
    result = imageNew(img.width, img.height, CYAN);

    printf("processing image (%lu, %lu)\n", result.width, result.height);

    // Creating a new clock that, upon creation, will hold the time in nanosecs
    // From bootload to it's creation
    Clock cl = clockNew();

    // The sweet part
    // Here we are creating ThreadPool with the number of working threads specified in the
    // threadCouhnt variable, and assigning the working task to them
    // All the indexing work is done automatically
    ThreadPool p = tpNew(threadCount, &processPartOfImage);
    
    // Waiting until all the threads are finished and cleaning up the resourses
    tpJoin(&p);

    // Checking the benchmark, by getting the elapsed time from our clock
    printf("Seconds passed: %f\n", clockGetElapsedTime(cl));

    // No comment
    printf("saving image\n");
    imageToFile(result, "./result/bears_Sobel.ppm");

    // Cleaning up all the used resources
    imageDelete(&img);
    imageDelete(&result);

    return 0;
}
```