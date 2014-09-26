#include <time.h>
#include <iostream>
#include <string>

using namespace std;

#include "image.h"

bool Image::readDB(bool debug, string inputDBName) {

  ifstream in;
  streambuf *original_cin;
  clock_t start = clock(), end;
  float clks = CLOCKS_PER_SEC / 1000.0;
  float secs;
  bool ok = true;

  // clear db if it already exists
  if(graph.adjacency.size() != 0) {
    graph.adjacency.clear();
    dictionary.clear();
  }

  // open input file
  if(debug) cerr << "  opening " << inputDBName << endl;
  in.open(inputDBName.c_str(), ios::in);
  if(!in) {
    cerr << "    unable to open '" << inputDBName << "' for reading" << endl;
    ok = false;
  }
  if(ok) {
    original_cin = cin.rdbuf();
    cin.ios::rdbuf(in.rdbuf());
  }

  // read adjacency list
  if(ok) {
    if(debug) cerr << "  reading adjacency list ...";
    if(debug) start = clock();
    if(!cin.eof()) {
      cin >> graph;
      if(debug) {
        end = clock();
        secs = (float)((end - start) / clks) / 1000.0;
        cerr << "done (" << secs << " seconds)" << endl;
      }
      if(debug) cerr << "  read " << graph.nodes() << " nodes"
        << " and " << graph.edges() << " edges" << endl;
    }
  }

  // read shape dictionary
  if(ok) {
    if(debug) cerr << "  reading shape dictionary ...";
    if(debug) start = clock();
    Shape *shape = NULL;
    int n_shapes = 0;
    while(!cin.eof()) {
      shape = new Shape;
      cin >> *shape;
      dictionary[shape->label] = *shape;
      n_shapes++;
    }
    if(debug) {
      end = clock();
      secs = (float)((end - start) / clks) / 1000.0;
      cerr << "done (" << secs << " seconds)" << endl;
    }
    if(debug) cerr << "  read " << n_shapes << " shapes" << endl;
  }

  // restore stream
  if(ok) {
    cin.ios::rdbuf(original_cin);
  }

  return true;
  
}

