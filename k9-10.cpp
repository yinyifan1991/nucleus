//
// Created by yifan on 3/25/18.
//

#include <unordered_set>
#include "main.h"
/*
inline void incrementCN(com_node root, vector<vertex>& clique, Graph& ordered4Cs, Graph& s4cs) {
    com_node next;
    int rid = -1;
    //cout << "clique: " << clique[0] << " " << clique[1] << " " << clique[2] << " " << clique[3] << endl;
    for(com_node i: root.nodes[clique[0]].nodes) {
        if(i.val == clique[1]) {
            next = i;
            //cout << next.val << " ";
            break;
        }
    }
    for(com_node i: next.nodes) {
        if(i.val == clique[2]) {
            next = i;
            rid = i.id;
            //cout << next.val << " ";
            break;
        }
    }
    //cout << "rid: " << rid << " " << ordered4Cs[rid].size() << endl;
    for(vertex j = 0;j < ordered4Cs[rid].size();j++) {
        //cout << "rid: " << rid << "ordered4Cs: " << ordered4Cs[rid][j];
        if(ordered4Cs[rid][j] == clique[3]) {
            s4cs[rid][j]++;
            //cout << "increment " << rid << " " << j << endl;
            break;
        }
    }
    //cout << endl;
}

inline void increment_clique(com_node root, initializer_list<vertex> elements, Graph& ordered4Cs, Graph& s4cs) {
    vector<vertex> clique (4);
    for(size_t i1 = 0;i1 <= 1;i1++) {
        clique[0] = elements.begin()[i1];
        for(size_t i2 = i1+1;i2 <= 2;i2++) {
            clique[1] = elements.begin()[i2];
            for(size_t i3 = i2+1;i3 <= 3;i3++) {
                clique[2] = elements.begin()[i3];
                for(size_t i4 = i3+1;i4 <= 4;i4++) {
                    clique[3] = elements.begin()[i4];
                    incrementCN(root, clique, ordered4Cs, s4cs);
                }
            }
        }
    }
}

inline vertex getId(com_node root, initializer_list<vertex> rc) {
    vertex i = -1;
    vertex u = rc.begin()[0];
    com_node next = root.nodes[u];
    vertex v = rc.begin()[1];
    for(vertex j = 0; j < next.nodes.size();j++) {
        if(next.nodes[j].val == v) {
            next = next.nodes[j];
            i = next.id;
            break;
        }
    }
    if(i == -1) return i;
    vertex w = rc.begin()[2];
    for(vertex j = 0;j < next.nodes.size();j++) {
        if(next.nodes[j].val == w) {
            next = next.nodes[j];
            i = next.id;
            break;
        }
    }
    if(i == -1) return i;
    vertex o = rc.begin()[3];
    for(vertex j = 0;j < next.nodes.size();j++) {
        if (next.nodes[j].val == o) {
            next = next.nodes[j];
            i = next.id;
            break;
        }
    }
    return i;

}
*/
inline void incrementCN(com_node *root, vector<vertex>& clique, Graph& ordered4Cs, Graph& s4cs) {

    com_node *next;
    int rid = -1;
    //cout << "clique: " << clique[0] << " " << clique[1] << " " << clique[2] << " " << clique[3] << endl;
    if(root->order == 0) {
        com_node *u = &root->nodes[clique[0]];
        for(size_t i = 0;i < u->nodes.size();i++) {
            if(u->nodes[i].val == clique[1]) {
                next = &u->nodes[i];
                //cout << next.val << " ";
                incrementCN(next, clique, ordered4Cs, s4cs);
                break;
            }
        }
    }
    else if(root->order < clique.size() - 1) {
        cout << "root order: " << root->order << " root val: " << root->val << endl;
        for(size_t i = 0;i < root->nodes.size();i++) {
            if(root->nodes[i].val == clique[root->order]) {
                next = &root->nodes[i];
                //rid = next->id;
                //cout << next.val << " ";
                incrementCN(next, clique, ordered4Cs, s4cs);
                break;
            }
        }
    }

        //cout << "rid: " << rid << " " << ordered4Cs[rid].size() << endl;
    else {
        rid = root->id;
        cout << "root order: " << root->order << " " << " incrementCN rid: " << rid << endl;
        for(vertex j = 0;j < ordered4Cs[rid].size();j++) {
            //cout << "rid: " << rid << "ordered4Cs: " << ordered4Cs[rid][j];
            if(ordered4Cs[rid][j] == clique[root->order]) {
                s4cs[rid][j]++;
                cout << "increment " << rid << " " << j << endl;
                break;
            }
        }
    }
    //cout << endl;
}

