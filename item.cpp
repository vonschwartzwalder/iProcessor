// ------------------------------------------------
// item
// ------------------------------------------------

// includes
#include <iostream>
#include <strstream>
#include <string>
#include <list>
#include <map>

#include "item.h"

// ------------------------------------------------
// constructor
// ------------------------------------------------
Item::Item() {
  s = "";
  type = STRING;
}

// ------------------------------------------------
// destructor
// ------------------------------------------------
Item::~Item() {
}

// ------------------------------------------------
// operator+
// ------------------------------------------------
Item &Item::operator+(Item &a) {

  Item *result = new Item;

  // add two numbers
  if (type == NUMBER && a.type == NUMBER) {
    result->type = NUMBER;
    result->n = a.n + n;
  }

  // add a number and a string
  else if (type == NUMBER && a.type == STRING) {
    result->type = STRING;
    strstream str;
    str << n;
    result->s = a.s + str.str();
  }

  // add a number and a list
  else if (type == NUMBER && a.type == LIST) {
    result->type = LIST;
    result->l = a.l;
    result->l.push_front(*this);
  }

  // add two strings
  else if (type == STRING && a.type == STRING) {
    result->type = STRING;
    result->s = a.s + s;
  }

  // add a string and a number
  else if (type == STRING && a.type == NUMBER) {
    result->type = STRING;
    strstream str;
    str << a.n;
    result->s = str.str() + s;
  }

  // add a string and a list
  else if (type == STRING && a.type == LIST) {
    result->type = LIST;
    result->l = a.l;
    result->l.push_front(*this);
  }

  // add two lists
  else if (type == LIST && a.type == LIST) {
    result->type = LIST;
    result->l = a.l;
    result->l.splice(result->l.end(), l);
  }

  // add a list and a string
  else if (type == LIST && a.type == STRING) {
    result->type = LIST;
    result->l.push_back(a);
  }

  // add a list and a number
  else if (type == LIST && a.type == NUMBER) {
    result->type = LIST;
    result->l.push_back(a);
  }

  return *result;

}

// ------------------------------------------------
// operator-
// ------------------------------------------------
Item &Item::operator-(Item &a) {

  Item *result = new Item;

  // subtract two numbers
  if (type == NUMBER && a.type == NUMBER) {
    result->type = NUMBER;
    result->n = a.n - n;
  }

  return *result;

}

// ------------------------------------------------
// operator*
// ------------------------------------------------
Item &Item::operator*(Item &a) {

  Item *result = new Item;

  // multiply two numbers
  if (type == NUMBER && a.type == NUMBER) {
    result->type = NUMBER;
    result->n = a.n * n;
  }

  return *result;

}

// ------------------------------------------------
// operator/
// ------------------------------------------------
Item &Item::operator/(Item &a) {

  Item *result = new Item;

  // divide two numbers
  if (type == NUMBER && a.type == NUMBER) {
    result->type = NUMBER;
    result->n = a.n / n;
  }

  return *result;

}

// ------------------------------------------------
// operator%
// ------------------------------------------------
Item &Item::operator%(Item &a) {

  Item *result = new Item;

  // modulo
  if (type == NUMBER && a.type == NUMBER) {
    result->type = NUMBER;
    result->n = (double)((int)a.n % (int)n);
  }

  return *result;

}

