#include <stdio.h>
#include "image.h"
#include "timer.h"
#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Functions for loading, saving, and freeing images
int load_image(const char *filename, Image *img) {
    img->data = stbi_load(filename, &img->width, &img->height, &img->bpp, 1);
    return img->data != NULL;
}

int save_image(const char *filename, Image *img) {
    return stbi_write_png(filename, img->width, img->height, 1, img->data, img->width) != 0;
}

void free_image(Image *img) {
    stbi_image_free(img->data);
}

// Histogram equalization function
void histogram_equalization(Image *srcImage, Image *destImage) {
    int hist[256] = {0};                  // Histogram array
    int new_gray_level[256] = {0};        // Mapping of new intensity values
    long total = srcImage->height * srcImage->width; // Total pixels

    // Step 1: Calculate histogram in parallel
    #pragma omp parallel
    {
        int local_hist[256] = {0};        // Local histogram for each thread
        #pragma omp for
        for (int row = 0; row < srcImage->height; row++) {
            for (int col = 0; col < srcImage->width; col++) {
                int index = row * srcImage->width + col;
                local_hist[srcImage->data[index]]++;
            }
        }
        #pragma omp critical
        for (int i = 0; i < 256; i++) {
            hist[i] += local_hist[i];
        }
    }

    // Step 2: Calculate cumulative distribution function (CDF)
    long cumulative = 0;
    for (int i = 0; i < 256; i++) {
        cumulative += hist[i];
        new_gray_level[i] = (int)((cumulative * 255) / total); // Normalize to 255
    }

    // Step 3: Map new gray levels to the destination image in parallel
    #pragma omp parallel for
    for (int row = 0; row < srcImage->height; row++) {
        for (int col = 0; col < srcImage->width; col++) {
            int index = row * srcImage->width + col;
            destImage->data[index] = new_gray_level[srcImage->data[index]];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    Image srcImage, destImage;

    // Load the image
    if (!load_image(argv[1], &srcImage)) {
        printf("Error: could not load image %s\n", argv[1]);
        return 1;
    }

    // Allocate memory for the destination image
    destImage.width = srcImage.width;
    destImage.height = srcImage.height;
    destImage.bpp = 1;
    destImage.data = (unsigned char *)malloc(srcImage.width * srcImage.height);

    // Perform histogram equalization
    histogram_equalization(&srcImage, &destImage);

    // Save the resulting image
    if (!save_image("output.png", &destImage)) {
        printf("Error: could not save output image\n");
        return 1;
    }

    // Free the memory used by the images
    free_image(&srcImage);
    free_image(&destImage);

    return 0;
}
