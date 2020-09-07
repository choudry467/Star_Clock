#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "starClockHelper.c"


int main(int argc, char ** argv) {
     int width, height, channels;

    // Loading the image. Input and output files are taken as executable file arguments
    unsigned char *img = stbi_load(argv[1], &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
    
    


    // Creating memory block for star pixels
    int *pg=malloc(5000*sizeof(int));

    if(pg == NULL) {
        printf("Unable to allocate memory for the paint image.\n");
        exit(1);
    }
    // Threshold for star pixels
    int thresh = atoi(argv[5]);
    // Counter for star pixels
    int count = 0;

    // Nested loop to go through all pixels
    for(int i =0; i < height ;i++) {
	    for (int j=0; j < width; j++) {
            unsigned char r = (uint8_t)img[(channels * width * i) + (channels * j)];           //Red Channel read
            unsigned char g = (uint8_t)img[(channels * width * i) + (channels * j) + 1 ];      //Green Channel read
            unsigned char b = (uint8_t)img[(channels * width * i) + (channels * j) + 2 ];      //Blue Channel read
            
            // Check if a pixel is bright enough to be star
            if(r>thresh&&g>thresh&&b>thresh){
                
                // Storing height and width in the block
                *pg=i;
                *(pg+1)=j;
                pg+=2;

                // Incrementing counter
                count++;

                // Having too many star pixels means threshold needs to be readjusted
                if(count>2499){
                    printf("Too many pixels above threshold. Change the threshold\n");
                    exit(1);
                }
            }
        }
    }

    // Taking the pointer to the start of the memory block
    pg-= (count*2);

    // Creating memory block for star centre coordinates
    int *p=malloc(1000*sizeof(int));

    // Avg Height and Width for each star. Counters for pixels taken by a star and total start. A check for boundry condition.
    int h=0,w=0,count2=0,count3=0,check=0;

    // Height and Width of last read pixel
    int ph=pg[0],pw=pg[1];

    // Loop to iterate through all star pixels. Height and Width are stored in conseutive addresses so each stride is +2
    for(int i =0; i < count*2 +1;i+=2) {

        if(pg[i]==-1) continue; // continue if pixel is already visited

        else if(abs(pg[i]-ph)<2 && abs(pw-pg[i+1])<20){ // if neighboring pixel (pixel of same star)
        h+=pg[i];
        w+=pg[i+1];
        count2++;
        ph=pg[i];
        pw=pg[i+1];
        pg[i]=-1;
        pg[i+1]=-1;
        
        }else if(pg[i]==ph && abs(pw-pg[i+1])>19 && check == 0){ // if at same height but not neighboring pixel (special condition)
            check = i;
            while (!(abs(pw-pg[i+1])<20)&&abs(ph-pg[i])<2){ // Iterate until pixel of same star is not found or until if too far away from star pixel(height)
                i+=2;
            }
            i-=2;
        }else if(pg[i]==ph && abs(pw-pg[i+1])>19){ // if at same height but not neighboring pixel (special condition)(continued)
            while (!(abs(pw-pg[i+1])<20)&&abs(ph-pg[i])<2){
                i+=2;
            }
            i-=2;
        }else if(i==(count*2)){ // at the last pixel of the memory block

            *p=h/count2;
            *(p+1)=w/count2;
            count3++;
            p+=2;
        }else if (check != 0){  // Star end (special condition)
            *p=(h/count2);
            *(p+1)=(w/count2);
            h=0;
            w=0;
            count3++;
            count2=0;
            ph=pg[check];
            pw=pg[check+1];
            i=check-2;
            p+=2;
            check = 0;
        }else{                  // Star end
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
        }
    }

    // Take star coordinate pointer to start and free pixel memory block
    p-= (count3*2);
    free(pg);

    // Allowed error while matching stars
    float err= atof(argv[6]);

    // Saved angles of stars from north star 2 reference stars (0 and 1)
    double angles0[]={2.726165,2.515315,2.306016,2.059232,2.004482,1.706687,1.599079};
    double angles1[]={2.493833,2.250788,2.047530,1.818661,1.787069,1.509963,1.395436};

    // Flags for all the stars found
    bool flags[]={false,false,false,false,false,false,false};
    int found=0;

    // Coordinates for stars matched
    int coordinates[]={-1,-1,-1,-1,-1,-1,-1,-1};

    // Iterating through all stars. 3 Nested loops to BruteForce North star and Reference stars 0 and 1
    for(int i =0; i < count3*2 ;i+=2) {
        if (found == 1) break;
        for(int j =0; j < count3*2 ;j+=2) {
            if (found == 1) break;
            if (i==j) continue;
            for(int l =0; l < count3*2 ;l+=2) {
                if (found == 1) break;
                if (l==j || l==i) continue;

                // Comparing angles to find big dipper stars. If star matches the angle, store coordinates
                for(int k =0; k < count3*2 ;k+=2) {
                    if (k==j || k==i || k==l) continue;

                    if (fabs(angle(p+j,p+i,p+k)-angles0[0])<err && fabs(angle(p+l,p+i,p+k)-angles1[0])<err){
                        flags[0]=true;
                        coordinates[0]=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-angles0[1])<err && fabs(angle(p+l,p+i,p+k)-angles1[1])<err){
                        flags[1]=true;
                        coordinates[1]=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-angles0[2])<err&& fabs(angle(p+l,p+i,p+k)-2.047530)<err){
                        flags[2]=true;
                        coordinates[2]=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-angles0[3])<err && fabs(angle(p+l,p+i,p+k)-1.818661)<err){
                        flags[3]=true;
                        coordinates[3]=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-angles0[4])<err && fabs(angle(p+l,p+i,p+k)-1.787069)<err){
                        flags[4]=true;
                        coordinates[4]=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-angles0[5])<err && fabs(angle(p+l,p+i,p+k)-1.509963)<err){
                        flags[5]=true;
                        coordinates[5]=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-angles0[6])<err && fabs(angle(p+l,p+i,p+k)-1.395436)<err){
                        flags[6]=true;
                        coordinates[6]=k;

                    }

                    // Check flags to see if all stars are found. Break the loops if true
                    if(flags[0]&&flags[1]&&flags[2]&&flags[3]&&flags[4]&&flags[5]&&flags[6]){
                        found=1;

                        // Set coordinates for norht star
                        coordinates[7]=i;
                    }
                    

                    if (found==1) break;

                }
            }
            // Set all flags to zero if combo not found.
            for(int m=0;m<7;m++) flags[m]=false;
        }
       
    }

    if (found==1){
        printf("Stars found North Height %d Width %d\nDipper Height %d Width %d\n",p[coordinates[7]],p[coordinates[7]+1],p[coordinates[6]],p[coordinates[6]+1]);
    }else{
        printf("Error. Couldn't match Stars\n");
        exit(1);
    }

    // Calculating raw time using angle between north star and big dipper
    float fTime= time(p[coordinates[7]+1],p[coordinates[7]],p[coordinates[6]+1],p[coordinates[6]]);
    //Raw time
    //printf("Raw Time %f\n",fTime);


    // Taking day and month
    float month = atoi(argv[3]);
    float date = atoi(argv[4]);
    
    // Readjusting to March 6(When raw time of 24 is actual 24)
    month-=3;
    date-=6;
    fTime-=(date/15);
    fTime-=(month*2);
    if (fTime>24) fTime-=24;
    if (fTime<0) fTime+=24;
  
    
    // Converting floating point part of hours to minutes 
    int timeH=(int)fTime;
    int timeM=(int)((fTime-timeH)*60);

    // Displaying the time
    printf("Clock %d h %d min\n",timeH,timeM);

    // Drawing the big dipper and red line for raw time
    img=draw(img,p[coordinates[7]+1],p[coordinates[7]],p[coordinates[6]+1],p[coordinates[6]],255,0,0,channels,height,width,5);
    img=draw(img,p[coordinates[6]+1],p[coordinates[6]],p[coordinates[5]+1],p[coordinates[5]],255,255,255,channels,height,width,1);
    img=draw(img,p[coordinates[0]+1],p[coordinates[0]],p[coordinates[1]+1],p[coordinates[1]],255,255,255,channels,height,width,1);
    img=draw(img,p[coordinates[2]+1],p[coordinates[2]],p[coordinates[1]+1],p[coordinates[1]],255,255,255,channels,height,width,1);
    img=draw(img,p[coordinates[2]+1],p[coordinates[2]],p[coordinates[3]+1],p[coordinates[3]],255,255,255,channels,height,width,1);
    img=draw(img,p[coordinates[3]+1],p[coordinates[3]],p[coordinates[4]+1],p[coordinates[4]],255,255,255,channels,height,width,1);
    img=draw(img,p[coordinates[4]+1],p[coordinates[4]],p[coordinates[5]+1],p[coordinates[5]],255,255,255,channels,height,width,1);
    
    // Writing the final image
    stbi_write_jpg(argv[2], width, height, channels,img, 100);
    
    // Deallocating memory blocks
    stbi_image_free(img);
    free(p);



 }