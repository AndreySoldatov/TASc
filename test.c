// int main() {
//     Img img = imageFromFile("./bliss.ppm");

//     printf("%lu, %lu\n", img.width, img.height);

//     // imageConvolute3x3(&img, &kernelBlur);

//     for (size_t i = 0; i < 1000; i++) {
//         imageSet(&img, rand() % img.width, rand() % img.height, RED);
//     }

//     imageConvolute3x3(&img, &kernelBlur);
//     imageConvolute3x3(&img, &kernelBlur);
//     imageConvolute3x3(&img, &kernelBlur);

//     imageToFile(img, "./bliss_out.ppm");

//     imageDelete(&img);
// }

// #include "tas_threads.h"

#include "tas_threads.h"
#include "tas_string.h"
#include "tas_args.h"
#include "tas_image.h"
#include "tas_time.h"

Img img;
Img result;

void *processPartOfImage(void *args) {
    ThreadInfo info = *((ThreadInfo*)args);

    size_t length = (img.pixels.length / info.threadCount);
    size_t start = (img.pixels.length / info.threadCount) * info.threadIndex;

    // printf("\tprocessing from %lu to %lu pixels;\n", start, start + length);

    for (size_t i = start; i < start + length; i++) {
        imageSet(
            &result, 
            coord(i, result.width), 
            imageConvolutePixel3x3(img, coord(i, result.width), &kernelEdgeDetection)
        );
    }

    // printf("\tend of processing\n");
    
    return NULL;
}

int main (int argc, char ** argv) {
    StrVec args = strVecFromCStrArray(argc, argv);
    if(!argsContainFlagWithMessage(args, "N")) {
        error_exit("No \"N\" flag (Used to specify the number of working threads);\n");
    }

    Str threadCountMsg = argsGetMessageFromFlag(args, "N");
    size_t threadCount = strtoul(threadCountMsg.data, NULL, 10);
    strDelete(&threadCountMsg);

    strVecDelete(&args);

    printf("Working threads: %lu\n", threadCount);

    img = imageFromFile("./bears.ppm");
    result = imageNew(img.width, img.height, CYAN);

    printf("processing image (%lu, %lu)\n", result.width, result.height);

    Clock cl = clockNew();
    ThreadPool p = tpNew(threadCount, &processPartOfImage);
    tpJoin(&p); 
    printf("Seconds passed: %f\n", clockGetElapsedTime(cl));

    printf("saving image\n");
    imageToFile(result, "./result/bears_Sobel.ppm");

    imageDelete(&img);
    imageDelete(&result);

    return 0;
}