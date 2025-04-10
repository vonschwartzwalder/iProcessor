// ------------------------------------------------
// langFunctions.cpp
// ImageProcessor
// ------------------------------------------------

#include "script.h"
#include "math.h"
#include "stdlib.h"

// ---------------------------------------------------------------------
// language functions
// ---------------------------------------------------------------------
void Script::nop() {
}

// ------------------------------------------
// output
// ------------------------------------------

void Script::print() {

  // if the stack is empty, print a new line
  if (s.size() < 1) {
    cout << endl;
    return;
  }

  // if the top of the stack contains a reference, put the contents on the stack
  if (s.top().type == STRING && s.top().s[0] == '&') {
    Item item;
    item = s.top(); s.pop();
    string name = item.s;
    name = name.substr(1);
    viter = v.find(name);
    if (viter != v.end()) {
      item = v[viter->first];
      s.push(item);
    }
    else {
      s.push(item);
    }
  }

  // if the top of the stack contains an image, display the image
  // TODO: search path instead of hard coding it
  if (s.top().type == IMAGE) {
    Item item;
    item = s.top();
    string name = "/tmp/iProcessor_img";
    ti.push_back(name);
    item.i->saveImage(name);
    string cmd = string("/usr/bin/display ") + name + " &";
    system(cmd.c_str());
    return;
  }

  // print the top of the stack
  cout << s.top();
  s.pop();
}

void Script::println() {
  print();
  cout << endl;
}

void Script::dump() {
  if (s.size() < 1) {
    cerr << "dump stack underflow" << endl;
    return;
  }
  if (s.top().type != STRING) {
    s.pop();
    cerr << "invalid type" << endl;
    return;
  }
  string which = s.top().s; s.pop();

  if (which == "stack" || which[0] == 's') {
    Item item;

    // dump stack
    std::stack<Item> stk;
    cout << "stack:" << endl;
    while (!s.empty()) {
      item = s.top();  s.pop();
      stk.push(item);
      switch (item.type) {
      case BOOL:
        cout << "  BOOL: ";
        break;
      case STRING:
        cout << "  STRING: ";
        break;
      case NUMBER:
        cout << "  NUMBER: ";
        break;
      case LIST:
        cout << "  LIST: ";
        break;
      case DICT:
        cout << "  DICT: ";
        break;
      case PROC:
        cout << "  PROC: ";
        break;
      case IMAGE:
        cout << "  IMAGE: ";
        break;
      }
      cout << item << endl;

    }

    // restore stack
    while (!stk.empty()) {
      item = stk.top();  stk.pop();
      s.push(item);
    }
  }

  else if (which == "function" || which == "func" || which[0] == 'f') {

    // dump function dictionary
    cout << "functions:" << endl;
    for (fiter = f.begin(); fiter != f.end(); fiter++) {
      cout << "  " << fiter->first;
      cout << " ( " << f[fiter->first].args << " )" << endl;
    }

  }

  else if (which == "variable" || which == "var" || which[0] == 'v') {

    // dump variable dictionary
    cout << "variables:" << endl;
    for (viter = v.begin(); viter != v.end(); viter++) {
      cout << "  " << viter->first << " = ";
      cout << v[viter->first] << endl;
    }

  }

  else if (which == "operator" || which == "op" || which[0] == 'o') {

    // dump operator dictionary
    cout << "operators:" << endl;
    for (oiter = o.begin(); oiter != o.end(); oiter++) {
      cout << "  " << oiter->first;
      cout << " ( " << o[oiter->first].args << " )" << endl;
    }

  }
}

void Script::clear() {
  if (s.size() < 1) {
    cerr << "clear: stack underflow" << endl;
    return;
  }
  if (s.top().type != STRING) {
    s.pop();
    cerr << "clear: invalid type, expecting STRING" << endl;
    return;
  }
  string which = s.top().s; s.pop();

  if (which == "stack" || which[0] == 's') {
    Item item;

    // clear stack
    while (!s.empty()) {
      item = s.top();  s.pop();
    }
  }

  else if (which == "variable" || which == "var" || which[0] == 'v') {
    v.clear();

    Item *item = new Item;
    item->type = NUMBER; item->n = 2.178281828459045; v["e"] = *item;
    item->type = NUMBER; item->n = 3.141592653589793; v["pi"] = *item;
  }

}

void Script::writeSexpr()
{
  if (s.size() < 1) {
    cout << endl;
    return;
  }
  Item item = s.top();
  item.writeSexpr(cout);
  s.pop();
}

