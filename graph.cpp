// --------------------------------------------------------
//  Graph implementation
//
//  Provide a graph representation
// --------------------------------------------------------

#include "graph.h"

// default constructor
Graph::Graph() {
}

// destructor
Graph::~Graph() {
}

// -------------------------------------
// adjacency iterators
// -------------------------------------
map<int, set<int> >::iterator Graph::a_begin() {
  return adjacency.begin();
}
map<int, set<int> >::iterator Graph::a_end() {
  return adjacency.end();
}

// -------------------------------------
// data structure sizes
// -------------------------------------
int Graph::adjacency_size() {
  return adjacency.size();
}
int Graph::aps_size() {
  return aps.size();
}
int Graph::bic_size() {
  return bic.size();
}
int Graph::bridge_size() {
  return bridge.size();
}

// -------------------------------------
// add a node to the adjacency list
// -------------------------------------
void Graph::addNode(int s) {
  adjacency[s].erase(s);
}

// -------------------------------------
// add an edge to the adjacency list
// -------------------------------------
void Graph::addEdge(int s1, int s2) {
  if(s1 != s2) {
    adjacency[s1].insert(s2);
    adjacency[s2].insert(s1);
  }
}

// -------------------------------------
// remove a node from the graph
// -------------------------------------
void Graph::deleteNode(int s) {
  set<int>::iterator siter;
  for(siter = adjacency[s].begin(); siter != adjacency[s].end(); siter++) {
    adjacency[*siter].erase(s);
  }
  adjacency.erase(s);
}

// -------------------------------------
// remove an edge from the graph (undirected)
// -------------------------------------
void Graph::deleteEdge(int s1, int s2) {
  adjacency[s1].erase(s2);
  adjacency[s2].erase(s1);
}

// -------------------------------------
// count the number of nodes in the graph
// -------------------------------------
int Graph::nodes() {
  return adjacency.size();
}

// -------------------------------------
// count the number of edges in the graph
// -------------------------------------
int Graph::edges() {
  int count = 0;
  map<int, set<int> >::iterator miter;
  for(miter = adjacency.begin(); miter != adjacency.end(); miter++) {
    count += miter->second.size();
  }
  return count;
}

// -------------------------------------
// see if this is a leaf node (less than two nodes adjacent)
// -------------------------------------
bool Graph::isLeaf(int s) {
  if(adjacency[s].size() < 2) return true;
  return false;
}

// --------------------------------------------------------
// depth first search (non-recursive)
// --------------------------------------------------------
list<int> &Graph::dfs(int snode) {

  // list to return
  list<int> *nodes = new list<int>;
  
  int node = snode;
  stack<int> s;
  set<int>::reverse_iterator siter;

  // initialize visited
  visited.clear();

  // put first node on stack
  if(adjacency.find(node) != adjacency.end()) {
    s.push(node);
  }

  // visit each node in stack
  while(! s.empty()) {

    // get current node from stack
    node = s.top(); s.pop();

    // if we have not been here, visit
    if(visited[node] == 0) {
      visited[node]++;

      // push adjacent nodes we haven't seen on stack
      for(siter = adjacency[node].rbegin(); siter != adjacency[node].rend(); siter++) {
        if(visited[*siter] == 0) {
          s.push(*siter);
        }
      }

      // do something
      {
        //cerr << "dfs visit " << node;
        nodes->push_back(node);
        
//        bool first = true;
//        set<int>::iterator siter;
//        for(siter = adjacency[node].begin(); siter != adjacency[node].end(); siter++) {
//          if(visited[*siter] == 0) {
//            cerr << (first ? " (" : ",") << *siter;
//            first = false;
//          }
//        }
//        cerr << (first ? "" : ")") << endl;
        
      }

    }
  }

  return *nodes;
  
} // dfs


// --------------------------------------------------------
// depth first search (recursive)
// --------------------------------------------------------
void Graph::rdfs(int node) {

  // visit this node
  visited[node]++;

  // print trace
  cerr << "dfs visit " << node;
  bool first = true;
  set<int>::iterator siter1;
  for(siter1 = adjacency[node].begin(); siter1 != adjacency[node].end(); siter1++) {
    if(visited[*siter1] == 0) {
      cerr << (first ? " (" : ",") << *siter1;
      first = false;
    }
  }
  cerr << (first ? "" : ")") << endl;

  // visit adjacent nodes we haven't seen before
  set<int>::iterator siter;
  for(siter = adjacency[node].begin(); siter != adjacency[node].end(); siter++) {
    if(visited[*siter] == 0) {
      rdfs(*siter);
    }
  }

} // rdfs

// --------------------------------------------------------
// find articulation points, biconnected components and bridges
// --------------------------------------------------------
// vertex 0 is treated as an all including node
// it is not included in articulation points etc.. even though
// it may be.

