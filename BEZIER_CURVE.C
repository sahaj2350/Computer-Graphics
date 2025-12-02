#include <graphics.h>
#include <conio.h>
#include <math.h>

int main() {
    int gd = DETECT, gm;
    int x[4], y[4];
    double t;
    int px, py;
    int i;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    printf("Enter 4 control points (x y): \n");
    for (i = 0; i < 4; i++) {
        scanf("%d %d", &x[i], &y[i]);
    }

    for (t = 0; t <= 1; t += 0.001) {
        px = pow(1 - t, 3) * x[0] +
             3 * t * pow(1 - t, 2) * x[1] +
             3 * t * t * (1 - t) * x[2] +
             t * t * t * x[3];

        py = pow(1 - t, 3) * y[0] +
             3 * t * pow(1 - t, 2) * y[1] +
             3 * t * t * (1 - t) * y[2] +
             t * t * t * y[3];

        putpixel(px, py, WHITE);
    }

    getch();
    closegraph();
    return 0;
}
