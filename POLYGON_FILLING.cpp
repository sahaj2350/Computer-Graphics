#include <dos.h>
#include <stdio.h>
#include <graphics.h>
#include <iostream.h>
#include <conio.h>
#include <math.h>

/* Data structures */
struct Polygon {
    int x;
    int y;
};

struct edRecord {
    int YUpper;            // y_max (scanline at which this edge no longer active)
    float XIntersect;      // x at current scanline
    float dxperscan;       // delta x per scan (1/m)
    struct edRecord *Next;
};

typedef struct edRecord Edge;
typedef struct Polygon Nodes;

Edge *Active;   // active edge table (dummy header)
Edge **Edges;   // global edge table buckets by y_min

int Cntr = 8; // number of vertices in the polygon

/* Proper 8-vertex polygon (all coordinates inside typical 640x480) */
Nodes NodesObj[8] = {
    {200, 100},
    {260, 130},
    {300, 190},
    {260, 250},
    {200, 280},
    {140, 250},
    {100, 190},
    {140, 130}
};

/* Function prototypes */
void Scanfill();
void InsertEdge(Edge *edgeList, Edge *InitialEdge);
void BuildEdgeList(int, Nodes *);
void MakeEdgeRec(Nodes lower, Nodes upper, Edge *CurrEdge);
void BulidActiveList(int);
void UpdateActiveList(int);
void DeleteAfter(Edge *);
void ResortActiveList(void);
void ResortActiveList(int);

/* Helper: safe swap for ints */
inline void swap_int(int &a, int &b) { int t = a; a = b; b = t; }

/* MAIN */
void main()
{
    int gDriver = DETECT, gMode;
    int xArray[16]; // 2 * Cntr
    int iCntr, jCntr;

    initgraph(&gDriver, &gMode, "C:\\TURBOC3\\BGI");
    if (graphresult() != grOk) {
        cout << "Graphics initialization failed: " << grapherrormsg(graphresult());
        getch();
        return;
    }

    // populate xArray with (x0,y0,x1,y1,...)
    for (iCntr = 0, jCntr = 0; jCntr < Cntr; jCntr++, iCntr += 2) {
        xArray[iCntr] = NodesObj[jCntr].x;
        xArray[iCntr + 1] = NodesObj[jCntr].y;
    }

    // draw polygon outline
    setcolor(WHITE);
    drawpoly(Cntr, xArray);

    // explicitly close polygon (drawpoly already closes it, but keep for clarity)
    line(NodesObj[Cntr - 1].x, NodesObj[Cntr - 1].y, NodesObj[0].x, NodesObj[0].y);

    getch();

    Scanfill();

    getch();
    closegraph();
    return;
}

/* SCAN FILL */
void Scanfill()
{
    int iCntr, Scan;
    int maxy = getmaxy();

    // allocate global edge table with one bucket per scanline (0..maxy)
    Edges = new Edge*[maxy + 1];
    for (iCntr = 0; iCntr <= maxy; ++iCntr) {
        Edges[iCntr] = new Edge;    // dummy head for each bucket
        Edges[iCntr]->Next = NULL;
    }

    // initialize Active list with dummy head
    Active = new Edge;
    Active->Next = NULL;

    // build global edge table from polygon vertices
    BuildEdgeList(Cntr, NodesObj);

    // scan through all scanlines
    for (Scan = 0; Scan <= maxy; ++Scan) {
        BulidActiveList(Scan);            // move edges from global table at this y into Active
        if (Active->Next != NULL) {
            ResortActiveList(Scan);      // draw spans for this scanline using current XIntersect values
            UpdateActiveList(Scan);      // increment XIntersect and remove finished edges
            ResortActiveList();          // reorder Active list by updated XIntersect for next scanline
        }
    }

    // cleanup global edge buckets
    for (iCntr = 0; iCntr <= maxy; ++iCntr) {
        Edge *p = Edges[iCntr];
        while (p) {
            Edge *t = p->Next;
            delete p;
            p = t;
        }
    }
    delete [] Edges;
}

/* Insert edge into a sorted edge list (sorted ascending by XIntersect) */
void InsertEdge(Edge* edgeList, Edge* InitialEdge)
{
    Edge *prev = edgeList;
    Edge *cur = edgeList->Next;
    // find insertion point
    while (cur != NULL && cur->XIntersect <= InitialEdge->XIntersect) {
        prev = cur;
        cur = cur->Next;
    }
    // insert after prev
    InitialEdge->Next = prev->Next;
    prev->Next = InitialEdge;
}