int depth;
int maxdepth;

void Graph::biconnected_components()
{
  now = 0;
  depth = 0;
  maxdepth = 0;

  contains.clear();
  aps.clear();
  articulation.clear();
  bic.clear();
  bridge.clear();
  visited.clear();
  minimum_reachable.clear();

  delete biconnected(0);

  aps.sort();
  aps.unique();
}

list<int> * Graph::biconnected(int k)
{
  set<int>::iterator t;
  list<int>::iterator u;

  list<int> *b;
  list<int> *rval;

  int min;
  int m;

  rval = new list<int>;
  rval->push_front(k);

  visited[k] = ++now;
  min = now;

  for(t = adjacency[k].begin(); t != adjacency[k].end(); t++) {
    if(visited[*t] == 0) {

      depth++;
      if (depth>maxdepth) maxdepth=depth;
      b = biconnected(*t);
      depth--;
      m = minimum_reachable[*t];

      // get smallest reachable node
      min = ( m < min ) ? m : min;

      if(m >= (visited[k])) {
        if(k != 0) {
          // k is an articulation point with respect to edge (k, *t)
          aps.push_front(k);    // record that the node is an articulation
                                // point

          articulation[k] = true;

          b->push_front(k);
          bic.push_front(*b);
          if(b->size()==2) bridge.push_front(*b);

          for(u = b->begin(); u != b->end(); u++) {
            if(adjacency[k].end() != adjacency[k].find(*u)) {
              contains[k].insert(*u);
            }
          }

        }

      } else {

        // k is not an articulation point with respect to the edge (k, *t)
        // add the subgraph *t to the return value
        rval->splice(rval->begin(), *b);

      }

      delete b;

    } else {  // visited

      // *t has been visited
      min = (visited[*t] < min) ? visited[*t] : min;
    }
  }   // end for

  minimum_reachable[k] = min;
  return rval;
}

// --------------------------------------------------------
// color nodes (four color algorithm eventually)
// --------------------------------------------------------
void Graph::gcolor_dfs(int snode) {

  int node = snode;
  int gcolor = 0;
  stack<int> s;
  set<int>::reverse_iterator siter;

  // initialize visited
  visited.clear();

  // put first node on stack
  if(adjacency.find(node) != adjacency.end()) {
    s.push(node);
  }

  // visit each node in stack
  while(! s.empty()) {

    // get current node from stack
    node = s.top(); s.pop();

    // if we have not been here, visit
    if(visited[node] == 0) {
      visited[node]++;

      // push adjacent nodes we haven't seen on stack
      for(siter = adjacency[node].rbegin(); siter != adjacency[node].rend(); siter++) {
        if(visited[*siter] == 0) {
          s.push(*siter);
        }
      }

      // do something
      {
        // at each node.
        gcolor++;
        //dictionary[node].gcolor=(gcolor) % 4;
      }

    }
  }

} // dfs

// -------------------------------------
// I/O
// -------------------------------------
ostream &operator<<(ostream &out, Graph &g) {
  map<int, set<int> >::iterator miter;
  set<int>::iterator siter;

  // adjacency list
  out << "adjacency {" << endl;
  for(miter = g.adjacency.begin(); miter != g.adjacency.end(); miter++) {
    out << "  " << (*miter).first << ":";
    if(!(*miter).second.empty()) {
      for(siter = (*miter).second.begin(); siter != (*miter).second.end();) {
        out << " " << *siter;
        siter++;
        if(siter != (*miter).second.end()) {
          out << ",";
        }
      }
    }
    out << endl;
  }
  out << "}" << endl;
  return out;
}


