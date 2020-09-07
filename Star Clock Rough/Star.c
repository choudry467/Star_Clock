#include <stdio.h>
#include <stdlib.h> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



int main(int argc, char ** argv) {
     int width, height, channels;

    // Loading the image. Input and output files are taken as executable file arguments
    unsigned char *img = stbi_load(argv[1], &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
    
    

    size_t paint_img_size = width * height * channels;

    // Creating memory block for output image
    unsigned char *pg = malloc(paint_img_size);

    if(pg == NULL) {
        printf("Unable to allocate memory for the paint image.\n");
        exit(1);
    }

    for(int i =0; i < height ;i++) {
	    for (int j=0; j < width; j++) {
            unsigned char r = (uint8_t)img[(channels * width * i) + (channels * j)];           //Red Channel read
            unsigned char g = (uint8_t)img[(channels * width * i) + (channels * j) + 1 ];      //Green Channel read
            unsigned char b = (uint8_t)img[(channels * width * i) + (channels * j) + 2 ];      //Blue Channel read
            if(r>10&&g>10&&b>10){
                //printf("red %u green %u blue %u height %d width %d \n",r,g,b,i,j);
                pg[(channels * width * i) + (channels * j)]      = (uint8_t)255;         // red
                pg[(channels * width * i) + (channels * j)+1] = (uint8_t)0;         // green
                pg[(channels * width * i) + (channels * j)+2] = (uint8_t)0;         // blue 
            }else{
                pg[(channels * width * i) + (channels * j)]   = r;         // red
                pg[(channels * width * i) + (channels * j)+1] = g;       // green
                pg[(channels * width * i) + (channels * j)+2] = b;       // blue 
            }

            if(channels == 4) {
                pg[(channels * width * i) + (channels * j) + 3 ] = img[(channels * width * i) + (channels * j) + 3 ];
            }

            //Incrementing pointer for output memory block
            //pg+=channels;
        }
    }

    // Writing the image to the file. Last argument is the compression size. 100 means no compression
    stbi_write_jpg(argv[2], width, height, channels, pg, 100);

    // Deallocation of memory
    stbi_image_free(img);
    free(pg);
 }