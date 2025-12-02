#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

int main() 
{
    int gd = DETECT, gm;
    int x0, y0, x1, y1;
    int rx0, ry0, rx1, ry1;
    double t;
    int px, py;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    printf("Enter P0 (x0 y0): ");
    scanf("%d %d", &x0, &y0);

    printf("Enter P1 (x1 y1): ");
    scanf("%d %d", &x1, &y1);

    printf("Enter tangent at P0 (rx0 ry0): ");
    scanf("%d %d", &rx0, &ry0);

    printf("Enter tangent at P1 (rx1 ry1): ");
    scanf("%d %d", &rx1, &ry1);

    for (t = 0; t <= 1; t += 0.001) {

        double h1 = 2*t*t*t - 3*t*t + 1;
        double h2 = -2*t*t*t + 3*t*t;
        double h3 = t*t*t - 2*t*t + t;
        double h4 = t*t*t - t*t;

        px = (int)(h1*x0 + h2*x1 + h3*rx0 + h4*rx1);
        py = (int)(h1*y0 + h2*y1 + h3*ry0 + h4*ry1);

        putpixel(px, py, WHITE);
    }

    getch();
    closegraph();
    return 0;
}
