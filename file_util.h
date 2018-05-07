//
// Created by yifan on 4/21/18.
//

#ifndef ND_FILE_UTIL_H
#define ND_FILE_UTIL_H

void print_clique (FILE* fp, initializer_list<vertex> clique, int id);
void createOrderedGraph (Graph& graph, Graph& orderedGraph, string file_name);
void create_triangleList (Graph& orderedGraph, string file_name);
void create_triangleList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<vt, int>& rclique_map);
void create_4cliqueList (Graph& orderedGraph, string file_name);
void create_4cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v4c, int>& rclique_map);
void create_4cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v4c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v4c, int>& sclique_map);
void create_5cliqueList (Graph& orderedGraph, string file_name);
void create_5cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v5c, int>& rclique_map);
void create_5cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v5c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v5c, int>& sclique_map);
void create_6cliqueList (Graph& orderedGraph, string file_name);
void create_6cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v6c, int>& clique_map);
void create_6cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v6c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v6c, int>& sclique_map);
void create_7cliqueList (Graph& orderedGraph, string file_name);
void create_7cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v7c, int>& clique_map);
void create_7cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v7c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v7c, int>& sclique_map);
void create_8cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v8c, int>& clique8_map);
void create_8cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v8c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v8c, int>& sclique_map);
void create_9cliqueList (Graph& orderedGraph, Graph& ordered9Cs, vector<v9c>& r9cs, vector<vertex>& xr9cs, Graph& s10cs, string file_name, unordered_map<v9c, int>& clique9_map);
#endif //ND_FILE_UTIL_H
