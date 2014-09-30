// ---------------------------------------------------------------------
// script.h
//
// constants, structure, language functions, image functions and global
// variables are declared here
//
// global variables are defined here
//   s is execution stack
//   f is function dictionary
//   v is variable dictionary
// ---------------------------------------------------------------------
#ifndef SCRIPT_H
#define SCRIPT_H

#include <iostream>
#include <stack>
#include <list>
#include <map>
#include <string>
using namespace std;

#include "item.h"
#include "image.h"

#define tokBOOL      0
#define tokALPHA     1
#define tokLITERAL   2
#define tokNUMBER    3
#define tokOPERATOR  4
#define tokREFERENCE 5
#define tokUNKNOWN   9

typedef struct TOKEN {
  string token;
  int    type;
} Token;

class Script {

public:

  // constructors
  Script();

  // destructor
  ~Script();

  // there be magic here
  typedef struct FUNCTION {
    //void (*p)();         // this is a pointer to a function in c (NOT c++)
    void (Script::*p)();   // this is a pointer to a Script member function
                           // in c++
    string args;
  } Function;

  // initialization
  void init();

  // interpretor
  Token getToken();
  void run();

  // language functions
  void nop();
  void print();
  void println();
  void dump();
  void clear();
  void writeSexpr();
  void dup();
  void swap();
  void roll();
  void size();
  void empty();
  void at();
  void def();
  void undef();
  void add();
  void sub();
  void mul();
  void div();
  void mod();
  void pwr();
  void ln();
  void neg();
  void pop();
  void lt();
  void eq();
  void ne();
  void gt();
  void doif();
  void foreach();
  void dofor();
  void list();
  void dict();
  void proc();
  void load();
  void exec();

  void makeList();
  void makeDict();
  void makeProc();

  // image functions
  void imgRead();
  void imgWrite();
  void imgDiscard();
  void imgWidth();
  void imgHeight();
  void imgShow();
  void imgHide();
  void imgUpdate();

  void imgInvert();
  void imgThreshold();
  void imgAutoThresholdOtsu();
  void imgIntensity();
  void imgErode();
  void imgDilate();
  void imgOpen();
  void imgClose();

  void imgLaplacian();
  void imgMedian();
  void imgSmooth();
  void imgSobel();
  void imgConvolve();

  void imgFindShapes();
  void imgOverlayShapes();
  
  void imgToHSV();
  void imgPlane();
  void imgQuantize();
  void imgClear();

  void imgAND();
  /*
  void imgOR ();
  void imgXOR();
  void imgADD();
  void imgSUB();
  void imgMUL();
  void imgDIV();
  void imgMOD();
  void imgDIF();
  void imgMAX();
  void imgMIN();
  */
  // ImageMagick functions
  /*
  void blur();
  void crop();
  void convolve();
  void despeckle();
  void edge();
  void equalize();
  void flip();
  void flop();
  void negate();
  void normalize();
  void quantize();
  void imgroll();
  void rotate();
  void sample();
  void scale();
  void sharpen();
  void shear();
  void threshold();
  void zoom();
  void ROI();

  void imgadd();
  */

  void Push(Item i) { s.push(i); };
private:

  // execution stack
  stack<Item> s;

  // function dictionary
  map<string, Function> f;
  map<string, Function>::iterator fiter;

  // variable dictionary
  map<string, Item> v;
  map<string, Item>::iterator viter;

  // operator dictionary
  map<string, Function> o;
  map<string, Function>::iterator oiter;

  // pointer to image
  Image *image;

  // list of temporary images
  std::list<string> ti;

  // procedure flag (delay execution)
  int inproc;

};

#endif
