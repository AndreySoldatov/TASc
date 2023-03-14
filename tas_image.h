#ifndef TAS_IMAGE
#define TAS_IMAGE

#include "tas_string.h"
#include "tas_files.h"
#include "tas_vector.h"
#include "tas_commonmacro.h"

#define index(w, x, y) (w * y + x)
#define coord(i, w) (i % w), (i / w)

typedef struct Color {
    Byte r;
    Byte g;
    Byte b;    
} Color;

Color colorNew(Byte r, Byte g, Byte b) {
    Color res;
    res.r = r;
    res.g = g;
    res.b = b;
    return res;
}

void colorPrint(Color c) {
    printf("[%u, %u, %u]", c.r, c.g, c.b);
}

#define BLACK colorNew(0, 0, 0)

#define RED colorNew(255, 0, 0)
#define GREEN colorNew(0, 255, 0)
#define BLUE colorNew(0, 0, 255)

#define CYAN colorNew(0, 255, 255)
#define MAGENTA colorNew(255, 0, 255)
#define YELLOW colorNew(255, 255, 0)

#define LIGHTGRAY colorNew(200, 200, 200)
#define GRAY colorNew(130, 130, 130)
#define DARKGRAY colorNew(60, 60, 60)

#define WHITE colorNew(255, 255, 255)

VEC(Color)

typedef struct Img {
    Vec_Color pixels;
    size_t width;
    size_t height;
} Img;

Img imageNew(size_t w, size_t h, Color c) {
    Img res;
    res.pixels = vecNew_Color();
    requestNewCap_Color(&res.pixels, w * h);

    res.width = w;
    res.height = h;
    
    for (size_t i = 0; i < w * h; i++) {
        vecPush_Color(&res.pixels, c);
    }

    return res;
}

void imageDelete(Img * image) {
    vecDelete_Color(&image->pixels);
}

Img imageFromFile(char * path) {
    ByteBuffer bb = fileReadByteBuffer(path);
    Str fileContent = strFromByteBuffer(bb);

    Img res;

    if(!strStartsWithCStr(fileContent, "P6")) {
        error_exit("imageFromFile error: File is corrupted or unsupported (no \"P6\" heading)\n");
    }

    size_t cursor_start = 2;
    if(!isWhitespace(strAt(fileContent, cursor_start))) {
        error_exit("imageFromFile error: File is corrupted or unsupported (no whitespace)\n");
    }

    size_t cursor_end = cursor_start + 1;

    if(fileContent.data[cursor_end] == '#') {
        while (fileContent.data[cursor_end] != '\n') {
            cursor_end++;
        }

        cursor_start = cursor_end + 1;
        cursor_end = cursor_start + 1;
    }

    for (; !isWhitespace(fileContent.data[cursor_end]); cursor_end++);

    Str num = strSub(fileContent, cursor_start, cursor_end);
    res.width = strtoul(num.data, NULL, 10);
    strDelete(&num);

    cursor_start = cursor_end + 1;
    cursor_end = cursor_start + 1;
    for (; !isWhitespace(fileContent.data[cursor_end]); cursor_end++);
    
    num = strSub(fileContent, cursor_start, cursor_end);
    res.height = strtoul(num.data, NULL, 10);
    strDelete(&num);

    cursor_end = cursor_end + 1;
    for (; !isWhitespace(fileContent.data[cursor_end]); cursor_end++);
    cursor_start = cursor_end + 1;

    res.pixels = vecNew_Color();
    requestNewCap_Color(&res.pixels, res.width * res.height);

    for (size_t i = cursor_start; i < fileContent.length; i+=3) {
        vecPush_Color(
            &res.pixels,
            colorNew(fileContent.data[i], fileContent.data[i + 1], fileContent.data[i + 2])
        );
    }

    byteBufferDelete(&bb);
    strDelete(&fileContent);
    return res;
}

void imageToFile(Img image, char * path) {
    Str fileContent = strNew("P6\n# This file was automatically created by tas_image lib\n");
    strRequestNewCap(&fileContent, image.pixels.length * 3 + BLOCK_SIZE);
    char buffer[16];
    sprintf(buffer, "%lu", image.width);
    strAppendCStr(&fileContent, buffer);
    strAppendCStr(&fileContent, " ");
    sprintf(buffer, "%lu", image.height);
    strAppendCStr(&fileContent, buffer);
    strAppendCStr(&fileContent, "\n255\n");

    for (size_t i = 0; i < image.pixels.length; i++) {
        strPush(&fileContent, image.pixels.data[i].r);
        strPush(&fileContent, image.pixels.data[i].g);
        strPush(&fileContent, image.pixels.data[i].b);
    }

    ByteBuffer bb = strToByteBuffer(fileContent);

    fileWriteByteBuffer(path, bb);

    byteBufferDelete(&bb);
    strDelete(&fileContent);
}

