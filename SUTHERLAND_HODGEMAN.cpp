 /*************************************************************************/
 /*************************************************************************

 A C++ program to show the implementation of Sutherland-Hodgeman Polygon
 Clipping Algorithm.


  *************************************************************************/

 /*************************************************************************/
 /*************************************************************************/
 //---------------------------  Header Files  ----------------------------//
 /*************************************************************************/
 /*************************************************************************/

 # include <iostream.h>
 # include <graphics.h>
 # include <conio.h>
 # include <math.h>

 /*************************************************************************/
 /*************************************************************************/
 //-----------------  Class Declarations & Definitions  ------------------//
 /*************************************************************************/
 /*************************************************************************/

 /*************************************************************************/
 //--------------------------  PointCoordinates  -------------------------//
 /*************************************************************************/

 class PointCoordinates
    {
       public:
	  float x;
	  float y;

	  PointCoordinates( )
	     {
		x=0;
		y=0;
	     }
    };

 /*************************************************************************/
 //---------------------------  LineCoordinates  -------------------------//
 /*************************************************************************/

 class LineCoordinates
    {
       public:
	  float x_1;
	  float y_1;
	  float x_2;
	  float y_2;

	  LineCoordinates( )
	     {
		x_1=0;
		y_1=0;
		x_2=0;
		y_2=0;
	     }

	  LineCoordinates(const float x1,const float y1,
					      const float x2,const float y2)
	     {
		x_1=x1;
		y_1=y1;
		x_2=x2;
		y_2=y2;
	     }
    };

 /*************************************************************************/
 //-------------------------  WindowCoordinates  -------------------------//
 /*************************************************************************/

 class WindowCoordinates
    {
       public:
	  float x_min;
	  float y_min;
	  float x_max;
	  float y_max;

	  WindowCoordinates(const float x1,const float y1,
					      const float x2,const float y2)
	     {
		x_min=x1;
		y_min=y1;
		x_max=x2;
		y_max=y2;
	     }
    };

 /*************************************************************************/
 /*************************************************************************/
 //-----------------------  Function Prototypes  -------------------------//
 /*************************************************************************/
 /*************************************************************************/

 void show_screen( );

 void clip_polygon(const WindowCoordinates,const int,const int []);

 const int check_line(const LineCoordinates,const LineCoordinates);
 const int check_point(const LineCoordinates,const float,const float);

 const PointCoordinates get_intersection_point(LineCoordinates,
							    LineCoordinates);

 void Polygon(const int,const int []);
 void Rectangle(const int,const int,const int,const int);
 void Line(const int,const int,const int,const int);

 /*************************************************************************/
 /*************************************************************************/
 //------------------------------  main( )  ------------------------------//
 /*************************************************************************/
 /*************************************************************************/

 int main( )
    {
       int driver=VGA;
       int mode=VGAHI;

       initgraph(&driver,&mode,"C:\\TURBOC3\\BGI");

       show_screen( );

       WindowCoordinates WC(150,120,490,370);

       setcolor(15);
	 Rectangle(WC.x_min,WC.y_min,WC.x_max,WC.y_max);

       int n=10;

       int polygon_vertices[20]={ 120,260 , 170,380 , 220,300 , 390,395 ,
				  540,360 , 500,290 , 510,80  , 280,180 ,
				  210,90  , 100,240 };

       setcolor(7);
	 Polygon(n,polygon_vertices);

       setcolor(15);
       settextstyle(0,0,1);
	 outtextxy(300,240,"Window");
	 outtextxy(80,240,"Polygon");

       char Key=NULL;

       do
	  {
	     Key=getch( );
	  }
       while(Key!='C' && Key!='c');

       settextstyle(0,0,1);
	 setcolor(0);
	   outtextxy(163,450,"  Press 'C' to see the Clipped Polygon. ");

	setcolor(15);
	   outtextxy(163,450,"------                          ---------");

	 setcolor(12);
	   outtextxy(213,450,"  Press any Key to exit.  ");

       clip_polygon(WC,n,polygon_vertices);

       setcolor(15);
       settextstyle(0,0,1);
	 outtextxy(280,155,"Clipped Polygon");

       getch( );
       return 0;
    }

 /*************************************************************************/
 /*************************************************************************/
 //------------------------  Funcion Definitions  ------------------------//
 /*************************************************************************/
 /*************************************************************************/

 /*************************************************************************/
 //--------------------------  clip_polygon( )  --------------------------//
 /*************************************************************************/

 void clip_polygon(const WindowCoordinates wc,const int n,
						 const int polygon_edges[])
    {
       int edges_counter;
       int number_of_edges=n;

       int *edges=new int[(n*4)];
       int *clipped_edges=new int[(n*4)];

       for(int count_1=0;count_1<(n*2);count_1++)
	  edges[count_1]=polygon_edges[count_1];

       LineCoordinates window_line;

       for(int count_2=1;count_2<=4;count_2++)
	  {
	     edges_counter=1;

	     for(int count_3=0;count_3<(number_of_edges*4);count_3++)
		clipped_edges[count_3]=0;

	     if(count_2==1)
		{
		   window_line.x_1=wc.x_min;
		   window_line.y_1=wc.y_max;
		   window_line.x_2=wc.x_max;
		   window_line.y_2=wc.y_max;
		}

	     else if(count_2==2)
		{
		   window_line.x_1=wc.x_max;
		   window_line.y_1=wc.y_max;
		   window_line.x_2=wc.x_max;
		   window_line.y_2=wc.y_min;
		}

	     else if(count_2==3)
		{
		   window_line.x_1=wc.x_max;
		   window_line.y_1=wc.y_min;
		   window_line.x_2=wc.x_min;
		   window_line.y_2=wc.y_min;
		}

	     else if(count_2==4)
		{
		   window_line.x_1=wc.x_min;
		   window_line.y_1=wc.y_min;
		   window_line.x_2=wc.x_min;
		   window_line.y_2=wc.y_max;
		}

	     int rule_no;

	     PointCoordinates point;

	     for(int count_4=1;count_4<number_of_edges;count_4++)
		{
		   LineCoordinates line(edges[((count_4*2)-2)],
					edges[((count_4*2)-1)],
					edges[(count_4*2)],
					edges[((count_4*2)+1)]);

		   rule_no=check_line(window_line,line);

		   switch(rule_no)
		      {
			 case 1 : clipped_edges[(edges_counter*2)]=
							 (int)(line.x_2+0.5);
				  clipped_edges[((edges_counter*2)+1)]=
							 (int)(line.y_2+0.5);
				  edges_counter++;
				  break;

			 case 2 : break;

			 case 3 : point=
				    get_intersection_point(window_line,line);

				  clipped_edges[(edges_counter*2)]=
							  (int)(point.x+0.5);
				  clipped_edges[((edges_counter*2)+1)]=
							  (int)(point.y+0.5);
				  edges_counter++;
				  break;

			 case 4 : point=
				    get_intersection_point(window_line,line);

				  clipped_edges[(edges_counter*2)]=
							  (int)(point.x+0.5);
				  clipped_edges[((edges_counter*2)+1)]=
							  (int)(point.y+0.5);
				  edges_counter++;

				  clipped_edges[(edges_counter*2)]=
							 (int)(line.x_2+0.5);
				  clipped_edges[((edges_counter*2)+1)]=
							 (int)(line.y_2+0.5);
				  edges_counter++;
				  break;
		      }
		}

	     clipped_edges[0]=clipped_edges[((edges_counter*2)-2)];
	     clipped_edges[1]=clipped_edges[((edges_counter*2)-1)];

	     for(int count_5=0;count_5<(edges_counter*2);count_5++)
		edges[count_5]=0;

	     number_of_edges=edges_counter;

	     for(int count_6=0;count_6<(edges_counter*2);count_6++)
		edges[count_6]=clipped_edges[count_6];
	 }

       setcolor(10);
	 Polygon(number_of_edges,edges);

       delete edges;
       delete clipped_edges;
    }

 /*************************************************************************/
 //----------------------------  check_line( )  --------------------------//
 /*************************************************************************/

 const int check_line(const LineCoordinates line,const LineCoordinates edge)
    {
       int rule_number=0;
       int point_1=check_point(line,edge.x_1,edge.y_1);
       int point_2=check_point(line,edge.x_2,edge.y_2);

       if(point_1==1 && point_2==1)
	  rule_number=1;

       else if(point_1!=1 && point_2!=1)
	  rule_number=2;

       else if(point_1==1 && point_2!=1)
	  rule_number=3;

       else if(point_1!=1 && point_2==1)
	  rule_number=4;

       return rule_number;
    }

 /*************************************************************************/
 //----------------------------  check_point( )  -------------------------//
 /*************************************************************************/

 const int check_point(const LineCoordinates lc,const float x,const float y)
    {
       float c=(((lc.x_2-lc.x_1)*(y-lc.y_1))-((lc.y_2-lc.y_1)*(x-lc.x_1)));

       if(c<=0)
	  return 1;

       else
	  return 0;
    }

 /*************************************************************************/
 //----------------------  get_intersection_point( )  --------------------//
 /*************************************************************************/

 const PointCoordinates get_intersection_point(LineCoordinates lc1,
							LineCoordinates lc2)
    {
       float x_min=lc1.x_1;
       float x_max=lc1.x_2;
       float y_min=lc1.y_1;
       float y_max=lc1.y_2;

       if(lc1.x_1==lc1.x_2)
	  {
	     if(lc2.y_2>=lc2.y_1 && lc2.y_2>y_max)
		y_max=lc2.y_2;

	     else if(lc2.y_1>lc2.y_2 && lc2.y_1>y_max)
		y_max=lc2.y_1;

	     if(lc2.y_1<=lc2.y_2 && lc2.y_1<y_min)
		y_min=lc2.y_1;

	     else if(lc2.y_2<lc2.y_1 && lc2.y_2<y_min)
		y_min=lc2.y_2;
	  }

       else if(lc1.y_1==lc1.y_2)
	  {
	     if(lc2.x_2>=lc2.x_1 && lc2.x_2>x_max)
		x_max=lc2.x_2;

	     else if(lc2.x_2<lc2.x_1 && lc2.x_1>x_max)
		x_max=lc2.x_1;

	     if(lc2.x_1<=lc2.x_2 && lc2.x_1<x_min)
		x_min=lc2.x_1;

	     else if(lc2.x_2<lc2.x_1 && lc2.x_2<x_min)
		x_min=lc2.x_2;
	  }

       float x_mid;
       float y_mid;

       if(lc2.y_1>y_max)
	  {
	     while(lc2.y_1!=y_max)
		{
		   x_mid=((lc2.x_1+lc2.x_2)/2);
		   y_mid=((lc2.y_1+lc2.y_2)/2);

		   if(y_mid>=y_max)
		      {
			 lc2.x_1=x_mid;
			 lc2.y_1=y_mid;
		      }

		   else
		      {
			 lc2.x_2=x_mid;
			 lc2.y_2=y_mid;
		      }
		}
	  }

       else if(lc2.y_1<y_min)
	  {
	     while(lc2.y_1!=y_min)
		{
		   x_mid=((lc2.x_1+lc2.x_2)/2);
		   y_mid=((lc2.y_1+lc2.y_2)/2);

		   if(y_mid<=y_min)
		      {
			 lc2.x_1=x_mid;
			 lc2.y_1=y_mid;
		      }

		   else
		      {
			 lc2.x_2=x_mid;
			 lc2.y_2=y_mid;
		      }
		}
	  }

       if(lc2.x_1>x_max)
	  {
	     while(lc2.x_1!=x_max)
		{
		   x_mid=((lc2.x_1+lc2.x_2)/2);
		   y_mid=((lc2.y_1+lc2.y_2)/2);

		   if(x_mid>=x_max)
		      {
			 lc2.x_1=x_mid;
			 lc2.y_1=y_mid;
		      }

		   else
		      {
			 lc2.x_2=x_mid;
			 lc2.y_2=y_mid;
		      }
		}
	  }

       else if(lc2.x_1<x_min)
	  {
	     while(lc2.x_1!=x_min)
		{
		   x_mid=((lc2.x_1+lc2.x_2)/2);
		   y_mid=((lc2.y_1+lc2.y_2)/2);

		   if(x_mid<=x_min)
		      {
			 lc2.x_1=x_mid;
			 lc2.y_1=y_mid;
		      }

		   else
		      {
			 lc2.x_2=x_mid;
			 lc2.y_2=y_mid;
		      }
		}
	  }

       PointCoordinates ipt;

       ipt.x=lc2.x_1;
       ipt.y=lc2.y_1;

       return ipt;
    }


 /*************************************************************************/
 //---------------------------  Rectangle( )  ----------------------------//
 /*************************************************************************/

 void Rectangle(const int x_1,const int y_1,const int x_2,const int y_2)
    {
       Line(x_1,y_1,x_2,y_1);
       Line(x_2,y_1,x_2,y_2);
       Line(x_2,y_2,x_1,y_2);
       Line(x_1,y_2,x_1,y_1);
    }

 /*************************************************************************/
 //-----------------------------  Polygon( )  ----------------------------//
 /*************************************************************************/

 void Polygon(const int n,const int coordinates[])
    {
       if(n>=2)
	  {
	     Line(coordinates[0],coordinates[1],
					     coordinates[2],coordinates[3]);

	     for(int count=1;count<(n-1);count++)
		Line(coordinates[(count*2)],coordinates[((count*2)+1)],
					    coordinates[((count+1)*2)],
					    coordinates[(((count+1)*2)+1)]);
	  }
    }

 /*************************************************************************/
 //--------------------------  Line( )  ------------------------//
 /*************************************************************************/

 void Line(const int x_1,const int y_1,const int x_2,const int y_2)
    {
       int color=getcolor( );

       int x1=x_1;
       int y1=y_1;

       int x2=x_2;
       int y2=y_2;

       if(x_1>x_2)
	  {
	     x1=x_2;
	     y1=y_2;

	     x2=x_1;
	     y2=y_1;
	  }

       int dx=abs(x2-x1);
       int dy=abs(y2-y1);
       int inc_dec=((y2>=y1)?1:-1);

       if(dx>dy)
	  {
	     int two_dy=(2*dy);
	     int two_dy_dx=(2*(dy-dx));
	     int p=((2*dy)-dx);

	     int x=x1;
	     int y=y1;

	     putpixel(x,y,color);

	     while(x<x2)
		{
		   x++;

		   if(p<0)
		      p+=two_dy;

		   else
		      {
			 y+=inc_dec;
			 p+=two_dy_dx;
		      }

		   putpixel(x,y,color);
		}
	  }

       else
	  {
	     int two_dx=(2*dx);
	     int two_dx_dy=(2*(dx-dy));
	     int p=((2*dx)-dy);

	     int x=x1;
	     int y=y1;

	     putpixel(x,y,color);

	     while(y!=y2)
		{
		   y+=inc_dec;

		   if(p<0)
		      p+=two_dx;

		   else
		      {
			 x++;
			 p+=two_dx_dy;
		      }

		   putpixel(x,y,color);
		}
	  }
    }

 /*************************************************************************/
 //--------------------------  show_screen( )  ---------------------------//
 /*************************************************************************/

 void show_screen( )
    {
       setfillstyle(1,1);
	 bar(125,26,510,38);

       settextstyle(0,0,1);
	 setcolor(15);
	   outtextxy(5,5,"******************************************************************************");
	   outtextxy(5,17,"*-**************************************************************************-*");
	   outtextxy(5,29,"*-------------                                                  -------------*");
	   outtextxy(5,41,"*-**************************************************************************-*");
	   outtextxy(5,53,"*-**************************************************************************-*");

	 setcolor(11);
	   outtextxy(134,29,"Sutherland-Hodgeman Polygon Clipping Algorithm");

	 setcolor(15);

	   for(int count=0;count<=30;count++)
	      outtextxy(5,(65+(count*12)),"*-*                                                                        *-*");

	   outtextxy(5,438,"*-**************************************************************************-*");
	   outtextxy(5,450,"*-------------------                                         ----------------*");
	   outtextxy(5,462,"******************************************************************************");

	 setcolor(12);
	   outtextxy(163,450,"  Press 'C' to see the Clipped Polygon. ");
    }

 /*************************************************************************/
 /*************************************************************************/
 //-----------------------------  THE END  -------------------------------//
 /*************************************************************************/
 /*************************************************************************/
