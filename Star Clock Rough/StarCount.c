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
    
    


    // Creating memory block for output image
    int *pg=malloc(5000*sizeof(int));

    if(pg == NULL) {
        printf("Unable to allocate memory for the paint image.\n");
        exit(1);
    }
    int count = 0;
    for(int i =0; i < height ;i++) {
	    for (int j=0; j < width; j++) {
            unsigned char r = (uint8_t)img[(channels * width * i) + (channels * j)];           //Red Channel read
            unsigned char g = (uint8_t)img[(channels * width * i) + (channels * j) + 1 ];      //Green Channel read
            unsigned char b = (uint8_t)img[(channels * width * i) + (channels * j) + 2 ];      //Blue Channel read
            if(r>15||g>15||b>15){
                *pg=i;
                *(pg+1)=j;
                pg+=2;
                count++;
            }
        }
    }

    pg-= (count*2);

    printf("pg Loaded\n");

    int *p=malloc(150*sizeof(int));
    stbi_image_free(img);
    int h=0,w=0,count2=0,count3=0,check=0;
    int ph=pg[0],pw=pg[1];

    for(int i =0; i < count*2 +1;i+=2) {
        if(pg[i]==-1) continue;
        else if(abs(pg[i]-ph)<2 && abs(pw-pg[i+1])<15){     
        h+=pg[i];
        w+=pg[i+1];
        count2++;
        ph=pg[i];
        pw=pg[i+1];
        printf("No. %d h %d w %d\n",i,pg[i],pg[i+1]);
        pg[i]=-1;
        pg[i+1]=-1;
        
        }else if(pg[i]==ph && abs(pw-pg[i+1])>14 && check == 0){
            check = i;
            while (!(abs(pw-pg[i+1])<15)&&abs(ph-pg[i])<2){
                i+=2;
            }
            printf("check ran %d\n",i);
            i-=2;
        }else if(pg[i]==ph && abs(pw-pg[i+1])>14){
            while (!(abs(pw-pg[i+1])<15)&&abs(ph-pg[i])<2){
                i+=2;
            }
            printf("check continued %d\n",i);
            i-=2;
        }else if (check != 0){
            *p=(h/count2);
            *(p+1)=(w/count2);
            h=0;
            w=0;
            count3++;
            count2=0;
            ph=pg[check];
            pw=pg[check+1];
            i=check;
            p+=2;
            check = 0;
            printf("p val added %d\n",i);
        }else if(i==(count*2)){
            *p=h/count2;
            *(p+1)=w/count2;
            count3++;
            p+=2;
        }else{
            *p=(h/count2);
            *(p+1)=(w/count2);
            h=0;
            w=0;
            count3++;
            count2=0;
            ph=pg[i];
            pw=pg[i+1];
            i-=2;
            p+=2;
            
            printf("p val added %d\n",i);
        }
    }

    p-= (count3*2);
    printf("p Loaded\n");
    free(pg);

    for(int i =0; i < count3*2 ;i+=2) {
        printf("Height %d Width %d\n",p[i],p[i+1]);
    }
    
    /*for(int i =0; i < count3*2 ;i+=2) {
	    for (int j=0; j < count3*2 ;j+=2) {
            if (i==j) continue;
            for (int k=0 ; k < count3*2 ;k+=2){
                if (k==i || k==j) continue;

            }
        }
    }*/

 }