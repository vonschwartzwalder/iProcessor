// ---------------------------------------------------------------------
//
//  Graph declaration
//
//  Provide a graph representation
//
// ---------------------------------------------------------------------

#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <list>
#include <set>
#include <stack>
#include "shape.h"

class Graph {

  public:
  // -------------------------------------
  // constructors, destructor
  // -------------------------------------
  Graph();
  //Graph(const Graph &g);
  ~Graph();

  // -------------------------------------
  // data access
  // -------------------------------------
  map<int, set<int> >::iterator a_begin();  // adjacency iterator
  map<int, set<int> >::iterator a_end();    // adjacency iterator

  int adjacency_size();
  int aps_size();
  int bic_size();
  int bridge_size();

  void addNode(int s);                    // add a node to graph
  void addEdge(int s1, int s2);           // add an edge to graph
  void deleteNode(int s);                 // remove a node from graph
  void deleteEdge(int s1, int s2);        // remove an edge from graph
  int  nodes();                           // number of nodes in graph
  int  edges();                           // number of edges in graph
  bool isLeaf(int s);                     // is this a leaf node?

  // -------------------------------------
  // algorithms
  // -------------------------------------
  list<int> &dfs(int snode = 0);          // depth first search
  void rdfs(int node = 0);                // recursive dfs

  void biconnected_components();          // find both biconnected components
                                          // and articulation points
  list<int> * biconnected(int k);
  void gcolor_dfs(int snode);             // color the nodes for drawing

  // -------------------------------------
  // operators
  // -------------------------------------
  friend ostream &operator<<(ostream &out, Graph &g);
  friend istream &operator>>(istream &in, Graph &g);
  
  // -------------------------------------
  // I/O
  // -------------------------------------
  void dump_adjacency(ostream &out);   // write adjacency list
  void dump_aps(ostream &out);         // write articulation points
  void dump_bic(ostream &out);         // write biconnected components
  void dump_bridge(ostream &out);      // write bridges
  void dump_graph(ostream &out);       // write adjacency list for Mathematica
  
  // -------------------------------------
  // data
  // -------------------------------------
  public:

  map<int, set<int> > adjacency;    // adjacency list
  map<int, set<int> > contains;     // directly contains

  list<int> aps;                    // articulation points
  map<int, bool> articulation;
  list<list <int> > bic;	          // biconnected components
  list<list <int> > bridge;	        // bridges

  map<int, int> visited;            // visited nodes
  map<int, int> minimum_reachable;  

  int now;
};

#endif
