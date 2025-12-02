/* 2D Object Transformation + Plotting (BGI)
   Minor fix: clear graphics buffer (cleardevice) right before drawing
   so the earlier menu text (outtextxy) does not stay on top of the plot.
*/

#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <conio.h> /* for getch() */

#define MAXV 20

void multiplyMatrix(float a[3][3], float b[3][1], float res[3][1]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        res[i][0] = 0.0f;
        for (j = 0; j < 3; j++) {
            res[i][0] += a[i][j] * b[j][0];
        }
    }
}

/* portable rounding to nearest integer (round halves away from zero) */
static int roundf_to_int(float v) {
    if (v >= 0.0f) return (int)(v + 0.5f);
    else return (int)(v - 0.5f);
}

/* Convert Cartesian (x,y) to screen coords (sx, sy) with origin at center */
void cartesianToScreen(float x, float y, int *sx, int *sy, int ox, int oy) {
    *sx = ox + roundf_to_int(x);
    *sy = oy - roundf_to_int(y); /* invert y for screen coordinates */
}

int main() {
    /* --- All declarations first (Turbo C / old compilers require this) --- */
    int choice;
    int n = 0; /* number of vertices */
    float obj[MAXV][2];
    float transObj[MAXV][2];
    float t[3][3], tmp[3][1], res[3][1];
    int i, j;
    float sx, sy, tx, ty, angle, rad, shx, shy;
    int axis;

    /* BGI variables declared here */
    int gd = DETECT;
    int gm;
    int gerr;
    int ox, oy;

    /* initialize arrays to zero (optional but safer) */
    for (i = 0; i < MAXV; ++i) { obj[i][0]=obj[i][1]=0.0f; transObj[i][0]=transObj[i][1]=0.0f; }

    /* --- Now executable statements --- */
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI"); /* use "" for WinBGIm; set path for Turbo C if needed */

    /* check result */
    gerr = graphresult();
    if (gerr != grOk) {
        printf("Graphics initialization failed: %s\n", grapherrormsg(gerr));
        return 1;
    }

    /* get origin (center of screen) AFTER initgraph */
    ox = getmaxx() / 2;
    oy = getmaxy() / 2;

    while (1) {
        /* draw the menu in graphics (so user sees it while entering values) */
        cleardevice();
        setcolor(WHITE);
        outtextxy(10, 10, "===== 2D TRANSFORMATION MENU =====");
        outtextxy(10, 30, "1. Scaling Up");
        outtextxy(10, 50, "2. Scaling Down");
        outtextxy(10, 70, "3. Translation");
        outtextxy(10, 90, "4. Rotation");
        outtextxy(10, 110, "5. Shearing");
        outtextxy(10, 130, "6. Reflection");
        outtextxy(10, 150, "7. Exit");
        outtextxy(10, 180, "Enter choice (type in console): ");

        /* Menu selection from console */
        printf("\n===== 2D TRANSFORMATION MENU =====\n");
        printf("1. Scaling Up\n2. Scaling Down\n3. Translation\n4. Rotation\n5. Shearing\n6. Reflection\n7. Exit\n");
        printf("Enter choice: ");
        fflush(stdout);
        if (scanf("%d", &choice) != 1) { /* invalid input */
            printf("Invalid input. Exiting.\n");
            break;
        }
        if (choice == 7) break;

        /* Get polygon from user */
        printf("Enter number of vertices (max %d): ", MAXV);
        fflush(stdout);
        scanf("%d", &n);
        if (n < 1) n = 1;
        if (n > MAXV) n = MAXV;

        printf("Enter %d vertices (x y) in Cartesian coordinates (e.g. 0 0):\n", n);
        fflush(stdout);
        for (i = 0; i < n; i++) {
            printf("Vertex %d: ", i + 1);
            fflush(stdout);
            scanf("%f %f", &obj[i][0], &obj[i][1]);
        }

        /* Build transformation matrix t[][] based on choice */
        switch (choice) {
            case 1:
                printf("Enter scaling factors (sx sy): ");
                fflush(stdout);
                scanf("%f %f", &sx, &sy);
                t[0][0] = sx; t[0][1] = 0;  t[0][2] = 0;
                t[1][0] = 0;  t[1][1] = sy; t[1][2] = 0;
                t[2][0] = 0;  t[2][1] = 0;  t[2][2] = 1;
                break;

            case 2:
                printf("Enter scaling factors (sx sy): ");
                fflush(stdout);
                scanf("%f %f", &sx, &sy);
                if (sx == 0) sx = 1; if (sy == 0) sy = 1;
                t[0][0] = 1.0f/sx; t[0][1] = 0;         t[0][2] = 0;
                t[1][0] = 0;       t[1][1] = 1.0f/sy;   t[1][2] = 0;
                t[2][0] = 0;       t[2][1] = 0;         t[2][2] = 1;
                break;

            case 3:
                printf("Enter translation (tx ty): ");
                fflush(stdout);
                scanf("%f %f", &tx, &ty);
                t[0][0] = 1; t[0][1] = 0; t[0][2] = tx;
                t[1][0] = 0; t[1][1] = 1; t[1][2] = ty;
                t[2][0] = 0; t[2][1] = 0; t[2][2] = 1;
                break;

            case 4:
                printf("Enter rotation angle in degrees (positive anticlockwise): ");
                fflush(stdout);
                scanf("%f", &angle);
                rad = angle * M_PI / 180.0f;
                t[0][0] = cos(rad);  t[0][1] = -sin(rad); t[0][2] = 0;
                t[1][0] = sin(rad);  t[1][1] = cos(rad);  t[1][2] = 0;
                t[2][0] = 0;         t[2][1] = 0;         t[2][2] = 1;
                break;

            case 5:
                printf("Enter shearing factors (shx shy): ");
                fflush(stdout);
                scanf("%f %f", &shx, &shy);
                t[0][0] = 1;  t[0][1] = shx; t[0][2] = 0;
                t[1][0] = shy; t[1][1] = 1;  t[1][2] = 0;
                t[2][0] = 0;  t[2][1] = 0;   t[2][2] = 1;
                break;

            case 6:
                printf("1. Reflect about X-axis\n2. Reflect about Y-axis\n3. Reflect about Origin\nEnter: ");
                fflush(stdout);
                scanf("%d", &axis);
                if (axis == 1) {
                    t[0][0] = 1;  t[0][1] = 0;  t[0][2] = 0;
                    t[1][0] = 0;  t[1][1] = -1; t[1][2] = 0;
                    t[2][0] = 0;  t[2][1] = 0;  t[2][2] = 1;
                } else if (axis == 2) {
                    t[0][0] = -1; t[0][1] = 0;  t[0][2] = 0;
                    t[1][0] = 0;  t[1][1] = 1;  t[1][2] = 0;
                    t[2][0] = 0;  t[2][1] = 0;  t[2][2] = 1;
                } else {
                    t[0][0] = -1; t[0][1] = 0;  t[0][2] = 0;
                    t[1][0] = 0;  t[1][1] = -1; t[1][2] = 0;
                    t[2][0] = 0;  t[2][1] = 0;  t[2][2] = 1;
                }
                break;

            default:
                printf("Invalid choice!\n");
                continue;
        }

        /* Apply transform to each vertex */
        for (i = 0; i < n; i++) {
            tmp[0][0] = obj[i][0];
            tmp[1][0] = obj[i][1];
            tmp[2][0] = 1.0f;
            multiplyMatrix(t, tmp, res);
            transObj[i][0] = res[0][0];
            transObj[i][1] = res[1][0];
        }

        /* --- Clear the menu text and draw the scene --- */
        cleardevice(); /* <--- this removes earlier menu drawn with outtextxy */

        /* Draw axes */
        setcolor(WHITE);
        line(0, oy, getmaxx(), oy); /* x-axis */
        line(ox, 0, ox, getmaxy()); /* y-axis */
        outtextxy(10, oy + 10, "Origin (0,0) at screen center");

        /* Draw original polygon in BLUE */
        setcolor(BLUE);
        for (i = 0; i < n; i++) {
            int x1, y1, x2, y2;
            cartesianToScreen(obj[i][0], obj[i][1], &x1, &y1, ox, oy);
            cartesianToScreen(obj[(i+1)%n][0], obj[(i+1)%n][1], &x2, &y2, ox, oy);
            line(x1, y1, x2, y2);
        }
        outtextxy(10, 200, "Original Object: BLUE");

        /* Draw transformed polygon in RED */
        setcolor(RED);
        for (i = 0; i < n; i++) {
            int x1, y1, x2, y2;
            cartesianToScreen(transObj[i][0], transObj[i][1], &x1, &y1, ox, oy);
            cartesianToScreen(transObj[(i+1)%n][0], transObj[(i+1)%n][1], &x2, &y2, ox, oy);
            line(x1, y1, x2, y2);
        }
        outtextxy(10, 220, "Transformed Object: RED");

        /* Print transformed coordinates to console */
        printf("\nTransformed vertices:\n");
        for (i = 0; i < n; i++) {
            printf("V%d -> (%.2f, %.2f)\n", i+1, transObj[i][0], transObj[i][1]);
        }
        fflush(stdout);
        printf("Press any key in graphics window to continue...\n");
        getch(); /* wait for key press in graphics window */
    }

    closegraph();
    return 0;
}
