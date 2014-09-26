#include <iostream>
#include <string>

using namespace std;

#include "image.h"


bool Image::writeDB(bool debug, string outputDBName) {

  ofstream out;
  streambuf *original_cout;
  clock_t start = clock(), end;
  float clks = CLOCKS_PER_SEC / 1000.0;
  float secs;
  bool ok = true;

  // open output file
  if(debug) cerr << "  opening " << outputDBName << endl;
  out.open(outputDBName.c_str(), ios::out);
  if(!out) {
    cerr << "    unable to open '" << outputDBName << "' for writing" << endl;
    ok = false;
  }
  if(ok) {
    original_cout = cout.rdbuf();
    cout.ios::rdbuf(out.rdbuf());
  }

  // dump adjacency list
  if(ok) {
    try {
      if(debug) {
        cerr << "  dumping adjacency list ...";
        start = clock();
      }
      cout << graph;
      if(debug) {
        end = clock();
        secs = (float)((end - start) / clks) / 1000.0;
        cerr << "done (" << secs << " seconds)" << endl;
      }
    }
    catch(...) {
      cerr << "    dump adjacency list failed" << endl;
      ok = false;
    }
  }

  // dump shapes
  if(ok) {
    try {
      if(debug) {
        cerr << "  dumping shape dictionary ...";
        start = clock();
      }
      dumpDictionary(cout);
      if(debug) {
        end = clock();
        secs = (float)((end - start) / clks) / 1000.0;
        cerr << "done (" << secs << " seconds)" << endl;
      }
    }
    catch(...) {
      cerr << "    dump dictionary failed" << endl;
      ok = false;
    }
  }

  // restore stream
  if(ok) {
    cout.ios::rdbuf(original_cout);
  }

  return ok;
}

