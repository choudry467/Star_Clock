// This file contains the definitions of all the functions used in main file (starClock)



// returns time using coordinates of 2 points. Adjusted for images because in image data unlike cartesian planes x increases from top to bottom
float time(int x1, int y1, int x2, int y2){
    float ang = atan2f(y1-y2,x2-x1);
    if (ang<0) ang = (3.141592*2)+ang;
    ang-=1.570796;
    if (ang<0) ang = (3.141592*2)+ang;
    return (ang/(3.141592*2))*24;
}

// Returns angle bac
double angle(int *a,int *b,int *c){
    double result = atan2(*c - *a, *(c+1) - *(a+1)) - atan2(*b - *a, *(b+1) - *(a+1));
    if (result<0) result = (3.141592*2)+result;
    return result;
}


// Draws a line between 2 points. Takes the whole pixel memory block
// Increase jump for dotted line. r g b values to determine the color of the line 
unsigned char *draw(unsigned char *pic,int x1, int y1, int x2, int y2, int r, int g, int b,int channels,int height, int width,int jump){
    
    int gd,gm,x,y,end,p,dx,dy;
    dx=x1-x2;
	dy=y1-y2;

    // Gradient of the line
	double m = (double)dy/dx;
    
    // Finding the lower point
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
    // Calculating Y intercept
    double c = y-m*x;

    // Drawing the first point
    pic[(channels * width * y) + (channels * x)]   = r;         // red
    pic[(channels * width * y) + (channels * x)+1] = g;       // green
    pic[(channels * width * y) + (channels * x)+2] = b;       // blue 


    // Going through all the X coordinates and Corresponding Y coordinates to draw a line
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


