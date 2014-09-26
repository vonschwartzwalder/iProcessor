// -------------------------------------
// prune
//
// find all shapes that are small and
// save them as deletion candidates
//
// for each deletion candidate that is a leaf node,
// save as a deletion candidate
//
// for each deletion candidate, delete it
// -------------------------------------
#include <iostream>
#include <time.h>
#include <stack>
#include <map>

using namespace std;

#include "image.h"


void Image::prune(bool debug, int pruneSize, bool leafOnly) {

  int count;
  int index;
  stack<int> small;
  stack<int> toDelete;
  clock_t start = clock(), end;
  float clks = CLOCKS_PER_SEC / 1000.0;
  float secs;
  
  // find small shapes
  count = 0;
  if(debug) cerr << "  finding small shapes ...";
  if(debug) start = clock();
  map<unsigned int, Shape>::iterator iter;
  for(iter = dictionary.begin(); iter != dictionary.end(); iter++) {
    if(iter->second.area < pruneSize) {
      index = iter->first;
      if(leafOnly) small.push(index); else toDelete.push(index);
      count++;
    }
  }
  if(debug) {
    end = clock();
    secs = (float)((end - start) / clks) / 1000.0;
    cerr << "done (" << secs << " seconds)" << endl;
  }
  if(debug) cerr << "    found " << count << " small shapes" << endl;

  // find leaf nodes
  if(leafOnly) {
    count = 0;
    if(debug) cerr << "  finding leaf nodes ...";
    if(debug) start = clock();
    while(!small.empty()) {
      index = small.top(); small.pop();
      if(graph.isLeaf(index)) {
        toDelete.push(index);
        count++;
      }
    }
    if(debug) {
      end = clock();
      secs = (float)((end - start) / clks) / 1000.0;
      cerr << "done (" << secs << " seconds)" << endl;
    }
    if(debug) cerr << "    found " << count << " leaf nodes" << endl;
  }

  // do the deletions
  count = 0;
  if(debug) cerr << "  deleting nodes ...";
  if(debug) start = clock();
  while(!toDelete.empty()) {
    index = toDelete.top(); toDelete.pop();
    graph.deleteNode(index);
    dictionary.erase(index);
    count++;
  }
  if(debug) {
    end = clock();
    secs = (float)((end - start) / clks) / 1000.0;
    cerr << "done (" << secs << " seconds)" << endl;
  }

  if(debug) cerr << "    deleted " << count << " shapes" << endl;

}