// ------------------------------------------
// list and dict access
// ------------------------------------------

void Script::at() {
  if (s.size() < 2) {
    cerr << "at: stack underflow" << endl;
    return;
  }

  Item item;

  // if top of stack is a string, must be a dict lookup
  if (s.top().type == STRING) {

    // get key
    string key = s.top().s; s.pop();

    // make sure item on stack is a dict
    if (s.top().type != DICT) {
      s.pop();
      cerr << "no dict on stack" << endl;
      return;
    }

    // create a new item
    item = s.top().d[key];

    // pop dict
    s.pop();

  }

  else if (s.top().type == NUMBER) {

    // get index
    double index = s.top().n; s.pop();

    // make sure item on stack is a list
    if (s.top().type != LIST) {
      s.pop();
      cerr << "no list on stack" << endl;
      return;
    }

    // find item
    std::list<Item>::iterator iter;
    unsigned int cnt = 0;
    // find the item in the list
    for (iter = s.top().l.begin(); iter != s.top().l.end(); iter++) {
      if (cnt == index) {
        break;
      }
      cnt++;
    }
    if (cnt != index) {
      s.pop();
      cerr << "index out of range" << endl;
      return;
    }

    // create a new item
    item = *iter;

    // pop list
    s.pop();

  }
  else {
    cerr << "at: invalid type" << endl;
  }

  // push item onto stack
  s.push(item);
}

// ------------------------------------------
// stack functions
// ------------------------------------------

void Script::pop() {
  if (s.size() < 1) {
    cerr << "pop: stack underflow" << endl;
    return;
  }
  s.pop();
}

void Script::dup() {
  if (s.size() < 1) {
    cerr << "dup: stack underflow" << endl;
    return;
  }

  // duplicate item on stack
  Item item;
  item = s.top();

  // push result onto stack
  s.push(item);
}

// swap top two items on the stack
void Script::swap() {
  if (s.size() < 2) {
    cerr << "swap: stack underflow" << endl;
    return;
  }
  Item item1 = s.top(); s.pop();
  Item item2 = s.top(); s.pop();
  s.push(item1);
  s.push(item2);
}

// roll the contents of the stack by one position
void Script::roll() {
  if (s.size() < 2) {
    return;
  }

  // create a temporary stack
  std::stack<Item> ts;

  // get top of stack
  Item item = s.top(); s.pop();

  // transfer rest of current stack to temporary
  while (!s.empty()) {
    ts.push(s.top()); s.pop();
  }

  // push top first
  s.push(item);

  // put rest of stack back
  while (!ts.empty()) {
    s.push(ts.top()); ts.pop();
  }
}

// get number of items on the stack
void Script::size() {
  Item item;
  item.type = NUMBER;
  item.n = (double)s.size();
  s.push(item);
}

// is stack empty?
void Script::empty() {
  Item item;
  item.type = BOOL;
  item.b = s.empty();
  s.push(item);
}

// ------------------------------------------
// variable definition
// ------------------------------------------

void Script::def() {
  if (s.size() < 2) {
    cerr << "def: stack underflow" << endl;
    return;
  }

  // get variable name
  if (s.top().type != STRING) {
    cerr << "def: invalid type, expecting STRING" << endl;
    s.pop();
    return;
  }
  string name = s.top().s; s.pop();

  // remove reference mark
  if (name[0] == '&') {
    name = name.substr(1);
  }

  // get value
  Item value = s.top(); s.pop();

  // set variable equal to value
  v[name] = value;
}

// undefine a variable
void Script::undef() {
  if (s.size() < 1) {
    cerr << "undef: stack underflow" << endl;
    return;
  }
  if (s.top().type != STRING) {
    s.pop();
    cerr << "undef: invalid type, expecting STRING" << endl;
    return;
  }
  string name = s.top().s; s.pop();
  if (name[0] == '&') {
    name = name.substr(1);
  }
  v.erase(name);
}

// ------------------------------------------
// math functions
// ------------------------------------------

void Script::add() {
  if (s.size() < 2) {
    cerr << "add: stack underflow" << endl;
    return;
  }

  Item a, b;

  a = s.top(); s.pop();
  b = s.top(); s.pop();

  if (a.type == IMAGE && b.type == IMAGE) {
    b.i->math(a.i->im, MATH_ADD);
    s.push(b);
    return;
  }

  if (!(a.type == STRING || a.type == NUMBER || a.type == LIST)) {
    cerr << "add: invalid type" << endl;
    return;
  }

  if (!(b.type == STRING || b.type == NUMBER || b.type == LIST)) {
    cerr << "add: invalid type" << endl;
    return;
  }

  s.push(a + b);
}

