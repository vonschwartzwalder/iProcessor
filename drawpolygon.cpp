// ---------------------------------------------------------------------
// Concave Polygon Scan Conversion
// by Paul Heckbert
// from "Graphics Gems", Academic Press, 1990
//
// updated by Duane Schwartzwald to use ANSI c and c++, 2003
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// drawPolygon: scan convert concave non-simple polygon with vertices at
// list<Point> polygon.
//
// Polygon can be clockwise or counterclockwise.
// Algorithm does uniform point sampling at pixel centers.
// Inside-outside test done by Jordan's rule: a point is considered inside if
// an emanating ray intersects the polygon an odd number of times.
// ---------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pnm.h"

// -------------------------------------
// structures and types
// -------------------------------------

typedef struct {
  double x, y;
} Point2;

typedef struct {		 // a polygon edge
  double x;	         // x coordinate of edge's intersection with current scanline
  double dx;	       // change in x with respect to y
  int i;	           // edge number: edge i goes from pt[i] to pt[i+1]
} Edge;

static int n;			   // number of vertices 
static Point2 *pt;	 // vertices 

static int nact;		 // number of active edges 
static Edge *active; // active edge list:edges crossing scanline y 

// -------------------------------------
// forward declarations
// -------------------------------------
static void cdelete(int i);
static void cinsert(int i, int y);
int compare_ind(const void *u, const void *v);
int compare_active(const void *u, const void *v);

// -------------------------------------
// drawPolygon
// -------------------------------------
void PNM::drawPolygon(list<Point> polygon, PNM_Color lc, PNM_Color fc) {

  // draw the filled polygon
  drawPolygon(polygon, fc);

  // draw outline
  Point p0, p1;
  bool first = true;
  list<Point>::iterator iter;
  for(iter = polygon.begin(); iter != polygon.end(); iter++) {
    if(first) {
      p0 = *iter;
    }
    else {
      p1 = *iter;
      drawLine(p0, p1, lc);
      p0 = p1;
    }
  }
  
}

void PNM::drawPolygon(list<Point> polygon, PNM_Color c) {

  // anything to draw?
  if(polygon.empty()) return;
  
  // recreate point list
  int nvert = polygon.size();
  Point2 *point = new Point2[nvert];
  list<Point>::iterator iter;
  int index = 0;
  for(iter = polygon.begin(); iter != polygon.end(); iter++) {
    point[index].x = iter->x;
    point[index].y = iter->y;
    index++;
  }

  int k, y0, y1, y, i, j, xl, xr;
  int *ind;		       // list of vertex indices, sorted by pt[ind[j]].y 

  n = nvert;
  pt = point;
  ind = new int[n];
  active = new Edge[n];

  // create y-sorted array of indices ind[k] into vertex list 
  for(k = 0; k < n; k++) {
    ind[k] = k;
  }
  qsort(ind, n, sizeof ind[0], compare_ind);	// sort ind by pt[ind[k]].y
  
  nact = 0;				// start with empty active list 
  k = 0;				  // ind[k] is next vertex to process 
  y0 = (int)(ceil(pt[ind[0]].y - 0.5));		  // ymin of polygon
  y1 = (int)(floor(pt[ind[n-1]].y - 0.5));	  // ymax of polygon 

  // step through scanlines
  for(y = y0; y <= y1; y++) {		 
    // scanline y is at y+.5 in continuous coordinates 

    // check vertices between previous scanline and current one, if any 
    for(; k < n && pt[ind[k]].y <= y + 0.5; k++) {
      
      // to simplify, if pt.y=y+.5, pretend it's above 
      // invariant: y-.5 < pt[i].y <= y+.5 
      i = ind[k];
      
      // insert or delete edges before and after vertex i (i-1 to i, and i to i+1)
      // from active list if they cross scanline y

      // vertex previous to i
      j = (i > 0) ? i-1 : n-1;

      // old edge, remove from active list
      if(pt[j].y <= y - 0.5)	     
        cdelete(j);

      // new edge, add to active list
      else if(pt[j].y > y + 0.5)	 
        cinsert(j, y);

      // vertex next after i
      j = i < n-1 ? i+1 : 0;

      // old edge, remove from active list
      if(pt[j].y <= y - 0.5)	     
        cdelete(i);

      // new edge, add to active list
      else if(pt[j].y > y + 0.5)	 
        cinsert(i, y);
    }

    // sort active edge list by active[j].x 
    qsort(active, nact, sizeof active[0], compare_active);

    // draw horizontal segments for scanline y 
    for(j = 0; j < nact; j += 2) {
      
      // span between j & j+1 is inside, span tween j+1 & j+2 is outside 
      xl = (int)(ceil(active[j].x - 0.5));		  // left end of span 
      xr = (int)(floor(active[j+1].x - 0.5));	// right end of span 
      if(xl <= xr) {
        for(int x = xl; x <= xr; x++) {
          try {
            color(x, y, c);
          }
          catch(PNM_ArrayException e) {}
        }
      }

      // increment edge coords
      active[j].x += active[j].dx;	 
      active[j+1].x += active[j+1].dx;
    }
  }
}

// -------------------------------------
// remove edge i from active list
// -------------------------------------
static void cdelete(int i) {
  int j;
  for(j = 0; j < nact && active[j].i != i; j++);
  if(j >= nact) return;	// edge not in active list
  nact--;
  memcpy(&active[j], &active[j+1], (nact-j)*sizeof active[0]);
}

// -------------------------------------
// append edge i to end of active list
// -------------------------------------
static void cinsert(int i, int y) {
  int j;
  double dx;
  Point2 *p, *q;

  j = (i < n-1) ? i+1 : 0;
  if(pt[i].y < pt[j].y) {
    p = &pt[i];
    q = &pt[j];
  }
  else {
    p = &pt[j];
    q = &pt[i];
  }

  // initialize x position at intersection of edge with scanline y 
  active[nact].dx = dx = (q->x - p->x)/(q->y - p->y);
  active[nact].x = dx*(y + 0.5 - p->y) + p->x;
  active[nact].i = i;
  nact++;
}

// comparison routines for qsort 
int compare_ind(const void *u, const void *v) { return (pt[*((int *)u)].y <= pt[*((int *)v)].y) ? -1 : 1; }
int compare_active(const void *u, const void *v) { return (((Edge *)u)->x <= ((Edge *)v)->x) ? -1 : 1; }
