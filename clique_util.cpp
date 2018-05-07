//
// Created by yifan on 4/22/18.
//
#include "main.h"
#include "clique_util.h"

using namespace std;


void incrementCN(com_node *root, vector<vertex>& clique, Graph& ordered4Cs, Graph& s4cs) {

    com_node *next;
    int rid = -1;
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
        for(size_t i = 0;i < root->nodes.size();i++) {
            if(root->nodes[i].val == clique[root->order]) {
                next = &root->nodes[i];
                incrementCN(next, clique, ordered4Cs, s4cs);
                break;
            }
        }
    }

    else {
        rid = root->id;
        for(vertex j = 0;j < ordered4Cs[rid].size();j++) {
            if(ordered4Cs[rid][j] == clique[root->order]) {
                s4cs[rid][j]++;
                break;
            }
        }
    }
}

void increment_clique(com_node *root, initializer_list<vertex> elements, Graph& ordered4Cs, Graph& s4cs, vector<vertex>& clique, int index, size_t start, size_t end) {

    for(size_t i = start;i <= end;i++) {
        clique[index] = elements.begin()[i];
        if(end == clique.size()) {
            incrementCN(root, clique, ordered4Cs, s4cs);
        }
        else {
            increment_clique(root, elements, ordered4Cs, s4cs, clique, index+1, i+1, end+1);
        }
    }

}

vertex getId_recur(com_node *node, initializer_list<vertex> rc, size_t x) {
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

vertex getId(com_node *root, initializer_list<vertex> rc) {
    vertex u = rc.begin()[0];
    com_node *next = &root->nodes[u];
    return getId_recur(next, rc, 1);
}

void create_triangleList (com_node *root, Graph& orderedGraph) {
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

void create_triangleList (com_node *root, Graph& orderedGraph, int *triangle_num) {
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
    *triangle_num = counter;
}


void create_4cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered4Cs, vector<v4c>& r4cs, vector<vertex>& xr4cs, Graph& s5cs) {
    vertex counter = 0;
    xr4cs.push_back(0);
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
                    ordered4Cs[(*w).id].push_back (x);
                    s5cs[(*w).id].push_back (0);
                    v4c cl = make_tuple ((*u).val, (*v).val, (*w).val, (vertex)x);
                    cout << (*u).val << " " << (*v).val << " " << (*w).val << " " << x << endl;
                    r4cs.push_back (cl);
                    counter++;
                }
                //cout << "counter: " << counter << endl;
                xr4cs.push_back(counter);
                cout << "w id: " << (*w).id << " xr4cs size: " << xr4cs.size() << " xr4cs value: " << xr4cs.back() << " s5cs size: " << s5cs[(*w).id].size() << endl;
            }

        }
    }
}


void create_4cliqueList (com_node *root, Graph& orderedGraph) {
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

void create_4cliqueList (com_node *root, Graph& orderedGraph, int* clique_num) {
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
    *clique_num = counter;
}

void create_5cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered5Cs, vector<v5c>& r5cs, vector<vertex>& xr5cs, Graph& s6cs) {
    vertex counter = 0;
    xr5cs.push_back(0);
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
                        ordered5Cs[(*o).id].push_back (x);
                        s6cs[(*o).id].push_back (0);
                        v5c cl = make_tuple ((*u).val, (*v).val, (*w).val, (*o).val, (vertex)x);
                        cout << (*u).val << " " << (*v).val << " " << (*w).val << " " << (*o).val << " " << x << endl;
                        r5cs.push_back (cl);
                        counter++;
                    }
                    //cout << "counter: " << counter << endl;
                    xr5cs.push_back(counter);
                    cout << "w id: " << (*w).id << " xr4cs size: " << xr5cs.size() << " xr4cs value: " << xr5cs.back() << " s5cs size: " << s6cs[(*o).id].size() << endl;

                }
            }

        }
    }
}

void create_5cliqueList (com_node *root, Graph& orderedGraph) {
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

void create_5cliqueList (com_node *root, Graph& orderedGraph, int *clique_num) {
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
    *clique_num = counter;
}

void create_6cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered6Cs, vector<v6c>& r6cs, vector<vertex>& xr6cs, Graph& s7cs) {
    vertex counter = 0;
    xr6cs.push_back(0);
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
                            ordered6Cs[(*p).id].push_back (x);
                            s7cs[(*p).id].push_back (0);
                            v6c cl = make_tuple ((*u).val, (*v).val, (*w).val, (*o).val, (*p).val, (vertex)x);
                            cout << (*u).val << " " << (*v).val << " " << (*w).val << " " << (*o).val << " " << (*p).val << " " << x << endl;
                            r6cs.push_back (cl);
                            counter++;
                        }
                        xr6cs.push_back(counter);
                    }
                }
            }

        }
    }
}

void create_6cliqueList (com_node *root, Graph& orderedGraph) {
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

void create_6cliqueList (com_node *root, Graph& orderedGraph, int *clique_num) {
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
    *clique_num = counter;
}

void create_7cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered7Cs, vector<v7c>& r7cs, vector<vertex>& xr7cs, Graph& s8cs) {
    vertex counter = 0;
    xr7cs.push_back(0);
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
                                ordered7Cs[(*u6).id].push_back (x);
                                s8cs[(*u6).id].push_back (0);
                                v7c cl = make_tuple ((*u1).val, (*u2).val, (*u3).val, (*u4).val, (*u5).val, (*u6).val, (vertex)x);
                                r7cs.push_back (cl);
                                counter++;
                            }
                            xr7cs.push_back(counter);
                        }
                    }
                }
            }

        }
    }
}

void create_7cliqueList (com_node *root, Graph& orderedGraph) {
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

void create_7cliqueList (com_node *root, Graph& orderedGraph, int *clique_num) {
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
    *clique_num = counter;
}

void create_8cliqueList (com_node *root, Graph& orderedGraph, int *clique_num) {
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

void create_8cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered8Cs, vector<v8c>& r8cs, vector<vertex>& xr8cs, Graph& s9cs) {
    vertex counter = 0;
    xr8cs.push_back(0);
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
                                    ordered8Cs[(*u7).id].push_back (x);
                                    s9cs[(*u7).id].push_back (0);
                                    v8c cl = make_tuple ((*u1).val, (*u2).val, (*u3).val, (*u4).val, (*u5).val, (*u6).val, (*u7).val, (vertex)x);
                                    r8cs.push_back (cl);
                                    counter++;
                                }
                                xr8cs.push_back(counter);
                            }
                        }
                    }
                }
            }

        }
    }
}

void create_9cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered9Cs, vector<v9c>& r9cs, vector<vertex>& xr9cs, Graph& s10cs) {
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