inline void increment_clique(com_node *root, initializer_list<vertex> elements, Graph& ordered4Cs, Graph& s4cs, vector<vertex>& clique, int index, size_t start, size_t end) {

    for(size_t i = start;i <= end;i++) {
        clique[index] = elements.begin()[i];
        //cout << "end: " << end << " clique size: " << clique.size() << endl;
        if(end == clique.size()) {
            incrementCN(root, clique, ordered4Cs, s4cs);
        }
        else {
            increment_clique(root, elements, ordered4Cs, s4cs, clique, index+1, i+1, end+1);
        }
    }

}

inline vertex getId_recur(com_node *node, initializer_list<vertex> rc, size_t x) {
    vertex i = -1;
    vertex w = rc.begin()[x];
    com_node *next = node;
    for(vertex j = 0;j < node->nodes.size();j++) {
        if(node->nodes[j].val == w) {
            next = &node->nodes[j];
            i = next->id;
            break;
        }
    }
    return x == rc.size()-1 ? i : i == -1 ? i : getId_recur(next, rc, x+1);
}

inline vertex getId(com_node *root, initializer_list<vertex> rc) {
    vertex u = rc.begin()[0];
    com_node *next = &root->nodes[u];
    return getId_recur(next, rc, 1);
}

inline vector<vertex> find_common_recursion(vector<vector<vertex>> vList, vector<vertex>& commonNeighbors, int pos) {
    if(pos >= vList.size()) return commonNeighbors;
    vector<vertex> a = vList[pos];
    vector<vertex> commonTemp;
    size_t i = 0, j = 0;
    while (i < a.size() && j < commonNeighbors.size()) {
        if (a[i] < commonNeighbors[j])
            i++;
        else if (commonNeighbors[j] < a[i])
            j++;
        else {
            commonTemp.push_back(a[i]);
            i++;
            j++;
        }
    }
    return find_common_recursion(vList, commonTemp, pos+1);
}

inline void intersectionR (vector<vector<vertex>> vList, vector<vertex>& commonNeighbors) {
    vector<vertex> a = vList[0];
    vector<vertex> b = vList[1];
    vector<vertex> commonTemp;
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] < b[j])
            i++;
        else if (b[j] < a[i])
            j++;
        else {
            commonTemp.push_back(a[i]);
            i++;
            j++;
        }
    }
    commonNeighbors = find_common_recursion(vList, commonTemp, 2);
}

inline void create_triangleList (com_node *root, Graph& orderedGraph) {
    int counter = 0;
    for(size_t i = 0;i < (*root).nodes.size();i++) {
        com_node *u = &(*root).nodes[i];
        for(size_t j = 0;j < (*u).nodes.size();j++) {
            com_node *v = &(*u).nodes[j];
            vector<vector<vertex>> vList;
            vList.push_back(orderedGraph[(*u).val]);
            vList.push_back(orderedGraph[(*v).val]);
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);
            for(auto w: commonNeighbors) {
                (*v).nodes.push_back(com_node(w, 3, counter));
                counter++;
                //cout << (*u).val << " " << (*v).val << " " << w << endl;
            }
        }
    }
}


