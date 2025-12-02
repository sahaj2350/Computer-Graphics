//PROGRAM TO DRAW A LINE USING BRESENHAM'S ALGORITHM

#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<math.h>
#include<iostream.h>


//FUNCTION TO DRAW A LINE USING BRESENHAM'S ALGORITHM
 x0,y0;
float x1,y1;
void input()
{

	cout<<endl<<endl<<"ENTER THE FIRST COORDINATE\n";
	cout<<"ENTER THE X COORDINATE:";
	cin>>x0;

	cout<<"ENTER THE Y COORDINATE:";
	cin>>y0;

	cout<<"\n\nENTER THE SECOND COORDINATE\n";
	cout<<"ENTER THE X COORDINATE: ";
	cin>>x1;

	cout<<"ENTER THE Y COORDINATE: ";
	cin>>y1;
}

#define INCR 1
#define DECR -1

float d,incrE,incrNE;
void togglexy_bres()
{
	if (x0>x1)
	{
		float temp=x0;
		x0=x1;
		x1=temp;

		temp=y0;
		y0=y1;
		y1=temp;
	}
}

void toggley_bres()
{
	if (y0 > y1)
	{
		float temp=y0;
		y0=y1;
		y1=temp;
	}
}

void linedraw(float x0,float y0,float x1,float y1,int pred,int incr)
{
	float start,end,var;
	if (pred==1)
	{
		start=x0;
		end=x1;
		var=y0;
	}
	else
	{
		start=y0;
		end=y1;
		var=x0;
	}
	float i=start;
	while (i<=end)
	{
		if (pred==1)
			putpixel(i,var,WHITE);
		else
			putpixel(var, i,WHITE);

		if(d<=0)
			d+=incrE;
		else
		{
			d+=incrNE;
			var+=incr;
		}
		i++;
	}
}

void bres_line()
{
	togglexy_bres();

	float dx=x1-x0;
	float dy=y1-y0;

	float x=x0;
	float y=y0;

	if(dx==0)      /*Vertical line*/
	{
		toggley_bres();
		while(y<=y1)
		{
			putpixel(x0,y,WHITE);
			y=y+1;
		}
	}
	else
	if(dy==0)        /*Horizontal line*/
	{
		while(x<=x1)
		{
			putpixel(x,y0,WHITE);
			x= x+1;
		}
	}
	else
	{
		float m=dy/dx;
		if(dy<=dx&&dy>0)          // 0 < m <= 1
		{
			d=2*dy-dx;
			incrE=2*dy;
			incrNE=2*(dy-dx);
			linedraw(x0,y0,x1,y1,1,INCR);
		}
		if(dy>dx&&dy>0)             // m > 1
		{
			d=2*dx-dy;
			incrE=2*dx;
			incrNE=2*(dx-dy);
			linedraw(x0,y0,x1,y1,0,INCR);
		}
		if(-dy<=dx&&dy<0)          // -1 <= m < 0
		{
			dy=-dy;
			d=2*dy-dx;
			incrE=2*dy;
			incrNE=2*(dy-dx);
			linedraw(x0,y0,x1,y1,1,DECR);
		}
		if(-dy>dx&&dy<0)           // m < -1
		{
			dx=-dx;
			d=2*dx-dy;
			incrE=-(2*dx);
			incrNE=-2*(dx-dy);
			linedraw(x1,y1,x0,y0,0,DECR);
		}
	}

}

int main()
{
	int gd=DETECT, gm;
	clrscr();
	printf("\n\nPROGRAM TO DRAW A LINE USING BRESENHAM'S ALGORITHM");
	input();
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	cleardevice();
	bres_line();
	getch();
	closegraph();

	return 0;
}