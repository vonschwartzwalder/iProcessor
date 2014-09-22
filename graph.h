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
  void add_vertex(Shape s);               // add a shape to the shape dictionary
  void add_vertex(int s);       
  void remove_vertex(Shape s);            // remove a shape from the adjacency list
  void remove_vertex(int s);
  void clear_vertex(Shape s);             // remove all edges touching this vertex
  void clear_vertex(int s);
  void delete_vertex(Shape s);            // delete this vertex from the shape dict
  void delete_vertex(int s);
  map<int, Shape>::iterator d_begin();    // shape iterator
  map<int, Shape>::iterator d_end();      // shape iterator

  void add_edge(Shape s1, Shape s2);      // add an edge to the adjacency list
  void add_edge(int s1, int s2);
  void remove_edge(Shape s1, Shape s2);   // remove an edge from the adjacency list
  void remove_edge(int s1, int s2);
  //void get_edges(Shape s);                // get iterator to edges touching this vertex
  map<int, set<int> >::iterator a_begin();  // adjacency iterator
  map<int, set<int> >::iterator a_end();    // adjacency iterator

  int dictionary_size();
  int adjacency_size();
  int aps_size();
  int bic_size();
  int bridge_size();
  
  // -------------------------------------
  // algorithms
  // -------------------------------------
  void dfs(int snode = 0);                // depth first search
  void rdfs(int node = 0);                // recursive dfs
  int  boundary_shapes();                 // remove boundary shapes
  int  prune_shapes(int size);            // remove shapes size and smaller
  int  coalesce_shapes(float size);       // coalesce shapes

  void biconnected_components(); // find both biconnected components
                                          // and articulation points
  list<int> * biconnected(int k);

  // -------------------------------------
  // operators
  // -------------------------------------
  friend ostream &operator<<(ostream &out, Graph &g);
  //friend ostream &operator>>(istream &in, Graph &g);
  
  // -------------------------------------
  // I/O
  // -------------------------------------
  void dump_dictionary(ostream &out);  // write all the vertices in the dictionary
  void dump_adjacency(ostream &out);   // write adjacency list
  void dump_aps(ostream &out);         // write articulation points
  void dump_bic(ostream &out);         // write biconnected components
  void dump_bridge(ostream &out);      // write bridges
  
  // -------------------------------------
  // data
  // -------------------------------------
  public:
  typedef map<int, Shape>::iterator d_iterator;
  typedef map<int, set<int> >::iterator a_iterator;

  //private:
  map<int, set<int> > adjacency;    // adjacency list
  map<int, set<int> > contains;     // directly contains

  map<int, int>  area;   // strict OO should not know about area.

  map<int, Shape> dictionary;       // dictionary of vertices

  list<int> aps;                    // articulation points
  list<list <int> > bic;	          // biconnected components
  list<list <int> > bridge;	        // bridges

  map<int, int> visited;            // visited nodes
  map<int, int> visited0;            // visited nodes
  map<int, int> minimum_reachable;  

  map<int, bool> articulation;
  map<int, bool> not_dfs_leaf;             // dfs leaf 
 
  int now;
};

#endif