inline void create_4cliqueList (com_node *root, Graph& orderedGraph) {
    vertex counter = 0;
    for(size_t i = 0;i < (*root).nodes.size();i++) {
        com_node *u = &(*root).nodes[i];
        for(size_t j = 0;j < (*u).nodes.size();j++) {
            com_node *v = &(*u).nodes[j];
            for(size_t k = 0;k < (*v).nodes.size();k++) {
                com_node *w = &(*v).nodes[k];
                vector<vector<vertex>> vList;
                vList.push_back(orderedGraph[(*u).val]);
                vList.push_back(orderedGraph[(*v).val]);
                vList.push_back(orderedGraph[(*w).val]);
                vector<vertex> commonNeighbors;
                intersectionR(vList, commonNeighbors);
                for (auto x : commonNeighbors) {
                    (*w).nodes.push_back(com_node(x, 4, counter));
                    counter++;
                }
            }
        }
    }
}

inline void create_5cliqueList (com_node *root, Graph& orderedGraph) {
    vertex counter = 0;
    for(size_t i = 0;i < (*root).nodes.size();i++) {
        com_node *u = &(*root).nodes[i];
        for(size_t j = 0;j < (*u).nodes.size();j++) {
            com_node *v = &(*u).nodes[j];
            for(size_t k = 0;k < (*v).nodes.size();k++) {
                com_node *w = &(*v).nodes[k];
                for(size_t m = 0;m < (*w).nodes.size();m++) {
                    com_node *o = &(*w).nodes[m];
                    vector<vector<vertex>> vList;
                    vList.push_back(orderedGraph[(*u).val]);
                    vList.push_back(orderedGraph[(*v).val]);
                    vList.push_back(orderedGraph[(*w).val]);
                    vList.push_back(orderedGraph[(*o).val]);
                    vector<vertex> commonNeighbors;
                    intersectionR(vList, commonNeighbors);

                    for (auto x : commonNeighbors) {
                        (*o).nodes.push_back(com_node(x, 5, counter));
                        counter++;
                    }
                }
            }

        }
    }
}

inline void create_6cliqueList (com_node *root, Graph& orderedGraph) {
    vertex counter = 0;
    for(size_t i1 = 0;i1 < (*root).nodes.size();i1++) {
        com_node *u = &(*root).nodes[i1];
        for(size_t i2 = 0;i2 < (*u).nodes.size();i2++) {
            com_node *v = &(*u).nodes[i2];
            for(size_t i3 = 0;i3 < (*v).nodes.size();i3++) {
                com_node *w = &(*v).nodes[i3];
                for(size_t i4 = 0;i4 < (*w).nodes.size();i4++) {
                    com_node *o = &(*w).nodes[i4];
                    for(size_t i5 = 0;i5 < (*o).nodes.size();i5++) {
                        com_node *p = &(*o).nodes[i5];
                        vector<vector<vertex>> vList;
                        vList.push_back(orderedGraph[(*u).val]);
                        vList.push_back(orderedGraph[(*v).val]);
                        vList.push_back(orderedGraph[(*w).val]);
                        vList.push_back(orderedGraph[(*o).val]);
                        vList.push_back(orderedGraph[(*p).val]);
                        vector<vertex> commonNeighbors;
                        intersectionR(vList, commonNeighbors);

                        for (auto x : commonNeighbors) {
                            (*p).nodes.push_back(com_node(x, 6, counter));
                            counter++;
                        }
                    }
                }
            }

        }
    }
}