void Script::sub() {
  if (s.size() < 2) {
    cerr << "sub: stack underflow" << endl;
    return;
  }

  Item a, b;

  a = s.top(); s.pop();
  b = s.top(); s.pop();

  if (a.type == IMAGE && b.type == IMAGE) {
    b.i->math(a.i->im, MATH_SUB);
    s.push(b);
    return;
  }

  if (!(a.type == NUMBER)) {
    cerr << "sub: invalid type" << endl;
    return;
  }

  if (!(b.type == NUMBER)) {
    cerr << "sub: invalid type" << endl;
    return;
  }

  s.push(a - b);
}

void Script::mul() {
  if (s.size() < 2) {
    cerr << "mul: stack underflow" << endl;
    return;
  }

  Item a, b;

  a = s.top(); s.pop();
  b = s.top(); s.pop();

  if (a.type == IMAGE && b.type == IMAGE) {
    b.i->math(a.i->im, MATH_MUL);
    s.push(b);
    return;
  }

  if (!(a.type == NUMBER)) {
    cerr << "mul: invalid type" << endl;
    return;
  }

  if (!(b.type == NUMBER)) {
    cerr << "mul: invalid type" << endl;
    return;
  }

  s.push(a * b);
}

void Script::div() {
  if (s.size() < 2) {
    cerr << "div: stack underflow" << endl;
    return;
  }

  Item a, b;

  a = s.top(); s.pop();
  b = s.top(); s.pop();

  if (a.type == IMAGE && b.type == IMAGE) {
    b.i->math(a.i->im, MATH_DIV);
    s.push(b);
    return;
  }

  if (!(a.type == NUMBER)) {
    cerr << "div: invalid type" << endl;
    return;
  }

  if (!(b.type == NUMBER)) {
    cerr << "div: invalid type" << endl;
    return;
  }

  s.push(a / b);
}

void Script::mod() {
  if (s.size() < 2) {
    cerr << "mod: stack underflow" << endl;
    return;
  }

  Item a, b;

  a = s.top(); s.pop();
  b = s.top(); s.pop();

  if (a.type == IMAGE && b.type == IMAGE) {
    b.i->math(a.i->im, MATH_MOD);
    s.push(b);
    return;
  }

  if (!(a.type == NUMBER)) {
    cerr << "mod: invalid type" << endl;
    return;
  }

  b = s.top();
  if (!(b.type == NUMBER)) {
    cerr << "mod: invalid type" << endl;
    return;
  }

  s.push(a % b);
}

void Script::pwr() {
  if (s.size() < 2) {
    cerr << "pwr: stack underflow" << endl;
    return;
  }
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "pwr: invalid type" << endl;
    return;
  }
  double y = s.top().n; s.pop();
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "pwr: invalid type" << endl;
    return;
  }
  double x = s.top().n; s.pop();
  double z = pow(x, y);
  Item item;
  item.type = NUMBER;
  item.n = z;
  s.push(item);
}

void Script::ln() {
  if (s.size() < 1) {
    cerr << "ln: stack underflow" << endl;
    return;
  }
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "ln: invalid type" << endl;
    return;
  }
  double x = s.top().n; s.pop();
  x = log(x);
  Item item;
  item.type = NUMBER;
  item.n = x;
  s.push(item);
}

void Script::neg() {
  if (s.size() < 1) {
    cerr << "neg: stack underflow" << endl;
    return;
  }
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "neg: invalid type" << endl;
    return;
  }
  double n = s.top().n; s.pop();
  Item item;
  item.type = NUMBER;
  item.n = -n;
  s.push(item);
}

// ------------------------------------------
// logical functions
// ------------------------------------------

void Script::lt() {
  if (s.size() < 2) {
    cerr << "lt: stack underflow" << endl;
    return;
  }

  Item item;
  item.type = BOOL;
  item.b = false;

  if (s.top().type == NUMBER) {
    double n1 = s.top().n; s.pop();
    if (s.top().type != NUMBER) {
      s.pop();
      cerr << "lt: invalid type" << endl;
    }
    double n2 = s.top().n; s.pop();
    if (n2 < n1) {
      item.b = true;
    }
  }
  else if (s.top().type == STRING) {
    string n1 = s.top().s; s.pop();
    if (s.top().type != STRING) {
      s.pop();
      cerr << "lt: invalid type" << endl;
    }
    string n2 = s.top().s; s.pop();
    if (n2 < n1) {
      item.b = true;
    }
  }
  else {
    cerr << "lt: invalid type" << endl;
    s.pop(); s.pop();
  }

  s.push(item);
}

