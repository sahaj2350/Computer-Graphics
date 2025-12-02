/* 3D Wireframe Transformer (Turbo C compatible)
   - No hard-coded figure: user enters vertices & edges
   - All declarations at top of main() to satisfy old compilers
   - Cast string literals to (char *) for BGI functions to avoid warnings
   - Original: BLUE, Transformed: RED
*/

#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>

#define MAXV 200
#define MAXE 400
#define EPS 1e-4f

/* ---- 4x4 helpers ---- */
void identity4(float M[4][4]) {
    int i,j;
    for (i=0;i<4;i++) for (j=0;j<4;j++) M[i][j] = (i==j)?1.0f:0.0f;
}

void mulMat4(float A[4][4], float B[4][4], float R[4][4]) {
    int i,j,k;
    float tmp[4][4];
    for (i=0;i<4;i++) for (j=0;j<4;j++) {
        tmp[i][j] = 0.0f;
        for (k=0;k<4;k++) tmp[i][j] += A[i][k] * B[k][j];
    }
    for (i=0;i<4;i++) for (j=0;j<4;j++) R[i][j] = tmp[i][j];
}

void mulMat4Vec(float M[4][4], float v[4], float r[4]) {
    int i,j;
    for (i=0;i<4;i++) {
        r[i] = 0.0f;
        for (j=0;j<4;j++) r[i] += M[i][j] * v[j];
    }
}

/* ---- transform constructors ---- */
void makeScale(float sx, float sy, float sz, float M[4][4]) {
    identity4(M);
    M[0][0] = sx; M[1][1] = sy; M[2][2] = sz;
}
void makeTranslate(float tx, float ty, float tz, float M[4][4]) {
    identity4(M);
    M[0][3] = tx; M[1][3] = ty; M[2][3] = tz;
}
void makeRotateX_mat(float deg, float M[4][4]) {
    identity4(M);
    {
        float r = deg * (float)M_PI / 180.0f;
        M[1][1] = cos(r); M[1][2] = -sin(r);
        M[2][1] = sin(r); M[2][2] = cos(r);
    }
}
void makeRotateY_mat(float deg, float M[4][4]) {
    identity4(M);
    {
        float r = deg * (float)M_PI / 180.0f;
        M[0][0] = cos(r); M[0][2] = sin(r);
        M[2][0] = -sin(r); M[2][2] = cos(r);
    }
}
void makeRotateZ_mat(float deg, float M[4][4]) {
    identity4(M);
    {
        float r = deg * (float)M_PI / 180.0f;
        M[0][0] = cos(r); M[0][1] = -sin(r);
        M[1][0] = sin(r); M[1][1] = cos(r);
    }
}

/* ---- screen helpers ---- */
static int roundf_to_int(float v) { if (v >= 0.0f) return (int)(v + 0.5f); else return (int)(v - 0.5f); }
void cartToScreen2D(float x, float y, int *sx, int *sy, int ox, int oy) { *sx = ox + roundf_to_int(x); *sy = oy - roundf_to_int(y); }

