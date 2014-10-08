// ---------------------------------------------------------------------
// Image Processor
//
// IP reads image files and performs various operations
//
// ---------------------------------------------------------------------

// system includes
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

// application includes
#include "main.h"
#include "image.h"
#include "script.h"


// ---------------------------------------------------------------------
// main
// ---------------------------------------------------------------------

int main(int argc, char **argv /*, char **envp */) {

  // local variables
  string programName;
  string inName;
  string outName;
  ifstream in;
  ofstream out;
  streambuf *original_cin;
  streambuf *original_cout;

  // make the compiler happy, but remember I CAN get the environment
//  char **e;
//  e = envp;

  Script script;
  script.init();

  // process arguments
  programName = argv[0];
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-i" || string(argv[i]) == "-input") {
      i++;
      inName = argv[i];
    }
    else if (string(argv[i]) == "-o" || string(argv[i]) == "-output") {
      i++;
      outName = argv[i];
    }
    else if (string(argv[i]) == "-image") {
      i++;
      Item *item = new Item();
      item->type = STRING;
      item->s = argv[i];
      script.Push(*item);
      script.imgRead();
    }
    else {
      cerr << "unknown parameter: '" << argv[i] << "' ignored" << endl;
    }
  }

  // open files

  // open input file
  if (!inName.empty()) {
    in.open(inName.c_str(), ios::in);
    if (!in) {
      cerr << "unable to open '" << inName << "' for reading" << endl;
      exit(-1);
    }
    original_cin = cin.rdbuf();
    cin.ios::rdbuf(in.rdbuf());
  }

  // open output file
  if (!outName.empty()) {
    out.open(outName.c_str(), ios::out);
    if (!out) {
      cerr << "unable to open '" << outName << "' for writing" << endl;
      exit(-1);
    }
    original_cout = cout.rdbuf();
    cout.ios::rdbuf(out.rdbuf());
  }

  // read and execute script
  script.run();

  // close files and restore streams
  if (!inName.empty()) {
    in.close();
    cin.ios::rdbuf(original_cin);
  }
  if (!outName.empty()) {
    out.close();
    cout.ios::rdbuf(original_cout);
  }

}