void Script::eq() {
  if (s.size() < 2) {
    cerr << "eq: stack underflow" << endl;
    return;
  }

  Item item;
  item.type = BOOL;
  item.b = false;

  if (s.top().type == NUMBER) {
    double n1 = s.top().n; s.pop();
    if (s.top().type != NUMBER) {
      s.pop();
      cerr << "eq: invalid type" << endl;
    }
    double n2 = s.top().n; s.pop();
    if (n2 == n1) {
      item.b = true;
    }
  }
  else if (s.top().type == STRING) {
    string n1 = s.top().s; s.pop();
    if (s.top().type != STRING) {
      s.pop();
      cerr << "eq: invalid type" << endl;
    }
    string n2 = s.top().s; s.pop();
    if (n2 == n1) {
      item.b = true;
    }
  }
  else {
    cerr << "eq: invalid type" << endl;
    s.pop(); s.pop();
  }

  s.push(item);

}

void Script::ne() {
  if (s.size() < 2) {
    cerr << "ne: stack underflow" << endl;
    return;
  }

  Item item;
  item.type = BOOL;
  item.b = false;

  if (s.top().type == NUMBER) {
    double n1 = s.top().n; s.pop();
    if (s.top().type != NUMBER) {
      s.pop();
      cerr << "ne: invalid type" << endl;
    }
    double n2 = s.top().n; s.pop();
    if (n2 != n1) {
      item.b = true;
    }
  }
  else if (s.top().type == STRING) {
    string n1 = s.top().s; s.pop();
    if (s.top().type != STRING) {
      s.pop();
      cerr << "ne: invalid type" << endl;
    }
    string n2 = s.top().s; s.pop();
    if (n2 != n1) {
      item.b = true;
    }
  }
  else {
    cerr << "ne: invalid type" << endl;
    s.pop(); s.pop();
  }

  s.push(item);
}

void Script::gt() {
  if (s.size() < 2) {
    cerr << "gt: stack underflow" << endl;
    return;
  }

  Item item;
  item.type = BOOL;
  item.b = false;

  if (s.top().type == NUMBER) {
    double n1 = s.top().n; s.pop();
    if (s.top().type != NUMBER) {
      s.pop();
      cerr << "gt: invalid type" << endl;
    }
    double n2 = s.top().n; s.pop();
    if (n2 > n1) {
      item.b = true;
    }
  }
  else if (s.top().type == STRING) {
    string n1 = s.top().s; s.pop();
    if (s.top().type != STRING) {
      s.pop();
      cerr << "gt: invalid type" << endl;
    }
    string n2 = s.top().s; s.pop();
    if (n2 > n1) {
      item.b = true;
    }
  }
  else {
    cerr << "gt: invalid type" << endl;
    s.pop(); s.pop();
  }

  s.push(item);
}

void Script::doif() {
  if (s.size() < 2) {
    cerr << "doif: stack underflow" << endl;
    return;
  }

  // if a reference, replace the reference with the real thing
  if (s.top().type == STRING && s.top().s[0] == '&') {
    Item item;
    item = s.top(); s.pop();
    string name = item.s;
    name = name.substr(1);
    viter = v.find(name);
    if (viter != v.end()) {
      item = v[viter->first];
      s.push(item);
    }
    else {
      s.push(item);
    }
  }

  // get proc
  if (s.top().type != PROC) {
    cerr << "doif: invalid type, expecting PROC" << endl;
    s.pop();
    return;
  }
  Item proc = s.top(); s.pop();

  // get condition
  if (s.top().type != BOOL) {
    s.pop();
    cerr << "doif: invalid type, expecting BOOL" << endl;
    return;
  }
  bool condition = s.top().b; s.pop();

  // exec proc?
  if (condition) {
    s.push(proc);
    exec();
  }

}

