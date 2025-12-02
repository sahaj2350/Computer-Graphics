#include <stdio.h>
#include <math.h>

void multiplyMatrix(float a[3][3], float b[3][1], float res[3][1]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        res[i][0] = 0;
        for (j = 0; j < 3; j++) {
            res[i][0] += a[i][j] * b[j][0];
        }
    }
}

int main() {

    /* ALL declarations MUST be at the top */
    float x, y;
    int choice;
    
    float sx, sy, tx, ty, angle, rad, shx, shy;
    int axis;
    int i, j;

    float t[3][3];
    float trans[3][3];
    float point[3][1];
    float result[3][1];

    while (1) {
        printf("\n===== 2D TRANSFORMATION MENU =====\n");
        printf("1. Scaling Up\n");
        printf("2. Scaling Down\n");
        printf("3. Translation\n");
        printf("4. Rotation\n");
        printf("5. Shearing\n");
        printf("6. Reflection\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 7)
            break;

        printf("Enter point (x y): ");
        scanf("%f %f", &x, &y);

        /* initialize point */
        point[0][0] = x;
        point[1][0] = y;
        point[2][0] = 1;

        switch (choice) {

        case 1:
            printf("Enter scaling factors (sx sy): ");
            scanf("%f %f", &sx, &sy);

            t[0][0] = sx; t[0][1] = 0;  t[0][2] = 0;
            t[1][0] = 0;  t[1][1] = sy; t[1][2] = 0;
            t[2][0] = 0;  t[2][1] = 0;  t[2][2] = 1;
            break;

        case 2:
            printf("Enter scaling factors (sx sy): ");
            scanf("%f %f", &sx, &sy);

            t[0][0] = 1/sx; t[0][1] = 0;     t[0][2] = 0;
            t[1][0] = 0;    t[1][1] = 1/sy; t[1][2] = 0;
            t[2][0] = 0;    t[2][1] = 0;    t[2][2] = 1;
            break;

        case 3:
            printf("Enter translation (tx ty): ");
            scanf("%f %f", &tx, &ty);

            t[0][0] = 1; t[0][1] = 0; t[0][2] = tx;
            t[1][0] = 0; t[1][1] = 1; t[1][2] = ty;
            t[2][0] = 0; t[2][1] = 0; t[2][2] = 1;
            break;

        case 4:
            printf("Enter angle: ");
            scanf("%f", &angle);
            rad = angle * M_PI / 180;

            t[0][0] = cos(rad);  t[0][1] = -sin(rad); t[0][2] = 0;
            t[1][0] = sin(rad);  t[1][1] = cos(rad);  t[1][2] = 0;
            t[2][0] = 0;         t[2][1] = 0;         t[2][2] = 1;
            break;

        case 5:
            printf("Enter shearing factors (shx shy): ");
            scanf("%f %f", &shx, &shy);

            t[0][0] = 1;  t[0][1] = shx; t[0][2] = 0;
            t[1][0] = shy; t[1][1] = 1;  t[1][2] = 0;
            t[2][0] = 0;  t[2][1] = 0;   t[2][2] = 1;
            break;

        case 6:
            printf("1. X-axis\n2. Y-axis\n3. Origin\nEnter: ");
            scanf("%d", &axis);

            if (axis == 1) {
                t[0][0] = 1;  t[0][1] = 0;  t[0][2] = 0;
                t[1][0] = 0;  t[1][1] = -1; t[1][2] = 0;
                t[2][0] = 0;  t[2][1] = 0;  t[2][2] = 1;
            }
            else if (axis == 2) {
                t[0][0] = -1; t[0][1] = 0;  t[0][2] = 0;
                t[1][0] = 0;  t[1][1] = 1;  t[1][2] = 0;
                t[2][0] = 0;  t[2][1] = 0;  t[2][2] = 1;
            }
            else {
                t[0][0] = -1; t[0][1] = 0;  t[0][2] = 0;
                t[1][0] = 0;  t[1][1] = -1; t[1][2] = 0;
                t[2][0] = 0;  t[2][1] = 0;  t[2][2] = 1;
            }
            break;

        default:
            printf("Invalid choice!\n");
            continue;
        }

        /* copy t to trans */
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                trans[i][j] = t[i][j];

        multiplyMatrix(trans, point, result);

        printf("\nTransformed Point: (%.2f, %.2f)\n",
               result[0][0], result[1][0]);
    }

    return 0;
}
