#include <iostream.h>
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>

void circle1(int, int, int);
void circlepoints(int, int, int, int, int);

void main(void)
{
    int gdriver = DETECT, gmode;
    int r;
    int c1, c2;

    initgraph(&gdriver, &gmode, "C:\\TURBOC3\\BGI");

    cout << "Enter the x coordinate for center: ";
    cin >> c1;
    cout << "\nEnter the y coordinate for center: ";
    cin >> c2;
    cout << "\nEnter the radius: ";
    cin >> r;

    cleardevice();  // clears the graphics screen (use this instead of clrscr)
    circle1(c1, c2, r);

    getch();
    closegraph();
}

void circle1(int c1, int c2, int r1)
{
    int x = 0;
    int y = r1;
    float d = 1.25 - r1;  // initial decision parameter

    circlepoints(x, y, 12, c1, c2);
    while (x < y)
    {
        if (d < 0)
            d += 2 * x + 3;
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        circlepoints(x, y, 12, c1, c2);
    }
}

void circlepoints(int x, int y, int color, int xc, int yc)
{
    // 8-way symmetry points
    putpixel(xc + x, yc + y, color);
    putpixel(xc - x, yc + y, color);
    putpixel(xc + x, yc - y, color);
    putpixel(xc - x, yc - y, color);
    putpixel(xc + y, yc + x, color);
    putpixel(xc - y, yc + x, color);
    putpixel(xc + y, yc - x, color);
    putpixel(xc - y, yc - x, color);
}