void Script::foreach() {
  if (s.size() < 2) {
    cerr << "foreach: stack underflow" << endl;
    return;
  }

  // if a reference, replace the reference with the real thing
  if (s.top().type == STRING && s.top().s[0] == '&') {
    Item item;
    item = s.top(); s.pop();
    string name = item.s;
    name = name.substr(1);
    viter = v.find(name);
    if (viter != v.end()) {
      item = v[viter->first];
      s.push(item);
    }
    else {
      s.push(item);
    }
  }

  // get proc
  if (s.top().type != PROC) {
    cerr << "foreach: invalid type" << endl;
    s.pop();
    return;
  }
  Item proc = s.top(); s.pop();

  // if a reference, replace the reference with the real thing
  if (s.top().type == STRING && s.top().s[0] == '&') {
    Item item;
    item = s.top(); s.pop();
    string name = item.s;
    name = name.substr(1);
    viter = v.find(name);
    if (viter != v.end()) {
      item = v[viter->first];
      s.push(item);
    }
    else {
      s.push(item);
    }
  }

  // get list or dict
  if (!(s.top().type == LIST || s.top().type == DICT)) {
    s.pop();
    cerr << "foreach: invalid type, expecting LIST or DICT" << endl;
    return;
  }
  Item container = s.top(); s.pop();

  // exec proc for each member of a list
  if (container.type == LIST) {
    std::list<Item>::iterator iter;
    for (iter = container.l.begin(); iter != container.l.end(); iter++) {
      s.push(*iter);
      s.push(proc);
      exec();
    }
  }

  // exec proc for each member of a dict
  if (container.type == DICT) {
    Item item;
    std::map<string, Item>::iterator iter;
    for (iter = container.d.begin(); iter != container.d.end(); iter++) {
      item.type = STRING;
      item.s = iter->first;
      s.push(item);
      s.push(container.d[iter->first]);
      s.push(proc);
      exec();
    }
  }

}

void Script::dofor() {
  if (s.size() < 4) {
    cerr << "dofor: stack underflow" << endl;
    return;
  }

  // if a reference, replace the reference with the real thing
  if (s.top().type == STRING && s.top().s[0] == '&') {
    Item item;
    item = s.top(); s.pop();
    string name = item.s;
    name = name.substr(1);
    viter = v.find(name);
    if (viter != v.end()) {
      item = v[viter->first];
      s.push(item);
    }
    else {
      s.push(item);
    }
  }

  // get proc
  if (s.top().type != PROC) {
    cerr << "dofor: invalid type" << endl;
    s.pop();
    return;
  }
  Item proc = s.top(); s.pop();

  // get end
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "dofor: invalid type, expecting NUMBER" << endl;
    return;
  }
  double end = s.top().n; s.pop();

  // get step
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "dofor: invalid type, expecting NUMBER" << endl;
    return;
  }
  double step = s.top().n; s.pop();

  // get start
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "dofor: invalid type, expecting NUMBER" << endl;
    return;
  }
  double start = s.top().n; s.pop();

  // do for loop
  Item item;
  for (double i = start; i < end; i += step) {

    // push current iteration number
    item.type = NUMBER;
    item.n = i;
    s.push(item);

    // execute proc
    s.push(proc);
    exec();

  }

}

// ------------------------------------------
// list, dict and proc operations
// ------------------------------------------

// put all elements from LIST or DICT onto stack
void Script::load() {
  if (s.size() < 1) {
    cerr << "load: stack underflow" << endl;
    return;
  }

  // if a reference, replace the reference with the real thing
  if (s.top().type == STRING && s.top().s[0] == '&') {
    Item item;
    item = s.top(); s.pop();
    string name = item.s;
    name = name.substr(1);
    viter = v.find(name);
    if (viter != v.end()) {
      item = v[viter->first];
      s.push(item);
    }
    else {
      s.push(item);
    }
  }

  // make sure we have the right thing on the stack
  if (!(s.top().type == LIST || s.top().type == DICT)) {
    cerr << "load: invalid type, expecting LIST or DICT" << endl;
    return;
  }
  Item container = s.top(); s.pop();

  // dump a list onto the stack
  if (container.type == LIST) {
    std::list<Item>::iterator iter;
    for (iter = container.l.begin(); iter != container.l.end(); iter++) {
      s.push(*iter);
    }
  }

  // dump a dict onto the stack
  else {
    Item item;
    std::map<string, Item>::iterator iter;
    for (iter = container.d.begin(); iter != container.d.end(); iter++) {
      item.type = STRING;
      item.s = iter->first;
      s.push(item);
      s.push(container.d[iter->first]);
    }
  }

}