/* Build global edge table: iterate edges (i -> i+1), wrap last->first */
void BuildEdgeList(int count, Nodes* Vertices)
{
    for (int i = 0; i < count; ++i) {
        Nodes v1 = Vertices[i];
        Nodes v2 = Vertices[(i + 1) % count]; // next vertex, wrapping

        // skip horizontal edges
        if (v1.y == v2.y) continue;

        // identify lower and upper vertex by y
        if (v1.y < v2.y) {
            // lower is v1, upper is v2
            Edge *newE = new Edge;
            MakeEdgeRec(v1, v2, newE);
        } else {
            Edge *newE = new Edge;
            MakeEdgeRec(v2, v1, newE);
        }
    }
}

/* Insert edges from global table bucket Edges[Scan] into Active list */
void BulidActiveList(int Scan)
{
    Edge *p = Edges[Scan]->Next;
    while (p != NULL) {
        Edge *next = p->Next;
        // reset next because we'll insert into Active list
        p->Next = NULL;
        InsertEdge(Active, p);
        p = next;
    }
    // clear bucket (their nodes moved to Active)
    Edges[Scan]->Next = NULL;
}

/* Draw spans for current scanline using pairs of intersections from Active list */
void ResortActiveList(int Scan)
{
    Edge *iEdge = Active->Next;
    while (iEdge != NULL && iEdge->Next != NULL) {
        Edge *jEdge = iEdge->Next;
        int xStart = (int)ceil(iEdge->XIntersect);
        int xEnd = (int)floor(jEdge->XIntersect) - 1; // inclusive end as original code used <= jEdge-1
        if (xEnd >= xStart) {
            for (int x = xStart; x <= xEnd; ++x) {
                putpixel(x, Scan, RED);
            }
        }
        iEdge = jEdge->Next;
    }
}

/* Update Active edge table: remove edges where Scan >= YUpper, otherwise advance XIntersect */
void UpdateActiveList(int Scan)
{
    Edge *prev = Active;
    Edge *cur = Active->Next;
    while (cur != NULL) {
        if (Scan >= cur->YUpper) {
            // remove cur from list
            prev->Next = cur->Next;
            Edge *toDelete = cur;
            cur = cur->Next;
            // do not delete the edge (it originated from Edges list allocation)
            // but since we allocated edges with new in MakeEdgeRec and moved nodes around,
            // we can safely delete it here to avoid memory leak
            delete toDelete;
        } else {
            // advance x for the next scanline
            cur->XIntersect += cur->dxperscan;
            prev = cur;
            cur = cur->Next;
        }
    }
}

/* Rebuild Active list ordering by inserting each node back into new list (sort by XIntersect) */
void ResortActiveList()
{
    Edge *cur = Active->Next;
    Active->Next = NULL; // empty list
    while (cur != NULL) {
        Edge *next = cur->Next;
        cur->Next = NULL;
        InsertEdge(Active, cur);
        cur = next;
    }
}

/* Return next different-y vertex (not used now) - placeholder kept from original */
int YNext(int YCurr)
{
    // preserved original signature but not used in this version
    // Return same value for safety
    return YCurr;
}

/* Delete the node following QEdges; kept for compatibility (unused) */
void DeleteAfter(Edge* QEdges)
{
    if (QEdges == NULL || QEdges->Next == NULL) return;
    Edge* p = QEdges->Next;
    QEdges->Next = p->Next;
    delete p;
}

/* MakeEdgeRec: prepare edge record from lower and upper vertex */
void MakeEdgeRec(Nodes lower, Nodes upper, Edge* CurrEdge)
{
    // dx per scan (1/m) : (x_upper - x_lower) / (y_upper - y_lower)
    CurrEdge->dxperscan = (float)(upper.x - lower.x) / (float)(upper.y - lower.y);
    // initial XIntersect is x at the lower y (for scan = lower.y)
    CurrEdge->XIntersect = (float)lower.x;

    // YUpper: the scanline at which this edge becomes inactive
    // commonly set to upper.y - 1 so the top-most pixel of upper vertex is not double-counted
    CurrEdge->YUpper = upper.y - 1;

    // next pointer set to NULL (will be set during insert)
    CurrEdge->Next = NULL;

    // insert into global edge table bucket at y = lower.y (bucket uses lower y as index)
    int yIndex = lower.y;
    // safety: ensure yIndex within bounds
    int maxy = getmaxy();
    if (yIndex < 0) yIndex = 0;
    if (yIndex > maxy) yIndex = maxy;

    InsertEdge(Edges[yIndex], CurrEdge);
}
