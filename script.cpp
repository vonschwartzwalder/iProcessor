// ---------------------------------------------------------------------
// script
//
// stack based language parser for using image processor
// variable declaration using def or operator =
// variable type is determined by content
// several mathematic operators defined (+,-,/,*,^,%)
// several mathematic functions defined (ln,pow,neg)
// stack control is pop and automatic push
// two print functions (print, println)
// no unary minus (use neg function)
// no looping constructs yet
// no conditionals yet
// lists use () pairs
// dictionaries use [] pairs
// procedures us {} pairs
// string literals use "" or '' pairs
// comments are '#'
//
// all local functions are void()
//   arguments are found on stack
//   results are pushed onto stack
//
// stack variables are of type
//   boolean
//   string
//   number (double)
//   list
//   dictionary
//   procedure
//   image
//
// globals used (global to script only)
//   s     - execution stack
//   f     - function dictionary
//   fiter - function dictionary iterator
//   v     - variable dictionary
//   viter - variable dictionary iterator
//   p     - procedure dictionary
//   piter - procedure dictionary iterator
//   o     - operator dictionary
//   oiter - operator dictionary iterator
//
// STL templates used
//   stack
//   map
//   string
//   list
// ---------------------------------------------------------------------

// script includes
#include <string>
#include <cstdlib>
#include <cstdio>
#include <list>
using namespace std;

#include <ctype.h>
#include <math.h>

#include "script.h"

// ---------------------------------------------------------------------
// script
// ---------------------------------------------------------------------
Script::Script() {
}

Script::~Script() {

  // clean up temporary files
  std::list<string>::iterator i;
  for (i = ti.begin(); i != ti.end(); i++) {
    remove((*i).c_str());
  }
}

void Script::run() {

  // local variables
  Item *item;
  Token token;
  inproc = 0;

  // read all tokens
  token = getToken();
  while (token.type != tokUNKNOWN) {
    switch (token.type) {
    case tokBOOL:
      item = new Item;
      item->type = BOOL;
      item->b = (token.token == "true" ? true : false);
      s.push(*item);
      break;
    case tokALPHA:
      if (inproc) {
        item = new Item;
        item->type = STRING;
        item->s = token.token;
        s.push(*item);
      }
      else {

        // see if this is a procedure, function or a variable
        fiter = f.find(token.token);
        viter = v.find(token.token);

        // if a function, execute
        if (fiter != f.end()) {
          // here be magic!
          // p is a pointer to a member function
          // f[str] is a Function structure that has p as a member
          // it works because we are executing a member of this
          (this->*(f[token.token].p))();
          //(*this.*(f[str].p))();
        }

        // if a variable, push value
        else if (viter != v.end()) {
          item = new Item(v[viter->first]);
          s.push(*item);

          // if this is a procedure, execute it
          if (item->type == PROC) {
            exec();
          }

        }

        // something else, push it
        else {
          item = new Item;
          item->type = STRING;
          item->s = token.token;
          s.push(*item);
        }

      }
      break;
    case tokLITERAL:
      item = new Item;
      item->type = STRING;
      item->s = token.token;
      s.push(*item);
      break;
    case tokNUMBER:
      item = new Item;
      item->type = NUMBER;
      item->n = atof(token.token.c_str());
      s.push(*item);
      break;
    case tokREFERENCE:
      item = new Item;
      item->type = STRING;
      item->s = token.token;
      s.push(*item);
      break;
    case tokOPERATOR:
      // put container markers on stack
      switch (token.token[0]) {
      case '(':
      case '[':
        item = new Item;
        item->type = STRING;
        item->s = token.token;
        s.push(*item);
        break;
      case '{':
        inproc++;
        item = new Item;
        item->type = STRING;
        item->s = token.token;
        s.push(*item);
        break;
      case ')':
      case ']':
      case '}':
        (this->*(o[token.token].p))();
        break;
      default:
        // if in a proc, push operator
        if (inproc) {
          item = new Item;
          item->type = STRING;
          item->s = token.token;
          s.push(*item);
        }
        // otherwise, execute operator
        else {
          (this->*(o[token.token].p))();
        }
        break;
      }
      break;
    }
    token = getToken();
  }
}


Token Script::getToken() {

  // local variables
  int c;
  string str;
  Token token;

  // while input is not empty
  while (!cin.eof()) {

    // see what next character is
    c = cin.peek();

    // check for an operator
    str = c;
    oiter = o.find(str);
    if (oiter != o.end()) {
      c = cin.get();
      token.token = str;
      token.type = tokOPERATOR;
      return token;
    }
    str = "";

    // comment, eat rest of line
    if (c == '#') {
      while (!cin.eof() && c != '\n') {
        c = cin.get();
      }
    }

    // string literal, find next "
    else if (c == '"') {
      c = cin.get(); // eat leading "
      c = cin.get();
      while (!cin.eof() && c != '"') {
        if (c == '\\') {
          c = cin.get();
          switch (c) {
          case 'n':
            c = '\n';
            break;
          case 't':
            c = '\t';
            break;
          case '\\':
            c = '\\';
            break;
          case '\'':
            c = '\'';
            break;
          case '"':
            c = '"';
            break;
          case 'r':
            c = '\r';
            break;
          case 'b':
            c = '\b';
            break;
          default:
            break;
          }
        }
        str += c;
        c = cin.get();
      }
      token.token = str;
      token.type = tokLITERAL;
      return token;
    }

    // string literal, find next '
    else if (c == '\'') {
      c = cin.get(); // eat leading '
      c = cin.get();
      while (!cin.eof() && c != '\'') {
        if (c == '\\') {
          c = cin.get();
          switch (c) {
          case 'n':
            c = '\n';
            break;
          case 't':
            c = '\t';
            break;
          case '\\':
            c = '\\';
            break;
          case '\'':
            c = '\'';
            break;
          case '\"':
            c = '\"';
            break;
          case 'r':
            c = '\r';
            break;
          case 'b':
            c = '\b';
            break;
          default:
            break;
          }
        }
        str += c;
        c = cin.get();
      }
      token.token = str;
      token.type = tokLITERAL;
      return token;
    }

    // alphanumeric string
    else if (isalpha(c)) {
      while (!cin.eof() && (isalnum(c) || c == '_' || c == '.')) {
        c = cin.get();
        str += c;
        c = cin.peek();
      }
      token.token = str;
      token.type = tokALPHA;
      return token;
    }

    // number
    else if (isdigit(c) || c == '.') {

      // get number
      while (!cin.eof() && (isdigit(c) || c == '.')) {
        c = cin.get();
        str += c;
        c = cin.peek();
      }
      token.token = str;
      token.type = tokNUMBER;
      return token;
    }

    // reference
    else if (c == '&') {

      // get reference
      c = cin.get();
      str += c;
      c = cin.peek();
      while (!cin.eof() && (isalnum(c) || c == '_' || c == '.')) {
        c = cin.get();
        str += c;
        c = cin.peek();
      }
      token.token = str;
      token.type = tokREFERENCE;
      return token;
    }

    // something else
    else {
      c = cin.get();
    }

  }

  token.token = "";
  token.type = tokUNKNOWN;
  return token;
}