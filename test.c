#include "tas_image.h"
#include "math.h"

int main() {
    Img img = imageFromFile("./bliss.ppm");

    printf("%lu, %lu\n", img.width, img.height);

    // imageConvolute3x3(&img, &kernelBlur);

    for (size_t i = 0; i < 1000; i++) {
        imageSet(&img, rand() % img.width, rand() % img.height, RED);
    }

    imageConvolute3x3(&img, &kernelBlur);
    imageConvolute3x3(&img, &kernelBlur);
    imageConvolute3x3(&img, &kernelBlur);

    imageToFile(img, "./bliss_out.ppm");

    imageDelete(&img);
}