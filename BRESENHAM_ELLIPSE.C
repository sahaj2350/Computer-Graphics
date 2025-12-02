/* bresenham_ellipse_turboc.c
   Integer Bresenham / Midpoint ellipse (outline only)
   Turbo C++ / Borland BGI compatible.
   Compile & run in Turbo C++ 3.0 or equivalent BGI environment.
*/

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void plot4(int xc, int yc, int x, int y, int color) {
    putpixel(xc + x, yc + y, color);
    putpixel(xc - x, yc + y, color);
    putpixel(xc + x, yc - y, color);
    putpixel(xc - x, yc - y, color);
}

/* Integer Bresenham / Midpoint ellipse outline */
void bresenham_ellipse(int xc, int yc, int rx, int ry, int color) {
    long long rx2 = (long long)rx * rx;
    long long ry2 = (long long)ry * ry;

    long long x = 0;
    long long y = ry;

    /* d1 scaled by 4 to avoid fractions: d1 = 4*ry2 - 4*rx2*ry + rx2 */
    long long d1 = 4 * ry2 - 4 * rx2 * ry + rx2;
    long long dx = 0;                 /* 4 * x * ry2 (initially 0) */
    long long dy = 4 * rx2 * y;       /* 4 * y * rx2 */

// Region 2 long lon
long long d2 = 4 * ry2 * (x + 1) * (x + 1) + 4 * rx2 * (y - 1) * (y - 1) - 4 * rx2 * ry2;

    /* Region 1: slope magnitude < 1 (move in x, maybe decrement y) */
    while (dx < dy) {
        plot4(xc, yc, (int)x, (int)y, color);

        if (d1 < 0) {
            /* move east */
            x += 1;
            dx += 8 * ry2;            /* dx += 4*(2*ry2) */
            d1 += dx + 4 * ry2;
        } else {
            /* move south-east */
            x += 1;
            y -= 1;
            dx += 8 * ry2;
            dy -= 8 * rx2;
            d1 += dx - dy + 4 * ry2;
        }
    }

    /* Region 2: slope magnitude >= 1 (move in y, maybe increment x) */
    /* d2 init (scaled by 4): 4*ry2*(x+0.5)^2 + 4*rx2*(y-1)^2 - 4*rx2*ry2
       To keep integers we can use the equivalent initialization below. */

    while (y >= 0) {
        plot4(xc, yc, (int)x, (int)y, color);

        if (d2 > 0) {
            /* move south */
            y -= 1;
            dy -= 8 * rx2;
            d2 += 4 * rx2 - dy;
        } else {
            /* move south-east */
            y -= 1;
            x += 1;
            dx += 8 * ry2;
            dy -= 8 * rx2;
            d2 += dx - dy + 4 * rx2;
        }
    }
}

int main(void) {
    int gdriver = DETECT, gmode;
    int xc, yc, rx, ry;
    int color = WHITE;

    printf("Enter center (xc yc): ");
    if (scanf("%d %d", &xc, &yc) != 2) {
        printf("Invalid input.\n");
        return 1;
    }
    printf("Enter radii (rx ry): ");
    if (scanf("%d %d", &rx, &ry) != 2) {
        printf("Invalid input.\n");
        return 1;
    }

    /* initialize BGI - put path if needed, e.g. "C:\\TURBOC3\\BGI" */
    initgraph(&gdriver, &gmode, "C:\\TURBOC3\\BGI");
    if (graphresult() != grOk) {
        printf("Graphics initialization failed: %s\n", grapherrormsg(graphresult()));
        return 1;
    }

    cleardevice();

    /* draw axes for reference (optional) */
    setcolor(DARKGRAY);
    line(0, yc, getmaxx(), yc);
    line(xc, 0, xc, getmaxy());

    setcolor(color);
    bresenham_ellipse(xc, yc, rx, ry, color);

    getch();
    closegraph();
    return 0;
}