istream &operator>>(istream &in, Graph &g) {
  int cp;
  string id;
  Point p;

  // look for an adjacency list
  while(id != "adjacency" && !in.eof()) {

    // find start of adjacency list
    cp = in.peek();
    while(cp != 'a' && !in.eof()) {
      in.get();
      cp = in.peek();
    }

    // get string
    cp = in.peek();
    while(isalpha(cp) && !in.eof()) {
      id += in.get();
      cp = in.peek();
    }

    if(id != "adjacency") id = "";
  }
  if(in.eof()) return in;

  // skip white space
  cp = in.peek();
  while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    in.get();
    if(in.eof()) return in;
    cp = in.peek();
  }

  // get starting brace
  cp = in.peek();
  if(cp != '{') {
    cerr << "no starting brace" << endl;
    return in;
  }
  else in.get();
  if(in.eof()) return in;

  // skip white space
  cp = in.peek();
  while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    in.get();
    if(in.eof()) return in;
    cp = in.peek();
  }

  // read adjacencies until closing brace
  int node, edge;
  while(cp != '}') {

    // skip white space
    cp = in.peek();
    while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
      in.get();
      if(in.eof()) return in;
      cp = in.peek();
    }

    // get a node
    if(!isdigit(cp)) {
      cerr << "not a number" << endl;
      return in;
    }
    if(in.eof()) return in;
    in >> node;

    // add node to adjacency
    g.addNode(node);

    // skip white space
    cp = in.peek();
    while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
      cp = in.get();
      if(in.eof()) return in;
      cp = in.peek();
    }

    // get separator
    if(!(cp == ':')) {
      cerr << "illegal character! (" << cp << ")" << endl;
      return in;
    }
    cp = in.get();
    if(in.eof()) return in;

    // read edges until newline
    cp = in.peek();
    while(!(cp == '\n' || cp == '\r')) {

      // read an edge
      in >> edge;
      if(in.eof()) return in;

      // add an edge to adjacency
      g.addEdge(node, edge);

      // skip white space
      cp = in.peek();
      while(cp == ',' || cp == ' ' || cp == '\t') {
        in.get();
        if(in.eof()) return in;
        cp = in.peek();
      }

    }
    in.get();
    if(in.eof()) return in;

    // skip white space
    cp = in.peek();
    while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
      in.get();
      if(in.eof()) return in;
      cp = in.peek();
    }
  }

  return in;
}



// --------------------------------------------------------
// dump adjacency list
// --------------------------------------------------------
void Graph::dump_adjacency(ostream &out) {
  map<int, set<int> >::iterator miter;
  set<int>::iterator siter;

  // normal list
  for(miter = adjacency.begin(); miter != adjacency.end(); miter++) {
    out << (*miter).first << ":";
    if(!(*miter).second.empty()) {
      for(siter = (*miter).second.begin(); siter != (*miter).second.end();) {
        out << " " << *siter;
        siter++;
        if(siter != (*miter).second.end()) {
          out << ",";
        }
      }
    }
    out << endl;
  }

} // dump_adjacency


// --------------------------------------------------------
// dump graph for Mathematica
// --------------------------------------------------------
void Graph::dump_graph(ostream &out) {
  map<int, set<int> >::iterator miter;
  set<int>::iterator siter;

  // iterate through adjacency list
  out << "{";
  for(miter = adjacency.begin(); miter != adjacency.end();) {
    if(!(*miter).second.empty()) {
      for(siter = (*miter).second.begin(); siter != (*miter).second.end();) {
        out << "{" << (*miter).first + 1 << "," << *siter + 1 << "}";
        siter++;
        if(siter != (*miter).second.end()) {
          out << ",";
        }
      }
      miter++;
      if(miter != adjacency.end()) {
        out << ",";
      }
    }
  }
  out << "}" << endl;


  // dump coordinates related to shape positions

} // dump_graph


// --------------------------------------------------------
// dump articulation points
// --------------------------------------------------------
void Graph::dump_aps(ostream &out) {
  bool first = true;
  list<int>::iterator siter;
  for(siter = aps.begin(); siter != aps.end(); siter++) {
    out << (first ? "(" : ",") << *siter;
    first = false;
  }
  out << (first ? "" : ")") << endl;
}

// --------------------------------------------------------
// dump biconnected components
// --------------------------------------------------------
void Graph::dump_bic(ostream &out) {
  bool first = true;
  list<list<int> >::iterator siter1;
  list<int>::iterator siter2;
  for(siter1 = bic.begin(); siter1 != bic.end(); siter1++) {
    out << "(";
    for(siter2 = siter1->begin(); siter2 != siter1->end(); siter2++) {
      out << (first ? "" : ",") << *siter2;
      first = false;
    }
    out << ")" << endl;
    first = true;
  }
  out << endl;
}

// --------------------------------------------------------
// dump bridges
// --------------------------------------------------------
void Graph::dump_bridge(ostream &out) {
  bool first = true;
  list<list<int> >::iterator siter1;
  list<int>::iterator siter2;
  for(siter1 = bridge.begin(); siter1 != bridge.end(); siter1++) {
    out << "(";
    for(siter2 = siter1->begin(); siter2 != siter1->end(); siter2++) {
      out << (first ? "" : ",") << *siter2;
      first = false;
    }
    out << ")" << endl;
    first = true;
  }
  out << endl;
}

void Graph::writeSexpr(ostream &out)
{
  map<int, set<int> >::iterator miter;
  set<int>::iterator siter;

  out << " (edges";
  for (miter = adjacency.begin(); miter != adjacency.end(); miter++) {
    out << " (" << (*miter).first;
    if (!(*miter).second.empty()) {
      for (siter = (*miter).second.begin(); siter != (*miter).second.end(); siter++) {
        out << " " << *siter;
      }
      out << ")";
    }
  }
  out << ")" << endl;
}