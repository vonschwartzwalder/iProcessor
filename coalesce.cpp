// -------------------------------------
// coalesce
//
// calculate articulation point and biconnected components
// for each shape in each biconnected component with only 1 articulation point
//   add area to articulation point
//   save as a deletion candidate
//
// for each deletion candidate, delete it
// -------------------------------------
#include <iostream>
#include <time.h>
#include <stack>
#include <map>

using namespace std;

#include "image.h"


void Image::coalesce(bool debug, int coalesceLevels) {

  int count;
  int total = 0;
  int index;
  stack<int> toDelete;
  clock_t start = clock(), end;
  float clks = CLOCKS_PER_SEC / 1000.0;
  float secs;

  // calculate biconnected components
  graph.biconnected_components();

  count = 0;
  list<list<int> >::iterator biter;
  list<int>::iterator siter;
  int aPoints;

  // for each level
  for(int i = 0; i < coalesceLevels; i++) {

    if(debug) {
      cerr << "  coalescing level " << i << endl;
      start = clock();
    }

    // for each biconnected component
    for(biter = graph.bic.begin(); biter != graph.bic.end(); biter++) {

      // for each shape in the component
      // see if this component is a leaf
      aPoints = 0;
      for(siter = biter->begin(); siter != biter->end(); siter++) {
        if(graph.articulation[*siter]) {
          aPoints++;
          index = *siter;  // keep track of the articulation point
        }
      }

      // coalesce this component?
      if(aPoints == 1) {
        count++;
        for(siter = biter->begin(); siter != biter->end(); siter++) {
          if(!(graph.articulation[*siter])) {
            toDelete.push(*siter);
          }
        }
      }

    }
    if(debug) cerr << "    coalesced " << count << " components" << endl;

    // do the deletions
    count = 0;
    if(debug) cerr << "    deleting nodes" << endl;
    while(!toDelete.empty()) {
      index = toDelete.top(); toDelete.pop();
      graph.deleteNode(index);
      dictionary.erase(index);
      count++;
    }
    total += count;

    if(debug) {
      end = clock();
      secs = (float)((end - start) / clks) / 1000.0;
      cerr << "  level " << i << " done (" << secs << " seconds)" << endl;
    }
  }

  if(debug) cerr << "  coalesced " << total << " shapes" << endl;
  
}
