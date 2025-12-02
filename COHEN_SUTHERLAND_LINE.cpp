//To clip a line using Cohen Sutherland Algorithm

#include<graphics.h>
#include<iostream.h>
#include<conio.h>
#include<dos.h>

typedef unsigned int outcode;
enum{TOP=0x1,BOTTOM=0x2,RIGHT=0x4,LEFT=0x8};
outcode compoutcode(double x,double y,double xmin,double xmax,double ymin,double ymax)
{
  outcode code=0;
  if(y>ymax)
	  code |=TOP;
  else if(y<ymin)
	  code |=BOTTOM;
  if(x>xmax)
	  code |=RIGHT;
  else if(x<xmin)
	  code |=LEFT;
  return code;
}

void linedraw(int x0,int y0,int x1,int y1,int color)
{
	int dx=x1-x0;
	int dy=y1-y0;
	int d=2*dy-dx;
	int incE=2*dy;
	int incNE=2*(dy-dx);
	int x=x0;
	int y=y0;
	putpixel(x,y,color);
	while(x<x1)
	{
		if(d<=0)
		{
			d+=incE;
			x++;
		}
		else
		{
			d+=incNE;
			x++;
			y++;
		}
		putpixel(x,y,color);
	}
}

void clipanddraw(double x0,double y0,double x1,double y1,double xmin,double xmax,double ymin,double ymax,int color)
{
   outcode outcode0,outcode1,outcodeout;
   int accept=0,done=0;
   outcode0 = compoutcode(x0,y0,xmin,xmax,ymin,ymax);
   outcode1 = compoutcode(x1,y1,xmin,xmax,ymin,ymax);
   do
    {
	 //trivially accept if both end points have outcode 0000
	 if(!(outcode0|outcode1))    //trivial acceptance
	    {
		 accept=1;
		 done=1;
	    }
	else if(outcode0 & outcode1)  //trivial rejection
		done=1;
	else     //clipping required
	   {
	    double x,y;
	    //select the endpoint to start clipping
	    outcodeout=outcode0 ? outcode0 : outcode1;
	    //equation of the line=> (x-x0) = (x1-x0)/(y1-y0) * (y-y0)
	    if(outcodeout & TOP)//intersection of top edge and line
	    {
		x=x0 + (x1-x0)*(ymax-y0)/(y1-y0);
		y=ymax;
	    }
	    else if(outcodeout & BOTTOM)
	    {
		x=x0 + (x1-x0)*(ymin-y0)/(y1-y0);
		y=ymin;
	    }
	    else if(outcodeout & RIGHT)
	    {
		y=y0 + (y1-y0)*(xmax-x0)/(x1-x0);
		x=xmax;
	    }
	    else      //LEFT clip edge
	    {
		y=y0 + (y1-y0)*(xmin-x0)/(x1-x0);
		x=xmin;
	    }

	    if(outcodeout==outcode0)
	    {
		x0=x;
		y0=y;
		outcode0=compoutcode(x0,y0,xmin,xmax,ymin,ymax);
	   }
	   else
	   {
		 x1=x;
		 y1=y;
		 outcode1=compoutcode(x1,y1,xmin,xmax,ymin,ymax);
	   }
	}
    }while(done==0);

    if(accept)
	linedraw(x0,y0,x1,y1,color);
}

void main()
{
	clrscr();
	double x0,x1,y0,y1;
	double xmax,xmin,ymax,ymin;
	int gm,gd=DETECT;
	int color=15;  //15=white
	cout<<"Enter the co-ordinates of line, P0->";	cin>>x0>>y0;
	cout<<"Enter the co-ordinates of line, P1->";	cin>>x1>>y1;
	cout<<"Enter xmin";	cin>>xmin;
	cout<<"Enter xmax";	cin>>xmax;
	cout<<"Enter ymin";	cin>>ymin;
	cout<<"Enter ymax";	cin>>ymax;
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	linedraw(x0,y0,x1,y1,12); //12=red
	for(int i=xmin;i<=xmax;i++)
		putpixel(i,ymin,9); //9=blue
	for(int j=ymin;j<=ymax;j++)
		putpixel(xmin,j,9);
	for(i=xmin;i<=xmax;i++)
		putpixel(i,ymax,9);
	for(j=ymin;j<=ymax;j++)
		putpixel(xmax,j,9);
	clipanddraw(x0,y0,x1,y1,xmin,xmax,ymin,ymax,color);
	getch();
	closegraph();
}