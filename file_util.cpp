//
// Created by yifan on 4/21/18.
//

#include "main.h"

using namespace std;

#define MAXLINE 1000000

void print_clique (FILE* fp, initializer_list<vertex> clique, int id) {
    for(size_t i = 0;i < clique.size();i++) {
        fprintf (fp, "%d ", clique.begin()[i]);
    }
    fprintf (fp, "%d\n", id);
    fflush(fp);
}

void createOrderedGraph (Graph& graph, Graph& orderedGraph, string file_name) {
    string out_name = file_name + "_2clique";
    FILE* fp = fopen(out_name.c_str(), "w");
    orderedGraph.resize(graph.size());
    int count = 0;
    for (size_t u = 0; u < graph.size(); u++) {
        for (size_t j = 0; j < graph[u].size(); j++) {
            vertex v = graph[u][j];
            if (less_than (u, v, graph)) {
                orderedGraph[u].push_back(v);
                print_clique(fp, {u, v}, count);
                count++;
            }
        }
    }
    fclose(fp);
}

void create_triangleList (Graph& orderedGraph, string file_name) {
    if(FILE *fp_pre = fopen((file_name + "_3clique").c_str(), "r")) {
        fclose(fp_pre);
        return;
    }
    if(FILE *fp_pre = fopen((file_name + "_2clique").c_str(), "r")) {
        string out_name = file_name + "_3clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, w}, counter);
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_triangleList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<vt, int>& rclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_2clique").c_str(), "r")) {
        string out_name = file_name + "_3clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, w}, counter);
                vt cl = make_tuple(u1, u2, w);
                rclique_map[cl] = counter;
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
        *clique_num = counter;
    }
}


void create_4cliqueList (Graph& orderedGraph, string file_name) {
    if(FILE *fp_pre = fopen((file_name + "_4clique").c_str(), "r")) {
        fclose(fp_pre);
        return;
    }
    if(FILE *fp_pre = fopen((file_name + "_3clique").c_str(), "r")) {
        string out_name = file_name + "_4clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, w}, counter);
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_4cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v4c, int>& rclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_3clique").c_str(), "r")) {
        string out_name = file_name + "_4clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, w}, counter);
                v4c cl = make_tuple(u1, u2, u3, w);
                rclique_map[cl] = counter;
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
        *clique_num = counter;
    }
}

void create_4cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v4c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v4c, int>& sclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_3clique").c_str(), "r")) {
        string out_name = file_name + "_4clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        xrcs.push_back(0);
        int counter = 0;
        vertex u1, u2, u3, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, w}, counter);
                v4c cl = make_tuple(u1, u2, u3, w);
                orderedCs[id].push_back (w);
                scs[id].push_back (0);
                rcs.push_back (cl);
                sclique_map[cl] = counter;
                counter++;
            }
            xrcs.push_back(counter);
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_5cliqueList (Graph& orderedGraph, string file_name) {
    if(FILE *fp_pre = fopen((file_name + "_5clique").c_str(), "r")) {
        fclose(fp_pre);
        return;
    }
    if(FILE *fp_pre = fopen((file_name + "_4clique").c_str(), "r")) {
        string out_name = file_name + "_5clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, u4, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, w}, counter);
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_5cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v5c, int>& rclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_4clique").c_str(), "r")) {
        string out_name = file_name + "_5clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, u4, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, w}, counter);
                v5c cl = make_tuple(u1, u2, u3, u4, w);
                rclique_map[cl] = counter;
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
        *clique_num = counter;
    }
}

void create_5cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v5c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v5c, int>& sclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_4clique").c_str(), "r")) {
        string out_name = file_name + "_5clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        xrcs.push_back(0);
        int counter = 0;
        vertex u1, u2, u3, u4, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, w}, counter);
                v5c cl = make_tuple(u1, u2, u3, u4, w);
                orderedCs[id].push_back (w);
                scs[id].push_back (0);
                rcs.push_back (cl);
                sclique_map[cl] = counter;
                counter++;
            }
            xrcs.push_back(counter);
        }
        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_6cliqueList (Graph& orderedGraph, string file_name) {
    if(FILE *fp_pre = fopen((file_name + "_6clique").c_str(), "r")) {
        fclose(fp_pre);
        return;
    }
    if(FILE *fp_pre = fopen((file_name + "_5clique").c_str(), "r")) {
        string out_name = file_name + "_6clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, u4, u5, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, w}, counter);
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_6cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v6c, int>& rclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_5clique").c_str(), "r")) {
        string out_name = file_name + "_6clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, u4, u5, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, w}, counter);
                v6c cl = make_tuple(u1, u2, u3, u4, u5, w);
                rclique_map[cl] = counter;
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
        *clique_num = counter;
    }
}

