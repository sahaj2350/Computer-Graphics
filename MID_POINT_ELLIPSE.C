#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>

 void ellipsemidpoint(int xcenter, int ycenter, int rx, int ry);
 void  plotpoint(int x,int xcenter, int ycenter,int y);


int maxx, maxy,xcenter,ycenter,rx,ry;

int main(void)
{
   int gdriver=DETECT, gmode, errorcode;
   initgraph(&gdriver, &gmode, "C:\\TURBOC3\\BGI");
   errorcode = graphresult();
      if (errorcode != grOk)
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1);
   }
   maxx = getmaxx();
   maxy = getmaxy();
   xcenter= 250;
   ycenter= 250;
   rx= 150;
   ry= 150;
   ellipsemidpoint(xcenter,ycenter,rx,ry);

   getch();
      closegraph();

 return(0);
}
 void ellipsemidpoint( int xcenter, int ycenter,int rx, int ry)
 {
  int p,px,py,x,y,ry2,rx2,tworx2,twory2;
    ry2= ry * ry;
    rx2= rx * rx;
    twory2= 2 * ry2;
    tworx2= 2 * rx2;
    x= 0;
    y= ry;
    plotpoint(x, xcenter,  ycenter, y);

   p= abs(ry2 - ry2 * ry + ( 0.25 * rx2));
   px= 0;
   py= tworx2 * y;
    while(px<py)
	{
	  x= x+1;
	  px= px + twory2;
	   if(p>=0)
	    {
	      y= y-1;
	      py= py - tworx2;
	     }
	  if(p<0)
	   {
	    p= p + ry2 + px;
	   }
       else
	{
	  p= p + ry2 + px;
	}
      plotpoint( x, xcenter,  ycenter, y);

     }
   p= abs(ry2 * (x + 6.5) * (x + 6.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
     while (y>0)
	{
	 y= y-1;
	 py= py - tworx2;
	if(p<0)
	 {
	   x= x +1;
	   px= px + twory2;
	 }
       if(p>0)
	 {
	  p= p + rx2 - py;
	 }
    else
       {
	p= p + rx2 -py +px;
      plotpoint(x, xcenter,  ycenter, y);

       }
    }
 }
   void plotpoint(int x,int xcenter, int ycenter,int y)
	   {

		putpixel (xcenter + x, ycenter + y, 2);
		putpixel (xcenter - x, ycenter + y, 2);
		putpixel (xcenter + x, ycenter - y, 2);
		putpixel (xcenter - x, ycenter - y, 2);

	   }