/* ---- Program entry ---- */
int main() {
    /* --- All declarations at top (Turbo C compatibility) --- */
    int gd = DETECT, gm;
    int gerr;
    int ox, oy;

    int i, j, idx1, idx2;
    int nverts = 0, nedges = 0;

    float verts[MAXV][3];
    int edges[MAXE][2];
    float transformed[MAXV][3];

    float composite[4][4], M[4][4];

    int menu_choice = 0, add_choice = 0, proj_choice = 1;
    float sx, sy, sz, tx, ty, tz, ang;
    float shxy, shxz, shyx, shyz, shzx, shzy;
    float d;

    int a, b;
    float px1_f, py1_f, px2_f, py2_f;
    int sx1_i, sy1_i, sx2_i, sy2_i;
    float tmpv[4], resv[4];

    /* extra temporaries to avoid declaring inside blocks */
    float denom1, denom2;
    int sub_choice;

    /* initialize arrays */
    for (i = 0; i < MAXV; i++) for (j = 0; j < 3; j++) { verts[i][j] = transformed[i][j] = 0.0f; }
    for (i = 0; i < MAXE; i++) { edges[i][0] = edges[i][1] = 0; }

    /* init graphics: cast string literal to (char *) to avoid warnings */
    initgraph(&gd, &gm, (char *)"C:\\TURBOC3\\BGI"); /* for Turbo C replace "" with "C:\\TURBOC3\\BGI" if needed */
    gerr = graphresult();
    if (gerr != grOk) {
        printf("Graphics initialization failed: %s\n", grapherrormsg(gerr));
        return 1;
    }
    ox = getmaxx() / 2; oy = getmaxy() / 2;

    identity4(composite);

    while (1) {
        cleardevice();
        setcolor(WHITE);
        outtextxy(10,10, (char *)"3D Wireframe Transformer (no hard-coded figure)");
        outtextxy(10,30, (char *)"1. Enter object (vertices + edges) and transform now");
        outtextxy(10,50, (char *)"2. Add transforms to persistent composite");
        outtextxy(10,70, (char *)"3. Apply composite & draw (object must be entered first)");
        outtextxy(10,90, (char *)"4. Reset composite to identity");
        outtextxy(10,110, (char *)"5. Exit");
        outtextxy(10,130, (char *)"Original: BLUE  Transformed: RED");
        fflush(stdout);

        printf("\n=== MAIN MENU ===\n1.Enter object & transform now\n2.Add transform to persistent composite\n3.Apply composite & draw\n4.Reset composite\n5.Exit\nEnter choice: ");
        fflush(stdout);
        if (scanf("%d", &menu_choice) != 1) break;

        if (menu_choice == 5) break;

        /* ---------------- OPTION 1 ----------------
           Enter object then immediate transform sub-menu and plot
        */
        if (menu_choice == 1) {
            printf("Enter number of vertices (1..%d): ", MAXV); fflush(stdout);
            if (scanf("%d", &nverts) != 1) { printf("Bad input.\n"); break; }
            if (nverts < 1) nverts = 1;
            if (nverts > MAXV) nverts = MAXV;

            for (i = 0; i < nverts; i++) {
                printf("V%d x y z: ", i+1); fflush(stdout);
                scanf("%f %f %f", &verts[i][0], &verts[i][1], &verts[i][2]);
            }

            printf("Enter number of edges (0..%d): ", MAXE); fflush(stdout);
            if (scanf("%d", &nedges) != 1) nedges = 0;
            if (nedges < 0) nedges = 0;
            if (nedges > MAXE) nedges = MAXE;

            for (i = 0; i < nedges; i++) {
                printf("Edge %d: vertex_index1 vertex_index2 (1-based): ", i+1); fflush(stdout);
                scanf("%d %d", &idx1, &idx2);
                idx1--; idx2--; /* convert to 0-based */
                if (idx1 < 0) idx1 = 0; if (idx1 >= nverts) idx1 = nverts - 1;
                if (idx2 < 0) idx2 = 0; if (idx2 >= nverts) idx2 = nverts - 1;
                edges[i][0] = idx1; edges[i][1] = idx2;
            }

            /* immediate transform sub-menu for this entered object */
            identity4(composite);
            while (1) {
                printf("\nTransform sub-menu (for entered object):\n");
                printf("1.Scale 2.Translate 3.RotateX 4.RotateY 5.RotateZ\n");
                printf("6.Shear 7.Reflect 8.Apply & Plot 9.Cancel\n");
                printf("Enter choice: "); fflush(stdout);
                if (scanf("%d", &add_choice) != 1) { add_choice = 9; }

                if (add_choice == 9) { printf("Cancelled transforms for this object.\n"); break; }

                if (add_choice == 8) {
                    /* apply composite to vertices */
                    for (i = 0; i < nverts; i++) {
                        tmpv[0] = verts[i][0]; tmpv[1] = verts[i][1]; tmpv[2] = verts[i][2]; tmpv[3] = 1.0f;
                        mulMat4Vec(composite, tmpv, resv);
                        transformed[i][0] = resv[0]; transformed[i][1] = resv[1]; transformed[i][2] = resv[2];
                    }

                    /* projection choice */
                    printf("Projection: 1=Orthographic 2=Perspective\nEnter: "); fflush(stdout);
                    if (scanf("%d", &proj_choice) != 1) proj_choice = 1;
                    if (proj_choice == 2) {
                        printf("Enter distance d (e.g. 500): "); fflush(stdout);
                        if (scanf("%f", &d) != 1) d = 500.0f;
                        if (fabs(d) < EPS) d = 500.0f;
                    }

                    /* draw */
                    cleardevice();
                    setcolor(WHITE);
                    outtextxy(10,10, (char *)"Original: BLUE  Transformed: RED");

                    /* draw original (BLUE) */
                    setcolor(BLUE);
                    for (i = 0; i < nedges; i++) {
                        a = edges[i][0]; b = edges[i][1];
                        if (proj_choice == 1) {
                            px1_f = verts[a][0]; py1_f = verts[a][1]; px2_f = verts[b][0]; py2_f = verts[b][1];
                        } else {
                            denom1 = d - verts[a][2]; if (fabs(denom1) < EPS) denom1 = (denom1 < 0 ? -EPS : EPS);
                            denom2 = d - verts[b][2]; if (fabs(denom2) < EPS) denom2 = (denom2 < 0 ? -EPS : EPS);
                            px1_f = verts[a][0] * (d / denom1); py1_f = verts[a][1] * (d / denom1);
                            px2_f = verts[b][0] * (d / denom2); py2_f = verts[b][1] * (d / denom2);
                        }
                        cartToScreen2D(px1_f, py1_f, &sx1_i, &sy1_i, ox, oy);
                        cartToScreen2D(px2_f, py2_f, &sx2_i, &sy2_i, ox, oy);
                        line(sx1_i, sy1_i, sx2_i, sy2_i);
                    }

                    /* draw transformed (RED) */
                    setcolor(RED);
                    for (i = 0; i < nedges; i++) {
                        a = edges[i][0]; b = edges[i][1];
                        if (proj_choice == 1) {
                            px1_f = transformed[a][0]; py1_f = transformed[a][1]; px2_f = transformed[b][0]; py2_f = transformed[b][1];
                        } else {
                            denom1 = d - transformed[a][2]; if (fabs(denom1) < EPS) denom1 = (denom1 < 0 ? -EPS : EPS);
                            denom2 = d - transformed[b][2]; if (fabs(denom2) < EPS) denom2 = (denom2 < 0 ? -EPS : EPS);
                            px1_f = transformed[a][0] * (d / denom1); py1_f = transformed[a][1] * (d / denom1);
                            px2_f = transformed[b][0] * (d / denom2); py2_f = transformed[b][1] * (d / denom2);
                        }
                        cartToScreen2D(px1_f, py1_f, &sx1_i, &sy1_i, ox, oy);
                        cartToScreen2D(px2_f, py2_f, &sx2_i, &sy2_i, ox, oy);
                        line(sx1_i, sy1_i, sx2_i, sy2_i);
                    }

                    /* print transformed vertices */
                    printf("\nTransformed vertices:\n");
                    for (i = 0; i < nverts; i++) {
                        printf("V%d: (%.3f, %.3f, %.3f)\n", i+1, transformed[i][0], transformed[i][1], transformed[i][2]);
                    }
                    fflush(stdout);
                    printf("Press any key in graphics window to continue...\n"); fflush(stdout);
                    getch();
                    break; /* back to main menu */
                } /* end apply */

                /* add transform to composite */
                if (add_choice == 1) {
                    printf("Enter sx sy sz: "); fflush(stdout); scanf("%f %f %f", &sx, &sy, &sz);
                    makeScale(sx, sy, sz, M); mulMat4(M, composite, composite); printf("Scale added.\n");
                } else if (add_choice == 2) {
                    printf("Enter tx ty tz: "); fflush(stdout); scanf("%f %f %f", &tx, &ty, &tz);
                    makeTranslate(tx, ty, tz, M); mulMat4(M, composite, composite); printf("Translate added.\n");
                } else if (add_choice == 3) {
                    printf("Enter angle degrees (X): "); fflush(stdout); scanf("%f", &ang);
                    makeRotateX_mat(ang, M); mulMat4(M, composite, composite); printf("RotateX added.\n");
                } else if (add_choice == 4) {
                    printf("Enter angle degrees (Y): "); fflush(stdout); scanf("%f", &ang);
                    makeRotateY_mat(ang, M); mulMat4(M, composite, composite); printf("RotateY added.\n");
                } else if (add_choice == 5) {
                    printf("Enter angle degrees (Z): "); fflush(stdout); scanf("%f", &ang);
                    makeRotateZ_mat(ang, M); mulMat4(M, composite, composite); printf("RotateZ added.\n");
                } else if (add_choice == 6) {
                    printf("Enter shxy shxz shyx shyz shzx shzy: "); fflush(stdout);
                    scanf("%f %f %f %f %f %f", &shxy, &shxz, &shyx, &shyz, &shzx, &shzy);
                    identity4(M);
                    M[0][1] = shxy; M[0][2] = shxz;
                    M[1][0] = shyx; M[1][2] = shyz;
                    M[2][0] = shzx; M[2][1] = shzy;
                    mulMat4(M, composite, composite);
                    printf("Shear added.\n");
                } else if (add_choice == 7) {
                    printf("Reflect about: 1.XY-plane(z->-z) 2.YZ-plane(x->-x) 3.ZX-plane(y->-y): "); fflush(stdout);
                    scanf("%d", &a);
                    identity4(M);
                    if (a == 1) M[2][2] = -1;
                    else if (a == 2) M[0][0] = -1;
                    else M[1][1] = -1;
                    mulMat4(M, composite, composite);
                    printf("Reflection added.\n");
                } else {
                    printf("Invalid sub-menu choice.\n");
                }
            } /* end transform sub-menu */
        } /* end menu_choice 1 */

        /* ---------------- OPTION 2 ----------------
           Add transforms to a persistent composite (applies later with option 3)
        */
        else if (menu_choice == 2) {
            printf("Add transform to persistent composite (will be applied by option 3):\n");
            printf("1.Scale 2.Translate 3.RotateX 4.RotateY 5.RotateZ 6.Shear 7.Reflect\nEnter: "); fflush(stdout);
            if (scanf("%d", &add_choice) != 1) continue;
            if (add_choice == 1) { printf("sx sy sz: "); fflush(stdout); scanf("%f %f %f",&sx,&sy,&sz); makeScale(sx,sy,sz,M); mulMat4(M,composite,composite); }
            else if (add_choice == 2) { printf("tx ty tz: "); fflush(stdout); scanf("%f %f %f",&tx,&ty,&tz); makeTranslate(tx,ty,tz,M); mulMat4(M,composite,composite); }
            else if (add_choice == 3) { printf("angle deg X: "); fflush(stdout); scanf("%f",&ang); makeRotateX_mat(ang,M); mulMat4(M,composite,composite); }
            else if (add_choice == 4) { printf("angle deg Y: "); fflush(stdout); scanf("%f",&ang); makeRotateY_mat(ang,M); mulMat4(M,composite,composite); }
            else if (add_choice == 5) { printf("angle deg Z: "); fflush(stdout); scanf("%f",&ang); makeRotateZ_mat(ang,M); mulMat4(M,composite,composite); }
            else if (add_choice == 6) { printf("shxy shxz shyx shyz shzx shzy: "); fflush(stdout); scanf("%f %f %f %f %f %f",&shxy,&shxz,&shyx,&shyz,&shzx,&shzy); identity4(M); M[0][1]=shxy; M[0][2]=shxz; M[1][0]=shyx; M[1][2]=shyz; M[2][0]=shzx; M[2][1]=shzy; mulMat4(M,composite,composite); }
            else if (add_choice == 7) { printf("Reflect:1.XY 2.YZ 3.ZX: "); fflush(stdout); scanf("%d",&a); identity4(M); if (a==1) M[2][2]=-1; else if (a==2) M[0][0]=-1; else M[1][1]=-1; mulMat4(M,composite,composite); }
            else { printf("Invalid transform choice.\n"); }
            printf("Transform appended to persistent composite.\n");
        }

        /* ---------------- OPTION 3 ----------------
           Apply persistent composite to the currently entered object and draw
        */
        else if (menu_choice == 3) {
            if (nverts <= 0) { printf("No object defined. First use option 1 to enter an object.\n"); continue; }
            printf("Projection: 1=Orthographic 2=Perspective\nEnter: "); fflush(stdout);
            if (scanf("%d", &proj_choice) != 1) proj_choice = 1;
            if (proj_choice == 2) { printf("Enter distance d (e.g. 500): "); fflush(stdout); if (scanf("%f", &d) != 1) d = 500.0f; if (fabs(d) < EPS) d = 500.0f; }

            for (i = 0; i < nverts; i++) {
                tmpv[0] = verts[i][0]; tmpv[1] = verts[i][1]; tmpv[2] = verts[i][2]; tmpv[3] = 1.0f;
                mulMat4Vec(composite, tmpv, resv);
                transformed[i][0] = resv[0]; transformed[i][1] = resv[1]; transformed[i][2] = resv[2];
            }

            cleardevice();
            outtextxy(10,10, (char *)"Original: BLUE  Transformed: RED");

            setcolor(BLUE);
            for (i = 0; i < nedges; i++) {
                a = edges[i][0]; b = edges[i][1];
                if (proj_choice == 1) {
                    px1_f = verts[a][0]; py1_f = verts[a][1]; px2_f = verts[b][0]; py2_f = verts[b][1];
                } else {
                    denom1 = d - verts[a][2]; if (fabs(denom1) < EPS) denom1 = (denom1 < 0 ? -EPS : EPS);
                    denom2 = d - verts[b][2]; if (fabs(denom2) < EPS) denom2 = (denom2 < 0 ? -EPS : EPS);
                    px1_f = verts[a][0] * (d / denom1); py1_f = verts[a][1] * (d / denom1);
                    px2_f = verts[b][0] * (d / denom2); py2_f = verts[b][1] * (d / denom2);
                }
                cartToScreen2D(px1_f, py1_f, &sx1_i, &sy1_i, ox, oy);
                cartToScreen2D(px2_f, py2_f, &sx2_i, &sy2_i, ox, oy);
                line(sx1_i, sy1_i, sx2_i, sy2_i);
            }

            setcolor(RED);
            for (i = 0; i < nedges; i++) {
                a = edges[i][0]; b = edges[i][1];
                if (proj_choice == 1) {
                    px1_f = transformed[a][0]; py1_f = transformed[a][1]; px2_f = transformed[b][0]; py2_f = transformed[b][1];
                } else {
                    denom1 = d - transformed[a][2]; if (fabs(denom1) < EPS) denom1 = (denom1 < 0 ? -EPS : EPS);
                    denom2 = d - transformed[b][2]; if (fabs(denom2) < EPS) denom2 = (denom2 < 0 ? -EPS : EPS);
                    px1_f = transformed[a][0] * (d / denom1); py1_f = transformed[a][1] * (d / denom1);
                    px2_f = transformed[b][0] * (d / denom2); py2_f = transformed[b][1] * (d / denom2);
                }
                cartToScreen2D(px1_f, py1_f, &sx1_i, &sy1_i, ox, oy);
                cartToScreen2D(px2_f, py2_f, &sx2_i, &sy2_i, ox, oy);
                line(sx1_i, sy1_i, sx2_i, sy2_i);
            }

            printf("\nTransformed vertices:\n");
            for (i = 0; i < nverts; i++) {
                printf("V%d: (%.3f, %.3f, %.3f)\n", i+1, transformed[i][0], transformed[i][1], transformed[i][2]);
            }
            fflush(stdout);
            printf("Press any key in graphics window to continue...\n"); fflush(stdout);
            getch();
        } /* end option 3 */

        else if (menu_choice == 4) {
            identity4(composite);
            printf("Composite reset to identity.\n");
        }

        else {
            printf("Invalid choice.\n");
        }
    } /* end main loop */

    closegraph();
    return 0;
}