Color imageAt(Img img, size_t x, size_t y) {
    if(x >= img.width || y >= img.height) {error_exit("imageAt error: out of bounds")}
    return img.pixels.data[index(img.width, x, y)];
}

void imageSet(Img * img, size_t x, size_t y, Color c) {
    if(x >= img->width || y >= img->height) {return;}
    img->pixels.data[index(img->width, x, y)] = c;
}

void imageFill(Img * img, Color c) {
    for (size_t i = 0; i < img->pixels.length; i++) {
        img->pixels.data[i] = c;
    }
}

//TODO: Shapes, drawing

void imageShade(Img * img, Color (*func)(Color, size_t, size_t)) {
    for (size_t x = 0; x < img->width; x++) {
        for (size_t y = 0; y < img->height; y++) {
            imageSet(img, x, y, (*func)(imageAt(*img, x, y), x, y));
        }
    }
}

void imageConvolute3x3(Img * img, Color (*func)(Color[])) {
    Img res = imageNew(img->width, img->height, BLACK);

    for (size_t x = 0; x < img->width; x++) {
        for (size_t y = 0; y < img->height; y++) {
            Color kernel[9] = {
                imageAt(*img, (x-1) % img->width, (y-1) % img->height), 
                imageAt(*img, x, (y-1) % img->height),
                imageAt(*img, (x+1) % img->width, (y-1) % img->height),

                imageAt(*img, (x-1) % img->width, y), 
                imageAt(*img, x, y),
                imageAt(*img, (x+1) % img->width, y),

                imageAt(*img, (x-1) % img->width, (y+1) % img->height), 
                imageAt(*img, x, (y+1) % img->height),
                imageAt(*img, (x+1) % img->width, (y+1) % img->height)
            };

            imageSet(&res, x, y, (*func)(kernel));
        }
    }

    imageDelete(img);

    img->pixels = res.pixels;
    img->width = res.width;
    img->height = res.height;
}

Color imageConvolutePixel3x3(Img img, size_t x, size_t y, Color (*func)(Color[])) {
    if(x >= img.width || y >= img.height) {
        // error_exit("imageConvolutePixel3x3 error: out of bounds\n");
        printf("(%lu, %lu)\n", x, y);
        exit(1);
    }

    Color kernel[9] = {
        imageAt(img, (x-1) % img.width, (y-1) % img.height), 
        imageAt(img, x, (y-1) % img.height),
        imageAt(img, (x+1) % img.width, (y-1) % img.height),

        imageAt(img, (x-1) % img.width, y), 
        imageAt(img, x, y),
        imageAt(img, (x+1) % img.width, y),

        imageAt(img, (x-1) % img.width, (y+1) % img.height), 
        imageAt(img, x, (y+1) % img.height),
        imageAt(img, (x+1) % img.width, (y+1) % img.height)
    };

    return (*func)(kernel);
}

Color colorGrayScale(Color c, size_t x, size_t y) {
    float res_b = (float)(c.r + c.g + c.b) * 0.3333;
    return colorNew((Byte)(res_b), (Byte)(res_b), (Byte)(res_b));
}

Color kernelBlur(Color c[]) {
    float k[] = {
        0.08, 0.11, 0.08,
        0.11, 0.23, 0.11,
        0.08, 0.11, 0.08
    };

    float r_v = 0.0;
    float g_v = 0.0;
    float b_v = 0.0;

    for (size_t i = 0; i < 9; i++) {
        r_v += (float)c[i].r * k[i];
        g_v += (float)c[i].g * k[i];
        b_v += (float)c[i].b * k[i];
    }

    return colorNew((Byte)r_v, (Byte)g_v, (Byte)b_v);
}

Color kernelEdgeDetection(Color c[]) {
    float dx = abs((float)(c[3].r + c[3].g + c[3].b) - (float)(c[5].r + c[5].g + c[5].b)) * 0.333;
    float dy = abs((float)(c[1].r + c[1].g + c[1].b) - (float)(c[8].r + c[8].g + c[8].b)) * 0.333;

    return colorNew((Byte)dx, (Byte)(dx + dy), (Byte)dy);
}

#endif