inline void create_7cliqueList (com_node *root, Graph& orderedGraph) {
    vertex counter = 0;
    for(size_t i1 = 0;i1 < (*root).nodes.size();i1++) {
        com_node *u1 = &(*root).nodes[i1];
        for(size_t i2 = 0;i2 < (*u1).nodes.size();i2++) {
            com_node *u2 = &(*u1).nodes[i2];
            for(size_t i3 = 0;i3 < (*u2).nodes.size();i3++) {
                com_node *u3 = &(*u2).nodes[i3];
                for(size_t i4 = 0;i4 < (*u3).nodes.size();i4++) {
                    com_node *u4 = &(*u3).nodes[i4];
                    for(size_t i5 = 0;i5 < (*u4).nodes.size();i5++) {
                        com_node *u5 = &(*u4).nodes[i5];
                        for(size_t i6 = 0;i6 < (*u5).nodes.size();i6++) {
                            com_node *u6 = &(*u5).nodes[i6];
                            vector<vector<vertex>> vList;
                            vList.push_back(orderedGraph[(*u1).val]);
                            vList.push_back(orderedGraph[(*u2).val]);
                            vList.push_back(orderedGraph[(*u3).val]);
                            vList.push_back(orderedGraph[(*u4).val]);
                            vList.push_back(orderedGraph[(*u5).val]);
                            vList.push_back(orderedGraph[(*u6).val]);
                            vector<vertex> commonNeighbors;
                            intersectionR(vList, commonNeighbors);

                            for (auto x : commonNeighbors) {
                                (*u6).nodes.push_back(com_node(x, 7, counter));
                                counter++;
                            }
                        }
                    }
                }
            }
        }
    }
}