// ------------------------------------------------
// ostream operator<<
// ------------------------------------------------
ostream &operator<<(ostream &out, Item &i) {

  if (i.type == BOOL) {
    out << (i.b ? "true" : "false");
  }
  else if (i.type == STRING) {
    out << i.s;
  }
  else if (i.type == NUMBER) {
    out << i.n;
  }
  else if (i.type == LIST) {
    list<Item>::iterator iter;
    out << "( ";
    for (iter = i.l.begin(); iter != i.l.end(); iter++) {
      if (iter->type == BOOL) {
        out << (iter->b ? "true" : "false") << " ";
      }
      else if (iter->type == STRING) {
        out << iter->s << " ";
      }
      else if (iter->type == NUMBER) {
        out << iter->n << " ";
      }
      else if (iter->type == LIST) {
        list<Item>::iterator liter;
        out << "( ";
        for (liter = iter->l.begin(); liter != iter->l.end(); liter++) {
          out << *liter << " ";
        }
        out << ") ";
      }
      else if (iter->type == DICT) {
        map<string, Item>::iterator diter;
        out << "[ ";
        for (diter = iter->d.begin(); diter != iter->d.end(); diter++) {
          out << diter->first << ":";
          out << (diter->second) << " ";
        }
        out << "] ";
      }
      else if (iter->type == PROC) {
        list<Item>::iterator piter;
        out << "{ ";
        for (piter = iter->p.begin(); piter != iter->p.end(); piter++) {
          out << *piter << " ";
        }
        out << "} ";
      }
      else if (iter->type == IMAGE){
        out << iter->i << " ";
      }
    }
    out << ")";
  }
  else if (i.type == DICT) {
    map<string, Item>::iterator iter;
    out << "[ ";
    for (iter = i.d.begin(); iter != i.d.end(); iter++) {
      out << iter->first << ":";
      if (iter->second.type == BOOL) {
        out << (iter->second.b ? "true" : "false") << " ";
      }
      else if (iter->second.type == STRING) {
        out << iter->second.s << " ";
      }
      else if (iter->second.type == NUMBER) {
        out << iter->second.n << " ";
      }
      else if (iter->second.type == LIST){
        list<Item>::iterator liter;
        out << "( ";
        for (liter = iter->second.l.begin(); liter != iter->second.l.end(); liter++) {
          out << *liter << " ";
        }
        out << ") ";
      }
      else if (iter->second.type == DICT){
        map<string, Item>::iterator diter;
        out << "[ ";
        for (diter = iter->second.d.begin(); diter != iter->second.d.end(); diter++) {
          out << diter->first << ":";
          out << (diter->second) << " ";
        }
        out << "] ";
      }
      else if (iter->second.type == PROC){
        list<Item>::iterator piter;
        out << "{ ";
        for (piter = iter->second.p.begin(); piter != iter->second.p.end(); piter++) {
          out << *piter << " ";
        }
        out << "} ";
      }
      else if (iter->second.type == IMAGE){
        out << iter->second.i << " ";
      }
    }
    out << "]";
  }
  else if (i.type == PROC) {
    out << "{ ";
    list<Item>::iterator iter;
    for (iter = i.p.begin(); iter != i.p.end(); iter++) {
      if (iter->type == BOOL) {
        out << (iter->b ? "true" : "false") << " ";
      }
      else if (iter->type == STRING) {
        out << iter->s << " ";
      }
      else if (iter->type == NUMBER) {
        out << iter->n << " ";
      }
      else if (iter->type == LIST){
        list<Item>::iterator liter;
        out << "( ";
        for (liter = iter->l.begin(); liter != iter->l.end(); liter++) {
          out << *liter << " ";
        }
        out << ") ";
      }
      else if (iter->type == DICT){
        map<string, Item>::iterator diter;
        out << "[ ";
        for (diter = iter->d.begin(); diter != iter->d.end(); diter++) {
          out << diter->first << ":";
          out << (diter->second) << " ";
        }
        out << "] ";
      }
      else if (iter->type == PROC){
        list<Item>::iterator piter;
        out << "{ ";
        for (piter = iter->p.begin(); piter != iter->p.end(); piter++) {
          out << *piter << " ";
        }
        out << "} ";
      }
      else if (iter->type == IMAGE){
        out << iter->i << " ";
      }
    }
    out << "}";
  }
  else if (i.type == IMAGE) {
    out << i.i;
  }
  else if (i.type == SHAPES) {
    map<unsigned int, Shape>::iterator iter;
    out << "[ ";
    for (iter = i.sd.begin(); iter != i.sd.end(); iter++) {
      out << iter->first << ":" << iter->second << " ";
    }
    out << "]";
  }

  return out;

}