void Script::makeList() {
  if (s.size() < 1) {
    cerr << "makeList: stack underflow" << endl;
    return;
  }

  // create a new list
  Item item;
  item.type = LIST;

  // read stack until starting '('
  Item o = s.top(); s.pop();
  while (o.s != "(") {
    item.l.push_front(o);
    o = s.top(); s.pop();
  }

  // push list onto stack
  s.push(item);
}

void Script::makeDict() {
  if (s.size() < 1) {
    cerr << "makeDict: stack underflow" << endl;
    return;
  }

  // get value from stack
  Item value = s.top(); s.pop();

  // is this an empty dict?
  if (value.type == STRING && value.s == "[") {
    Item item;
    item.type = DICT;
    s.push(item);
    return;
  }

  if (s.size() < 1) {
    cerr << "makeDict: stack underflow" << endl;
    return;
  }

  // get key from stack
  Item key = s.top(); s.pop();
  if (key.type != STRING) {
    cerr << "makeDict: invalid key '" << key << "' in dict" << endl;
    cerr << "  value = " << value << endl;
    return;
  }

  // new item
  Item item;
  item.type = DICT;

  // build new dictionary
  while (value.s != "[") {

    // add item to dictionary
    item.d[key.s] = value;

    // get value from stack
    value = s.top(); s.pop();
    if (value.type == STRING && value.s == "[") break;
    if (s.size() < 1) {
      cerr << "makeDict: stack underflow" << endl;
      return;
    }

    // get key from stack
    key = s.top(); s.pop();
    if (key.type != STRING) {
      cerr << "makeDict: invalid key '" << key << "' in dict" << endl;
      cerr << "  value = " << value << endl;
      return;
    }
  }

  // push dictionary onto stack
  s.push(item);
}

void Script::makeProc() {
  if (s.size() < 1) {
    cerr << "makeProc: stack underflow" << endl;
    return;
  }

  // create a new proc
  Item item;
  item.type = PROC;

  // read stack until starting '{'
  Item o = s.top(); s.pop();
  while (o.s != "{") {
    item.p.push_front(o);
    o = s.top(); s.pop();
  }
  s.push(item);

  // unwind 1 level
  inproc--;
}

// create an empty list on stack
void Script::list() {
  Item item;
  item.type = LIST;
  s.push(item);
}

// create an empty dict on stack
void Script::dict() {
  Item item;
  item.type = DICT;
  s.push(item);
}

// create an empty proc on stack
void Script::proc() {
  Item item;
  item.type = PROC;
  s.push(item);
}

// execute a procedure
void Script::exec() {
  if (s.size() < 1) {
    cerr << "exec: stack underflow" << endl;
    return;
  }

  // if a reference, replace the reference with the real thing
  if (s.top().type == STRING && s.top().s[0] == '&') {
    Item item;
    item = s.top(); s.pop();
    string name = item.s;
    name = name.substr(1);
    viter = v.find(name);
    if (viter != v.end()) {
      item = v[viter->first];
      s.push(item);
    }
    else {
      s.push(item);
    }
  }

  // check for proc
  if (s.top().type != PROC) {
    cerr << "exec: invalid type" << endl;
    cout << "  type = " << s.top().type << endl;
    cout << "  " << s.top() << endl;
    s.pop();
    return;
  }

  Item item;

  std::list<Item> plist = s.top().p; s.pop();
  std::list<Item>::iterator iter;
  for (iter = plist.begin(); iter != plist.end(); iter++) {

    switch (iter->type) {
    case BOOL:
    case NUMBER:
    case IMAGE:
    case LIST:
    case DICT:
      s.push(*iter);
      break;
    case STRING:

      // see if this is an operator, function or a variable
      fiter = f.find(iter->s);
      viter = v.find(iter->s);
      oiter = o.find(iter->s);

      // if a function, execute
      if (fiter != f.end()) {
        // here be magic!
        // p is a pointer to a member function
        // f[str] is a Function structure that has p as a member
        // it works because we are executing a member of this
        (this->*(f[iter->s].p))();
        //(*this.*(f[str].p))();
      }

      // if a variable, push value
      else if (viter != v.end()) {
        item = v[viter->first];
        s.push(item);

        // if this is a procedure, execute it
        if (item.type == PROC) {
          exec();
        }
      }

      // if an operator, execute it
      else if (oiter != o.end()) {
        (this->*(o[iter->s].p))();
      }

      // something else, push it
      else {
        s.push(*iter);
      }
      break;
    case PROC:
      s.push(*iter);
      break;
    }

  }

}

