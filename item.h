#ifndef ITEM_H
#define ITEM_H

#include <iostream>

#include <string>
#include <list>
#include <map>
using namespace std;

#include "image.h"

// constants
#define BOOL   0
#define STRING 1
#define NUMBER 2
#define LIST   3
#define DICT   4
#define PROC   5
#define IMAGE  6

// Item class

class Item {
public:

  // constructors
  Item();

  // destructor
  ~Item();

  // I/O
  friend ostream &operator<<(ostream &out, Item &i);

  // methods

  // operators
  Item &operator+(Item &a);
  Item &operator-(Item &a);
  Item &operator*(Item &a);
  Item &operator/(Item &a);
  Item &operator%(Item &a);

  // data members
  int type;
  bool b;
  string s;
  double n;
  list<Item> l;
  map<string, Item> d;
  list<Item> p;
  Image *i;

};

#endif