inline void create_8cliqueList (com_node *root, Graph& orderedGraph, int *clique_num) {
    vertex counter = 0;
    for(size_t i1 = 0;i1 < (*root).nodes.size();i1++) {
        com_node *u1 = &(*root).nodes[i1];
        for(size_t i2 = 0;i2 < (*u1).nodes.size();i2++) {
            com_node *u2 = &(*u1).nodes[i2];
            for(size_t i3 = 0;i3 < (*u2).nodes.size();i3++) {
                com_node *u3 = &(*u2).nodes[i3];
                for(size_t i4 = 0;i4 < (*u3).nodes.size();i4++) {
                    com_node *u4 = &(*u3).nodes[i4];
                    for(size_t i5 = 0;i5 < (*u4).nodes.size();i5++) {
                        com_node *u5 = &(*u4).nodes[i5];
                        for(size_t i6 = 0;i6 < (*u5).nodes.size();i6++) {
                            com_node *u6 = &(*u5).nodes[i6];
                            for(size_t i7 = 0;i7 < (*u6).nodes.size();i7++) {
                                com_node *u7 = &(*u6).nodes[i7];
                                //cout << (*u1).val << " " << (*u2).val << " " << (*u3).val << " " << (*u4).val << " " << (*u5).val << " " << (*u6).val << " " << " " << (*u7).val << endl;
                                vector<vector<vertex>> vList;
                                vList.push_back(orderedGraph[(*u1).val]);
                                vList.push_back(orderedGraph[(*u2).val]);
                                vList.push_back(orderedGraph[(*u3).val]);
                                vList.push_back(orderedGraph[(*u4).val]);
                                vList.push_back(orderedGraph[(*u5).val]);
                                vList.push_back(orderedGraph[(*u6).val]);
                                vList.push_back(orderedGraph[(*u7).val]);
                                vector<vertex> commonNeighbors;
                                intersectionR(vList, commonNeighbors);

                                for (auto x : commonNeighbors) {
                                    (*u7).nodes.push_back(com_node(x, 8, counter));
                                    counter++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    *clique_num = counter;
}

inline void create_9cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered9Cs, vector<v9c>& r9cs, vector<vertex>& xr9cs, Graph& s10cs) {
    vertex counter = 0;
    xr9cs.push_back(0);
    for(size_t i1 = 0;i1 < (*root).nodes.size();i1++) {
        com_node *u1 = &(*root).nodes[i1];
        for(size_t i2 = 0;i2 < (*u1).nodes.size();i2++) {
            com_node *u2 = &(*u1).nodes[i2];
            for(size_t i3 = 0;i3 < (*u2).nodes.size();i3++) {
                com_node *u3 = &(*u2).nodes[i3];
                for(size_t i4 = 0;i4 < (*u3).nodes.size();i4++) {
                    com_node *u4 = &(*u3).nodes[i4];
                    for(size_t i5 = 0;i5 < (*u4).nodes.size();i5++) {
                        com_node *u5 = &(*u4).nodes[i5];
                        for(size_t i6 = 0;i6 < (*u5).nodes.size();i6++) {
                            com_node *u6 = &(*u5).nodes[i6];
                            for(size_t i7 = 0;i7 < (*u6).nodes.size();i7++) {
                                com_node *u7 = &(*u6).nodes[i7];
                                for(size_t i8 = 0;i8 < (*u7).nodes.size();i8++) {
                                    com_node *u8 = &(*u7).nodes[i8];
                                    //cout << (*u1).val << " " << (*u2).val << " " << (*u3).val << " " << (*u4).val << " " << (*u5).val << " " << (*u6).val << " " << " " << (*u7).val << endl;
                                    vector<vector<vertex>> vList;
                                    vList.push_back(orderedGraph[(*u1).val]);
                                    vList.push_back(orderedGraph[(*u2).val]);
                                    vList.push_back(orderedGraph[(*u3).val]);
                                    vList.push_back(orderedGraph[(*u4).val]);
                                    vList.push_back(orderedGraph[(*u5).val]);
                                    vList.push_back(orderedGraph[(*u6).val]);
                                    vList.push_back(orderedGraph[(*u7).val]);
                                    vList.push_back(orderedGraph[(*u8).val]);
                                    vector<vertex> commonNeighbors;
                                    intersectionR(vList, commonNeighbors);

                                    for (auto x : commonNeighbors) {
                                        (*u8).nodes.push_back(com_node(x, 9, counter));
                                        ordered9Cs[(*u8).id].push_back (x);
                                        s10cs[(*u8).id].push_back (0);
                                        v9c cl = make_tuple ((*u1).val, (*u2).val, (*u3).val, (*u4).val, (*u5).val, (*u6).val, (*u7).val, (*u8).val, (vertex)x);
                                        r9cs.push_back (cl);
                                        counter++;
                                    }
                                    xr9cs.push_back(counter);
                                }
                            }
                        }
                    }
                }
            }

        }
    }
}

// per 4-clique
inline lol count10cliques (com_node *root, Graph& graph, Graph& orderedGraph, Graph& ordered9Cs, vector<v9c>& r9cs, Graph& s10cs) {
    //cout << "start count 5 cliques" << endl;
    lol fc = 0;
    for (auto t : r9cs) {
        vertex i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0, i8 = 0, i9 = 0;
        vertex u1 = get<0>(t);
        vertex u2 = get<1>(t);
        vertex u3 = get<2>(t);
        vertex u4 = get<3>(t);
        vertex u5 = get<4>(t);
        vertex u6 = get<5>(t);
        vertex u7 = get<6>(t);
        vertex u8 = get<7>(t);
        vertex u9 = get<7>(t);
        while (i1 < orderedGraph[u1].size() && i2 < orderedGraph[u2].size() && i3 < orderedGraph[u3].size()
               && i4 < orderedGraph[u4].size() && i5 < orderedGraph[u5].size() && i6 < orderedGraph[u6].size() &&
               i7 < orderedGraph[u7].size() && i8 < orderedGraph[u8].size() && i9 < orderedGraph[u9].size()) {
            vertex a1 = orderedGraph[u1][i1];
            vertex a2 = orderedGraph[u2][i2];
            vertex a3 = orderedGraph[u3][i3];
            vertex a4 = orderedGraph[u4][i4];
            vertex a5 = orderedGraph[u5][i5];
            vertex a6 = orderedGraph[u6][i6];
            vertex a7 = orderedGraph[u7][i7];
            vertex a8 = orderedGraph[u8][i8];
            vertex a9 = orderedGraph[u9][i9];

            if (a1 == a2 && a1 == a3 && a1 == a4 && a1 == a5 && a1 == a6 && a1 == a7 && a1 == a8 && a1 == a9) {
                vertex x = a1;
                vector<vertex> clique(9);
                increment_clique(root, {u1, u2, u3, u4, u5, u6, u7, u8, u9, x}, ordered9Cs, s10cs, clique, 0, 0, 1);
                //cout << u1 << " " << u2 << " " << u3 << " " << u4 << " " << u5 << " " << u6 << " " << " " << u7 << " " << x << endl;
                i1++; i2++; i3++; i4++, i5++, i6++, i7++, i8++, i9++;
                fc++;
            }
            else {
                vertex mn = max ({a1, a2, a3, a4, a5, a6, a7, a8, a9});
                if (a1 != mn)
                    i1++;
                if (a2 != mn)
                    i2++;
                if (a3 != mn)
                    i3++;
                if (a4 != mn)
                    i4++;
                if (a5 != mn)
                    i5++;
                if (a6 != mn)
                    i6++;
                if (a7 != mn)
                    i7++;
                if (a8 != mn)
                    i8++;
                if (a9 != mn)
                    i9++;
            }
        }
    }
    return fc;
}

void base_k9n10 (Graph& graph, bool hierarchy, edge nEdge, vector<vertex>& K, vertex* max45, string vfile, FILE* fp) {

    const auto t1 = chrono::steady_clock::now();
    vertex nVtx = graph.size();

    // Create directed graph from low degree vertices to higher degree vertices AND prepare a CSR-like structure to index the edges
    Graph orderedGraph;
    com_node root(-1, 0, -1);
    //createOrderedIndexEdges (graph, el, xel, orderedGraph);
    createOrderedIndexEdgesNode(graph, &root, orderedGraph);
    cout << "created edges" << endl;
    // Creating triangle list
    create_triangleList (&root, orderedGraph);
    cout << "created triangles" << endl;
    // Creating 4-clique list
    create_4cliqueList (&root, orderedGraph);
    cout << "created 4-cliques" << endl;
    // Creating 5-clique list
    create_5cliqueList (&root, orderedGraph);
    cout << "created 5-cliques" << endl;
    // Creating 6-clique list
    create_6cliqueList (&root, orderedGraph);
    cout << "created 6-cliques" << endl;
    // Creating 7-clique list
    create_7cliqueList (&root, orderedGraph);
    cout << "created 7-cliques" << endl;
    // Creating 8-clique list
    int clique_num = 0;
    create_8cliqueList (&root, orderedGraph, &clique_num);
    cout << "created 8-cliques" << endl;
    // Creating 9-clique list
    Graph ordered9Cs (clique_num); // like orderedGraph: each vector<vertex> is the list of rth vertices, ur, in the cliques of i-th r-1 cliques, u1 - u2 - u3... s.t. u1 < v2... < ur (deg ordering)
    vector<v9c> r9cs; // like el: list of the r-cliques aligned to the order in orderedCs
    vector<vertex> xr9cs; // like xel: indices in rcs that starts the r-clique list for an (r-1)-clique
    Graph s10cs (clique_num); // s-clique counts of each r-clique in the orderedCs structure
    create_9cliqueList (&root, orderedGraph, ordered9Cs, r9cs, xr9cs, s10cs);
    cout << "created 9-cliques" << endl;

    const auto t2 = chrono::steady_clock::now();

    print_time (fp, "Triangle enumeration: ", t2 - t1);

    const auto f1 = chrono::steady_clock::now();
    // 7-clique counting for each 6-clique
    lol fc = count10cliques (&root, graph, orderedGraph, ordered9Cs, r9cs, s10cs);
    cout << "fc: " << fc << endl;
    fprintf (fp, "# 6-cliques: %lld\n", fc);
    const auto f2 = chrono::steady_clock::now();
    print_time (fp, "6-clique counting: ", f2 - f1);

    // Peeling
    const auto p1 = chrono::steady_clock::now();
    K.resize (r9cs.size(), -1);
    Naive_Bucket nBucket;
    nBucket.Initialize (clique_num, r9cs.size()); // maximum 4-clique count of a triangle is nVtx

    vertex id = 0;
    for (size_t i = 0; i < s10cs.size(); i++)
        for (size_t j = 0; j < s10cs[i].size(); j++) {
            if (s10cs[i][j] > 0) {
                nBucket.Insert(xr9cs[i] + j, s10cs[i][j]);
                cout << i << " " << j << " " << xr9cs[i] + j<< " " << s10cs[i][j] << endl;
            }
            else
                K[xr9cs[i] + j] = 0;
            id++;
        }
    cout << "id: " << id << endl;
    //return;
    vertex fc_t = 0;

    // required for hierarchy
    vertex cid; // subcore id number
    vector<subcore> skeleton; // equal K valued cores
    vector<vertex> component; // subcore ids for each vertex
    vector<vp> relations;
    vector<vertex> unassigned;
    vertex nSubcores;
    cout << "prepared to process" << endl;
    if (hierarchy) {
        cid = 0;
        nSubcores = 0;
        component.resize (r9cs.size(), -1);
    }

    vertex monitor = 0;
    while (true) {
        edge t;
        edge val ;
        cout << "prepare to popmin" << endl;
        if (nBucket.PopMin(&t, &val)) // if the bucket is empty
            break;
        cout << t << " " << val << endl;
#ifdef MONITOR
        if (monitor % 10000 == 0)
			printf ("t: %d    val: %d    counter: %d  nEdge: %d\n", t, val, monitor, nEdge);
		monitor++;
#endif

        if (hierarchy) {
            unassigned.clear();
            subcore sc (val);
            skeleton.push_back (sc);
        }

        fc_t = K[t] = val;

        vertex u1 = get<0> (r9cs[t]);
        vertex u2 = get<1> (r9cs[t]);
        vertex u3 = get<2> (r9cs[t]);
        vertex u4 = get<3> (r9cs[t]);
        vertex u5 = get<4> (r9cs[t]);
        vertex u6 = get<5> (r9cs[t]);
        vertex u7 = get<6> (r9cs[t]);
        vertex u8 = get<7> (r9cs[t]);
        vertex u9 = get<8> (r9cs[t]);
        vector<vector<vertex>> vList;
        vList.push_back(orderedGraph[u1]);
        vList.push_back(orderedGraph[u2]);
        vList.push_back(orderedGraph[u3]);
        vList.push_back(orderedGraph[u4]);
        vList.push_back(orderedGraph[u5]);
        vList.push_back(orderedGraph[u6]);
        vList.push_back(orderedGraph[u7]);
        vList.push_back(orderedGraph[u8]);
        vList.push_back(orderedGraph[u9]);
        vector<vertex> commonNeighbors;
        //threeWay (graph[u], graph[v], graph[w], commonNeighbors);
        intersectionR(vList, commonNeighbors);
        for (auto x : commonNeighbors) { // decrease the FC of the neighbor triangles with greater FC
            vertex v1 = getId(&root, {u1, u2, u3, u4, u5, u6, u7, u8, x});
            vertex v2 = getId(&root, {u1, u2, u3, u4, u5, u6, u7, u9, x});
            vertex v3 = getId(&root, {u1, u2, u3, u4, u5, u6, u8, u9, x});
            vertex v4 = getId(&root, {u1, u2, u3, u4, u5, u7, u8, u9, x});
            vertex v5 = getId(&root, {u1, u2, u3, u4, u6, u7, u8, u9, x});
            vertex v6 = getId(&root, {u1, u2, u3, u5, u6, u7, u8, u9, x});
            vertex v7 = getId(&root, {u1, u2, u4, u5, u6, u7, u8, u9, x});
            vertex v8 = getId(&root, {u1, u3, u4, u5, u6, u7, u8, u9, x});
            vertex v9 = getId(&root, {u2, u3, u4, u5, u6, u7, u8, u9, x});
            if ((v1 == -1 || K[v1] == -1) && (v2 == -1 || K[v2] == -1) &&
                    (v3 == -1 || K[v3] == -1) && (v4 == -1 || K[v4] == -1) &&
                    (v5 == -1 || K[v5] == -1) && (v6 == -1 || K[v6] == -1) &&
                    (v7 == -1 || K[v7] == -1) && (v8 == -1 || K[v8] == -1) &&
                    (v9 == -1 || K[v9] == -1)) {
                if (v1 != -1 && nBucket.CurrentValue(v1) > fc_t)
                    nBucket.DecVal(v1);
                if (v2 != -1 && nBucket.CurrentValue(v2) > fc_t)
                    nBucket.DecVal(v2);
                if (v3 != -1 && nBucket.CurrentValue(v3) > fc_t)
                    nBucket.DecVal(v3);
                if (v4 != -1 && nBucket.CurrentValue(v4) > fc_t)
                    nBucket.DecVal(v4);
                if (v5 != -1 && nBucket.CurrentValue(v5) > fc_t)
                    nBucket.DecVal(v5);
                if (v6 != -1 && nBucket.CurrentValue(v6) > fc_t)
                    nBucket.DecVal(v6);
                if (v7 != -1 && nBucket.CurrentValue(v7) > fc_t)
                    nBucket.DecVal(v7);
                if (v8 != -1 && nBucket.CurrentValue(v8) > fc_t)
                    nBucket.DecVal(v8);
                if (v9 != -1 && nBucket.CurrentValue(v9) > fc_t)
                    nBucket.DecVal(v9);
            }
            else if (hierarchy)
                createSkeleton (t, {v1, v2, v3, v4, v5, v6, v7, v8, v9}, &nSubcores, K, skeleton, component, unassigned, relations);
        }
        if (hierarchy)
            updateUnassigned (t, component, &cid, relations, unassigned);
    }
    cout << "out loop" << endl;
    cout << "fc_t: " << fc_t << endl;
    nBucket.Free();
    *max45 = fc_t; // fc_t is fc of the last popped triangle

    const auto p2 = chrono::steady_clock::now();

    if (!hierarchy) {
        print_time (fp, "Only peeling time: ", p2 - p1);
        print_time (fp, "Total time: ", (p2 - p1) + (f2 - f1) + (t2 - t1));
    }
    else {
        print_time (fp, "Only peeling + on-the-fly hierarchy construction time: ", p2 - p1);
        const auto b1 = chrono::steady_clock::now();
        buildHierarchy (*max45, relations, skeleton, &nSubcores, nEdge, nVtx);
        const auto b2 = chrono::steady_clock::now();

        print_time (fp, "Building hierarchy time: ", b2 - b1);
        print_time (fp, "Total 3,4 nucleus decomposition time (excluding density computation): ", (p2 - p1) + (f2 - f1) + (t2 - t1) + (b2 - b1));

        fprintf (fp, "# subcores: %d\t\t # subsubcores: %d\t\t |V|: %d\n", nSubcores, skeleton.size(), graph.size());

        const auto d1 = chrono::steady_clock::now();
        //helpers hp (&tris);
        //presentNuclei (34, skeleton, component, graph, nEdge, hp, vfile, fp);
        const auto d2 = chrono::steady_clock::now();

        print_time (fp, "Total 3,4 nucleus decomposition time: ", (p2 - p1) + (f2 - f1) + (t2 - t1) + (b2 - b1) + (d2 - d1));
    }
}







