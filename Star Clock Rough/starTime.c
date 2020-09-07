#include <stdio.h>
#include <stdlib.h> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


float time(int x1, int y1, int x2, int y2){
    float ang = atan2f(y1-y2,x2-x1);
    if (ang<0) ang = (3.141592*2)+ang;
    ang-=1.570796;
    if (ang<0) ang = (3.141592*2)+ang;
    return (ang/(3.141592*2))*24;
}
double angle(int *a,int *b,int *c){
    double result = atan2(*c - *a, *(c+1) - *(a+1)) - atan2(*b - *a, *(b+1) - *(a+1));
    if (result<0) result = (3.141592*2)+result;
    return result;
}
unsigned char *draw(unsigned char *pic,int x1, int y1, int x2, int y2, int r, int g, int b,int channels,int height, int width,int jump){
    int gd,gm,x,y,end,p,dx,dy;
    dx=x1-x2;
	dy=y1-y2;
	double m = (double)dy/dx;
    if(x1>x2)
	{
		x=x2;
		y=y2;
		end=x1;
	}
	else
	{
		x=x1;
		y=y1;
		end=x2;
	}
    double c = y-m*x;
    pic[(channels * width * y) + (channels * x)]   = r;         // red
    pic[(channels * width * y) + (channels * x)+1] = g;       // green
    pic[(channels * width * y) + (channels * x)+2] = b;       // blue 


    while(x<=end)
	{
		x+=jump;
		y = m*x+c;
		pic[(channels * width * y) + (channels * x)]   = r;         // red
        pic[(channels * width * y) + (channels * x)+1] = g;       // green
        pic[(channels * width * y) + (channels * x)+2] = b;       // blue
		
	}

    return pic;
            
}
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
    int *pg=malloc(10000*sizeof(int));

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
            if(r>60&&g>60&&b>60){
                *pg=i;
                *(pg+1)=j;
                pg+=2;
                count++;
            }
        }
    }

    pg-= (count*2);

    int *p=malloc(250*sizeof(int));
    int h=0,w=0,count2=0,count3=0,check=0;
    int ph=pg[0],pw=pg[1];

    for(int i =0; i < count*2 +1;i+=2) {
        if(pg[i]==-1) continue;
        else if(abs(pg[i]-ph)<2 && abs(pw-pg[i+1])<20){     
        h+=pg[i];
        w+=pg[i+1];
        count2++;
        ph=pg[i];
        pw=pg[i+1];
        pg[i]=-1;
        pg[i+1]=-1;
        
        }else if(pg[i]==ph && abs(pw-pg[i+1])>19 && check == 0){
            check = i;
            while (!(abs(pw-pg[i+1])<20)&&abs(ph-pg[i])<2){
                i+=2;
            }
            i-=2;
        }else if(pg[i]==ph && abs(pw-pg[i+1])>19){
            while (!(abs(pw-pg[i+1])<20)&&abs(ph-pg[i])<2){
                i+=2;
            }
            i-=2;
        }else if(i==(count*2)){
            *p=h/count2;
            *(p+1)=w/count2;
            count3++;
            p+=2;
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
        }
    }

    p-= (count3*2);
    free(pg);

    /*
    for(int i =0; i < count3*2 ;i+=2) {
        printf("Height %d Width %d\n",p[i],p[i+1]);
    }
    
    int *north=p+18;
    int *test0=p+12;


    int *test1=p+10;
    int *test2=p+16;
    int *test3=p+20;
    int *test4=p+22;
    int *test5=p+24;
    int *test6=p+28;
    int *test7=p+26;
    int *test8=p+8;

    
    printf("%f \n",angle(test0,north,test1));
    printf("%f \n",angle(test0,north,test2));
    printf("%f \n",angle(test0,north,test3));
    printf("%f \n",angle(test0,north,test4));
    printf("%f \n",angle(test0,north,test5));
    printf("%f \n",angle(test0,north,test6));
    printf("%f \n",angle(test0,north,test7));
    */
    
    int found=0,a=0,a1,b=0,b1,c=0,c1,d=0,d1,e=0,e1,f=0,f1,g=0,n=0,dip=0;
    for(int i =0; i < count3*2 ;i+=2) {
        if (found == 1) break;
        for(int j =0; j < count3*2 ;j+=2) {
            if (found == 1) break;
            if (i==j) continue;
            for(int l =0; l < count3*2 ;l+=2) {
                if (found == 1) break;
                if (l==j || l==i) continue;
                for(int k =0; k < count3*2 ;k+=2) {
                    if (k==j || k==i || k==l) continue;

                    if (fabs(angle(p+j,p+i,p+k)-2.726165)<0.03 && fabs(angle(p+l,p+i,p+k)-2.493833)<0.03){
                        a=1;
                      a1=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-2.515315)<0.03 && fabs(angle(p+l,p+i,p+k)-2.250788)<0.03){
                        b=1;
                        b1=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-2.306016)<0.03 && fabs(angle(p+l,p+i,p+k)-2.047530)<0.03){
                        c=1;
                        c1=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-2.059232)<0.03 && fabs(angle(p+l,p+i,p+k)-1.818661)<0.03){
                        d=1;
                        d1=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-2.004482)<0.03 && fabs(angle(p+l,p+i,p+k)-1.787069)<0.03){
                        e=1;
                        e1=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-1.706687)<0.03 && fabs(angle(p+l,p+i,p+k)-1.509963)<0.03){
                        f=1;
                        f1=k;
                    }
                    if (fabs(angle(p+j,p+i,p+k)-1.599079)<0.03 && fabs(angle(p+l,p+i,p+k)-1.395436)<0.03){
                        g=1;
                        dip=k;

                    }
                    if(a==1&&b==1&&c==1&&d==1&&e==1&&f==1&&g==1){
                        found=1;
                        n=i;
                    }
                    

                    if (found==1) break;

                }
            }
            //printf("\n\n");
            //printf(" %d %d %d %d %d %d %d \n",a,b,c,d,e,f,g);
            a=0,b=0,c=0,d=0,e=0,f=0,g=0;
        }
       
    }

    if (found==1){
        printf("Stars found North Height %d Width %d\nDipper Height %d Width %d\n",p[n],p[n+1],p[dip],p[dip+1]);
    }else{
        printf("Error. Couldn't match Stars\n");
        exit(1);
    }

    
    float fTime= time(p[n+1],p[n],p[dip+1],p[dip]);
    float month = atoi(argv[3]);
    float date = atoi(argv[4]);

    month-=3;
    date-=6;
    fTime-=(date/30);
    fTime-=(month*2);
    if (fTime>24) fTime-=24;
    if (fTime<24) fTime+=24;

    
    
    int timeH=(int)fTime;
    int timeM=(int)((fTime-timeH)*60);
    printf("Clock %d h %d min\n",timeH,timeM);


    img=draw(img,p[n+1],p[n],p[dip+1],p[dip],255,0,0,channels,height,width,5);
    img=draw(img,p[f1+1],p[f1],p[dip+1],p[dip],255,255,255,channels,height,width,1);
    img=draw(img,p[a1+1],p[a1],p[b1+1],p[b1],255,255,255,channels,height,width,1);
    img=draw(img,p[c1+1],p[c1],p[b1+1],p[b1],255,255,255,channels,height,width,1);
    img=draw(img,p[c1+1],p[c1],p[d1+1],p[d1],255,255,255,channels,height,width,1);
    img=draw(img,p[d1+1],p[d1],p[e1+1],p[e1],255,255,255,channels,height,width,1);
    img=draw(img,p[e1+1],p[e1],p[f1+1],p[f1],255,255,255,channels,height,width,1);
    stbi_write_jpg(argv[2], width, height, channels,img, 100);
    stbi_image_free(img);



 }