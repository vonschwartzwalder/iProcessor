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
// add a node to the dictionary
// -------------------------------------
void Graph::add_vertex(Shape s) {
  dictionary[s.label] = s;
}

// -------------------------------------
// remove a node from the adjacency list
// -------------------------------------
void Graph::remove_vertex(Shape s) {
  remove_vertex(s.label);
}

void Graph::remove_vertex(int node) {
  set<int>::iterator siter;
  for(siter = adjacency[node].begin(); siter != adjacency[node].end(); siter++) { 
    adjacency[(*siter)].erase(node);
  }
  adjacency.erase(node);
}

// -------------------------------------
// delete this vertex from the shape dictionary
// -------------------------------------
void Graph::delete_vertex(Shape s) {
  delete_vertex(s.label);
}

void Graph::delete_vertex(int node) {
  dictionary.erase(node);
}

// -------------------------------------
// shapes iterators
// -------------------------------------
map<int, Shape>::iterator Graph::d_begin() {
  return dictionary.begin();
}
map<int, Shape>::iterator Graph::d_end() {
  return dictionary.end();
}

// -------------------------------------
// add an edge to the adjacency list
//   no self reference allowed
// -------------------------------------
void Graph::add_edge(Shape s1, Shape s2) {
  add_edge(s1.label, s2.label);
}

void Graph::add_edge(int node1, int node2) {
  if(node1 != node2) {
    adjacency[node1].insert(node2);
    adjacency[node2].insert(node1);
  }
}

// -------------------------------------
// remove an edge from the adjacency list
// -------------------------------------
void Graph::remove_edge(Shape s1, Shape s2) {
  remove_edge(s1.label, s2.label);
}

void Graph::remove_edge(int node1, int node2) {
  adjacency[node1].erase(node2);
  adjacency[node2].erase(node1);
} // remove edge

// -------------------------------------
// remove a node from data structures
// -------------------------------------
void Graph::clear_vertex(Shape s) {
  clear_vertex(s.label);
}

void Graph::clear_vertex(int node) {
  
  // remove node from dictionary
  dictionary.erase(node);

  // remove node from adjacency
  set<int>::iterator siter;
  for(siter = adjacency[node].begin(); siter != adjacency[node].end(); siter++) {
    adjacency[*siter].erase(node);
  }
  adjacency.erase(node);

} // clear_vertex


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
int Graph::dictionary_size() {
  return dictionary.size();
}
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

// --------------------------------------------------------
// depth first search (non-recursive)
// --------------------------------------------------------
void Graph::dfs(int snode) {

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
        cerr << "dfs visit " << node;
        bool first = true;
        set<int>::iterator siter;
        for(siter = adjacency[node].begin(); siter != adjacency[node].end(); siter++) {
          if(visited[*siter] == 0) {
            cerr << (first ? " (" : ",") << *siter;
            first = false;
          }
        }
        cerr << (first ? "" : ")") << endl;
      }

    }
  }

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
// remove shapes touching the boundary (0)
// --------------------------------------------------------
int Graph::boundary_shapes() {
  int count = 0;
  int index;
  stack<int> to_process;
  stack<int> to_delete;
  set<int>::iterator iter;

  // delete what?
  map<int, set<int> >::iterator miter;
  for(miter = adjacency.begin(); miter != adjacency.end(); miter++) {
    if(miter->second.find(0) != miter->second.end()) {
      index = miter->first;
      to_process.push(index);
      count++;
    }
  }

  // make all shapes adjacent to deleted shapes adjacent to shape 0
  while(!to_process.empty()) {
    index = to_process.top(); to_process.pop();
    to_delete.push(index);
    for(iter = adjacency[index].begin(); iter != adjacency[index].end(); iter++) {
      if(*iter != 0) {
        adjacency[0].insert(*iter);
        adjacency[*iter].insert(0);
      }
    }
  }

  // delayed delete
  while(!to_delete.empty()) {
    index = to_delete.top(); 
    to_delete.pop();

    clear_vertex(index);
  }

  return count;
}

// --------------------------------------------------------
// remove shapes of area size or smaller
// --------------------------------------------------------
int Graph::prune_shapes(int size) {
  int count = 0;
  int index;
  stack<int> to_delete;

  // find shapes to delete
  map<int, Shape>::iterator miter;
  for(miter = dictionary.begin(); miter != dictionary.end(); miter++) {
    if(miter->second.area < size) {
      index = miter->first;
      to_delete.push(index);
      count++;
    }
  }

  // do the deletions
  while(!to_delete.empty()) {
    index = to_delete.top(); to_delete.pop();
    clear_vertex(index);
  }

  return count;
}

// --------------------------------------------------------
// coalesce shapes with areas size (percentage) difference or less
// --------------------------------------------------------
int Graph::coalesce_shapes(float size) {
  int count = 0;
  int index;
  stack<int> to_delete;
  Shape shape1;
  Shape shape2;

  // coalesce using adjacency list
  map<int, set<int> >::iterator miter;
  set<int>::iterator siter;
  for(miter = adjacency.begin(); miter != adjacency.end(); miter++) {
    shape1 = dictionary[miter->first];
    for(siter = miter->second.begin(); siter != miter->second.end(); siter++) {
      shape2 = dictionary[*siter];
      if(contains[shape1.label].find(shape2.label) != contains[shape1.label].end()) {
        cerr << "shape " << shape1.label << " contains " << shape2.label << endl;
        if(((float)(shape2.area) / (float)(shape1.area)) > size) {
          to_delete.push(shape2.label);
          count++;
        }
      }
    }
  }

  // execute deferred deletions
  while(!to_delete.empty()) {
    index = to_delete.top(); to_delete.pop();
    clear_vertex(index);
  }

  return count;
}

// --------------------------------------------------------
// find articulation points, biconnected components and bridges
// --------------------------------------------------------
// vertex 0 is treated as an all including node
// it is not included in articulation points etc.. even though
// it may be.

void Graph::biconnected_components()
{
  now = 0;

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
  area[k] = dictionary[k].pixel_count;

  for(t = adjacency[k].begin(); t != adjacency[k].end(); t++) {
    if(visited[*t] == 0) {

      b = biconnected(*t);
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

        dictionary[k].area += area[*t];

      } else {

        // k is not an articulation point with respect to the edge (k, *t)
        // add the subgraph *t to the return value
        rval->splice(rval->begin(), *b);

      }

      area[k] += area[*t];
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
// dump all the nodes in the dictionary
// --------------------------------------------------------
void Graph::dump_dictionary(ostream &out) {
  map<int, Shape>::iterator iter;
  for(iter = dictionary.begin(); iter != dictionary.end(); iter++) {
    out << iter->first << " : " << iter->second << endl;
  }
} // dump_dictionary


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

