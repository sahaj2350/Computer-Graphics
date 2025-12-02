#include <stdio.h>
#include <math.h>

void multiply(float T[4][4], float P[4][1], float R[4][1]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        R[i][0] = 0;
        for (j = 0; j < 4; j++) {
            R[i][0] += T[i][j] * P[j][0];
        }
    }
}

int main() {
    int choice, i, j;
    float x, y, z;

    float T[4][4], R[4][1], P[4][1];
    float sx, sy, sz, tx, ty, tz;
    float angle, rad;
    float shxy, shxz, shyx, shyz, shzx, shzy;
    float d;

    while (1) {
        printf("\n===== 3D TRANSFORMATION MENU =====\n");
        printf("1. Scaling Up\n");
        printf("2. Scaling Down\n");
        printf("3. Translation\n");
        printf("4. Rotation about X-axis\n");
        printf("5. Rotation about Y-axis\n");
        printf("6. Shearing\n");
        printf("7. Reflection\n");
        printf("8. Parallel Projection\n");
        printf("9. Perspective Projection\n");
        printf("10. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 10)
            break;

        printf("Enter 3D point (x y z): ");
        scanf("%f %f %f", &x, &y, &z);

        /* Initialize point vector */
        P[0][0] = x;
        P[1][0] = y;
        P[2][0] = z;
        P[3][0] = 1;

        /* Initialize Identity Matrix */
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                T[i][j] = (i == j) ? 1 : 0;

        switch (choice) {

        case 1: // Scaling Up
            printf("Enter scale factors (sx sy sz): ");
            scanf("%f %f %f", &sx, &sy, &sz);
            T[0][0] = sx;
            T[1][1] = sy;
            T[2][2] = sz;
            break;

        case 2: // Scaling Down
            printf("Enter scale factors (sx sy sz): ");
            scanf("%f %f %f", &sx, &sy, &sz);
            T[0][0] = 1/sx; 
            T[1][1] = 1/sy;
            T[2][2] = 1/sz;
            break;

        case 3: // Translation
            printf("Enter translation (tx ty tz): ");
            scanf("%f %f %f", &tx, &ty, &tz);
            T[0][3] = tx;
            T[1][3] = ty;
            T[2][3] = tz;
            break;

        case 4: // Rotation about X
            printf("Enter angle: ");
            scanf("%f", &angle);
            rad = angle * M_PI / 180;
            T[1][1] = cos(rad);
            T[1][2] = -sin(rad);
            T[2][1] = sin(rad);
            T[2][2] = cos(rad);
            break;

        case 5: // Rotation about Y
            printf("Enter angle: ");
            scanf("%f", &angle);
            rad = angle * M_PI / 180;
            T[0][0] = cos(rad);
            T[0][2] = sin(rad);
            T[2][0] = -sin(rad);
            T[2][2] = cos(rad);
            break;

        case 6: // Shearing
            printf("Enter shearing factors shxy shxz shyx shyz shzx shzy:\n");
            scanf("%f %f %f %f %f %f",
                  &shxy, &shxz, &shyx, &shyz, &shzx, &shzy);

            T[0][1] = shxy;
            T[0][2] = shxz;
            T[1][0] = shyx;
            T[1][2] = shyz;
            T[2][0] = shzx;
            T[2][1] = shzy;
            break;

        case 7: // Reflection
            printf("1. XY-plane\n2. YZ-plane\n3. ZX-plane\nEnter: ");
            scanf("%d", &i);
            if (i == 1) T[2][2] = -1;
            else if (i == 2) T[0][0] = -1;
            else T[1][1] = -1;
            break;

        case 8: // Parallel Projection
            printf("Enter direction ratios (dx dy dz): ");
            scanf("%f %f %f", &sx, &sy, &sz);
            T[0][2] = -sx;
            T[1][2] = -sy;
            T[2][2] = -sz;
            break;

        case 9: // Perspective Projection
            printf("Enter distance d from origin: ");
            scanf("%f", &d);
            T[3][2] = -1/d;
            break;

        default:
            printf("Invalid choice!\n");
            continue;
        }

        /* Perform matrix multiplication */
        multiply(T, P, R);

        /* Normalize if perspective projection */
        if (choice == 9) {
            R[0][0] /= R[3][0];
            R[1][0] /= R[3][0];
            R[2][0] /= R[3][0];
        }

        printf("\nTransformed Point: (%.3f, %.3f, %.3f)\n",
               R[0][0], R[1][0], R[2][0]);
    }

    return 0;
}
