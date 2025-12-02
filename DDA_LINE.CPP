//PROGRAM TO DRAW A LINE USING DDA ALGORITHM

#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<math.h>
#include<iostream.h>

//FUNCTION TO DRAW A LINE USING DDA ALGORITHM
float x0,y0;
float x1,y1;
void input()
{

	cout <<endl<<endl<<"ENTER THE FIRST COORDINATE\n";
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
void dda_line()
{
	float dx=x1-x0;
	float dy=y1-y0;

	float x=x0;
	float y=y0;

	if(dx==0)          // VERTICAL LINE
	{
		while(y<=y1)
		{
			putpixel(x0,y,WHITE);
			y++;
		}
	}
	else
	if(dy==0)          // HORIZONTAL LINE
	{
		while(x<=x1)
		{
			putpixel(x,y0,WHITE);
			x++;
		}
	}
	//Dy = dx
	else
	{
		float m=dy/dx;
		if(abs(m)<=1)
		{
			while(x<=x1)
			{
				putpixel(x,int(y+0.5),WHITE);
				y+=m;
				x++;
			}
		}
		else
		{
			while(y<=y1)
			{
				putpixel(int(x+0.5),y,WHITE);
				x+=(1/m);
				y++;
			}
		}
	}
}

int main()
{
	int gd=DETECT, gm;
	clrscr();
	input();
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");  //graphics driver,mode,path to driver
	dda_line();
	getch();
	closegraph();

	return 0;
}