void create_6cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v6c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v6c, int>& sclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_5clique").c_str(), "r")) {
        string out_name = file_name + "_6clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        xrcs.push_back(0);
        int counter = 0;
        vertex u1, u2, u3, u4, u5, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, w}, counter);
                v6c cl = make_tuple(u1, u2, u3, u4, u5, w);
                orderedCs[id].push_back (w);
                scs[id].push_back (0);
                rcs.push_back (cl);
                sclique_map[cl] = counter;
                counter++;
            }
            xrcs.push_back(counter);
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_7cliqueList (Graph& orderedGraph, string file_name) {
    if(FILE *fp_pre = fopen((file_name + "_7clique").c_str(), "r")) {
        fclose(fp_pre);
        return;
    }
    if(FILE *fp_pre = fopen((file_name + "_6clique").c_str(), "r")) {
        string out_name = file_name + "_7clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, u4, u5, u6, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> u6 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vList.push_back(orderedGraph[u6]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, u6, w}, counter);
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_7cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v7c, int>& rclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_6clique").c_str(), "r")) {
        string out_name = file_name + "_7clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, u4, u5, u6, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> u6 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vList.push_back(orderedGraph[u6]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, u6, w}, counter);
                v7c cl = make_tuple(u1, u2, u3, u4, u5, u6, w);
                rclique_map[cl] = counter;
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
        *clique_num = counter;
    }
}

void create_7cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v7c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v7c, int>& sclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_6clique").c_str(), "r")) {
        string out_name = file_name + "_7clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        xrcs.push_back(0);
        int counter = 0;
        vertex u1, u2, u3, u4, u5, u6, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> u6 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vList.push_back(orderedGraph[u6]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, u6, w}, counter);

                v7c cl = make_tuple(u1, u2, u3, u4, u5, u6, w);
                orderedCs[id].push_back (w);
                scs[id].push_back (0);
                rcs.push_back (cl);

                sclique_map[cl] = counter;
                counter++;
            }
            xrcs.push_back(counter);
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_8cliqueList (Graph& orderedGraph, int *clique_num, string file_name, unordered_map<v8c, int>& rclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_7clique").c_str(), "r")) {
        string out_name = file_name + "_8clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        int counter = 0;
        vertex u1, u2, u3, u4, u5, u6, u7, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);
        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> u6 >> u7 >> id;
            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vList.push_back(orderedGraph[u6]);
            vList.push_back(orderedGraph[u7]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);
            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, u6, u7, w}, counter);
                v8c cl = make_tuple(u1, u2, u3, u4, u5, u6, u7, w);
                rclique_map[cl] = counter;
                counter++;
            }
        }

        fclose(fp_cur);
        fclose(fp_pre);
        *clique_num = counter;
    }
}

void create_8cliqueList (Graph& orderedGraph, Graph& orderedCs, vector<v8c>& rcs, vector<vertex>& xrcs, Graph& scs, string file_name, unordered_map<v8c, int>& sclique_map) {
    if(FILE *fp_pre = fopen((file_name + "_7clique").c_str(), "r")) {
        string out_name = file_name + "_8clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        xrcs.push_back(0);
        int counter = 0;
        vertex u1, u2, u3, u4, u5, u6, u7, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> u6 >> u7 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vList.push_back(orderedGraph[u6]);
            vList.push_back(orderedGraph[u7]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, u6, u7, w}, counter);
                v8c cl = make_tuple(u1, u2, u3, u4, u5, u6, u7, w);
                orderedCs[id].push_back (w);
                scs[id].push_back (0);
                rcs.push_back (cl);

                sclique_map[cl] = counter;
                counter++;
            }
            xrcs.push_back(counter);
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}

void create_9cliqueList (Graph& orderedGraph, Graph& ordered9Cs, vector<v9c>& r9cs, vector<vertex>& xr9cs, Graph& s10cs, string file_name, unordered_map<v9c, int>& clique9_map) {
    if(FILE *fp_pre = fopen((file_name + "_8clique").c_str(), "r")) {
        string out_name = file_name + "_9clique";
        FILE* fp_cur = fopen(out_name.c_str(), "w");

        xr9cs.push_back(0);
        int counter = 0;
        vertex u1, u2, u3, u4, u5, u6, u7, u8, id;
        char* line = (char*) malloc (sizeof (char) * MAXLINE);

        while(fgets(line, MAXLINE, fp_pre)) {
            stringstream ss(line);
            ss >> u1 >> u2 >> u3 >> u4 >> u5 >> u6 >> u7 >> u8 >> id;

            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[u1]);
            vList.push_back(orderedGraph[u2]);
            vList.push_back(orderedGraph[u3]);
            vList.push_back(orderedGraph[u4]);
            vList.push_back(orderedGraph[u5]);
            vList.push_back(orderedGraph[u6]);
            vList.push_back(orderedGraph[u7]);
            vList.push_back(orderedGraph[u8]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for(auto w: commonNeighbors) {
                print_clique(fp_cur, {u1, u2, u3, u4, u5, u6, u7, u8, w}, counter);
                ordered9Cs[id].push_back (w);
                s10cs[id].push_back (0);
                v9c cl = make_tuple (u1, u2, u3, u4, u5, u6, u7, u8, w);
                r9cs.push_back (cl);

                clique9_map[cl] = counter;
                counter++;
            }
            xr9cs.push_back(counter);
        }

        fclose(fp_cur);
        fclose(fp_pre);
    }
}