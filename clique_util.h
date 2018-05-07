//
// Created by yifan on 4/22/18.
//

#ifndef ND_CLIQUE_UTIL_H
#define ND_CLIQUE_UTIL_H

void incrementCN(com_node *root, vector<vertex>& clique, Graph& ordered4Cs, Graph& s4cs);
void increment_clique(com_node *root, initializer_list<vertex> elements, Graph& ordered4Cs, Graph& s4cs, vector<vertex>& clique, int index, size_t start, size_t end);
vertex getId_recur(com_node *node, initializer_list<vertex> rc, size_t x);
vertex getId(com_node *root, initializer_list<vertex> rc);
void create_triangleList (com_node *root, Graph& orderedGraph);
void create_triangleList (com_node *root, Graph& orderedGraph, int *triangle_num);
void create_4cliqueList (com_node *root, Graph& orderedGraph);
void create_4cliqueList (com_node *root, Graph& orderedGraph, int* clique_num);
void create_4cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered4Cs, vector<v4c>& r4cs, vector<vertex>& xr4cs, Graph& s5cs);
void create_5cliqueList (com_node *root, Graph& orderedGraph);
void create_5cliqueList (com_node *root, Graph& orderedGraph, int *clique_num);
void create_5cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered5Cs, vector<v5c>& r5cs, vector<vertex>& xr5cs, Graph& s6cs);
void create_6cliqueList (com_node *root, Graph& orderedGraph);
void create_6cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered6Cs, vector<v6c>& r6cs, vector<vertex>& xr6cs, Graph& s7cs);
void create_6cliqueList (com_node *root, Graph& orderedGraph, int *clique_num);
void create_7cliqueList (com_node *root, Graph& orderedGraph);
void create_7cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered7Cs, vector<v7c>& r7cs, vector<vertex>& xr7cs, Graph& s8cs);
void create_7cliqueList (com_node *root, Graph& orderedGraph, int *clique_num);
void create_8cliqueList (com_node *root, Graph& orderedGraph, int *clique_num);
void create_8cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered8Cs, vector<v8c>& r8cs, vector<vertex>& xr8cs, Graph& s9cs);
void create_9cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered9Cs, vector<v9c>& r9cs, vector<vertex>& xr9cs, Graph& s10cs);

#endif //ND_CLIQUE_UTIL_H
