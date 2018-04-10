//
// Created by yifan on 3/25/18.
//

#include <unordered_set>
#include "main.h"

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

inline void create_5cliqueList (com_node *root, Graph& orderedGraph, int *clique_num) {
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

inline void create_6cliqueList (com_node *root, Graph& orderedGraph, Graph& ordered6Cs, vector<v6c>& r6cs, vector<vertex>& xr6cs, Graph& s7cs) {
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

// per 4-clique
lol count7cliques (com_node *root, Graph& graph, Graph& orderedGraph, Graph& ordered6Cs, vector<v6c>& r6cs, Graph& s7cs) {
    //cout << "start count 5 cliques" << endl;
    lol fc = 0;
    for (auto t : r6cs) {
        vertex i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;
        vertex u = get<0>(t);
        vertex v = get<1>(t);
        vertex w = get<2>(t);
        vertex o = get<3>(t);
        vertex p = get<4>(t);
        vertex q = get<5>(t);
        while (i < orderedGraph[u].size() && j < orderedGraph[v].size() && k < orderedGraph[w].size()
               && l < orderedGraph[o].size() && m < orderedGraph[p].size() && n < orderedGraph[q].size()) {
            vertex a = orderedGraph[u][i];
            vertex b = orderedGraph[v][j];
            vertex c = orderedGraph[w][k];
            vertex d = orderedGraph[o][l];
            vertex e = orderedGraph[p][m];
            vertex f = orderedGraph[q][n];

            if (a == b && a == c && a == d && a == e && a == f) {
                vertex x = a;
                vector<vertex> clique(6);
                increment_clique(root, {u, v, w, o, p, q, x}, ordered6Cs, s7cs, clique, 0, 0, 1);
                cout << u << " " << v << " " << w << " " << o << " " << p << " " << q << " " << x << endl;
                i++; j++; k++; l++, m++, n++;
                fc++;
            }
            else {
                vertex mn = max ({a, b, c, d, e, f});
                if (a != mn)
                    i++;
                if (b != mn)
                    j++;
                if (c != mn)
                    k++;
                if (d != mn)
                    l++;
                if (e != mn)
                    m++;
                if (f != mn)
                    n++;
            }
        }
    }
    return fc;
}

void base_k67 (Graph& graph, bool hierarchy, edge nEdge, vector<vertex>& K, vertex* max45, string vfile, FILE* fp) {

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
    int clique_num = 0;
    create_5cliqueList (&root, orderedGraph, &clique_num);
    cout << "created 5-cliques" << endl;
    // Creating 6-clique list
    Graph ordered6Cs (clique_num); // like orderedGraph: each vector<vertex> is the list of rth vertices, ur, in the cliques of i-th r-1 cliques, u1 - u2 - u3... s.t. u1 < v2... < ur (deg ordering)
    vector<v6c> r6cs; // like el: list of the r-cliques aligned to the order in orderedCs
    vector<vertex> xr6cs; // like xel: indices in rcs that starts the r-clique list for an (r-1)-clique
    Graph s7cs (clique_num); // s-clique counts of each r-clique in the orderedCs structure
    create_6cliqueList (&root, orderedGraph, ordered6Cs, r6cs, xr6cs, s7cs);
    cout << "created 6-cliques" << endl;

    const auto t2 = chrono::steady_clock::now();

    print_time (fp, "Triangle enumeration: ", t2 - t1);

    const auto f1 = chrono::steady_clock::now();
    // 7-clique counting for each 6-clique
    lol fc = count7cliques (&root, graph, orderedGraph, ordered6Cs, r6cs, s7cs);
    cout << "fc: " << fc << endl;
    fprintf (fp, "# 6-cliques: %lld\n", fc);
    const auto f2 = chrono::steady_clock::now();
    print_time (fp, "6-clique counting: ", f2 - f1);

    // Peeling
    const auto p1 = chrono::steady_clock::now();
    K.resize (r6cs.size(), -1);
    Naive_Bucket nBucket;
    nBucket.Initialize (clique_num, r6cs.size()); // maximum 4-clique count of a triangle is nVtx

    vertex id = 0;
    for (size_t i = 0; i < s7cs.size(); i++)
        for (size_t j = 0; j < s7cs[i].size(); j++) {
            if (s7cs[i][j] > 0) {
                nBucket.Insert(xr6cs[i] + j, s7cs[i][j]);
                cout << i << " " << j << " " << xr6cs[i] + j<< " " << s7cs[i][j] << endl;
            }
            else
                K[xr6cs[i] + j] = 0;
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
        component.resize (r6cs.size(), -1);
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

        vertex u1 = get<0> (r6cs[t]);
        vertex u2 = get<1> (r6cs[t]);
        vertex u3 = get<2> (r6cs[t]);
        vertex u4 = get<3> (r6cs[t]);
        vertex u5 = get<4> (r6cs[t]);
        vertex u6 = get<5> (r6cs[t]);
        vector<vector<vertex>> vList;
        vList.push_back(orderedGraph[u1]);
        vList.push_back(orderedGraph[u2]);
        vList.push_back(orderedGraph[u3]);
        vList.push_back(orderedGraph[u4]);
        vList.push_back(orderedGraph[u5]);
        vList.push_back(orderedGraph[u6]);
        vector<vertex> commonNeighbors;
        //threeWay (graph[u], graph[v], graph[w], commonNeighbors);
        intersectionR(vList, commonNeighbors);
        for (auto x : commonNeighbors) { // decrease the FC of the neighbor triangles with greater FC
            vertex v1 = getId(&root, {u1, u2, u3, u4, u5, x});
            vertex v2 = getId(&root, {u1, u2, u3, u4, u6, x});
            vertex v3 = getId(&root, {u1, u2, u3, u5, u6, x});
            vertex v4 = getId(&root, {u1, u2, u4, u5, u6, x});
            vertex v5 = getId(&root, {u1, u3, u4, u5, u6, x});
            vertex v6 = getId(&root, {u2, u3, u4, u5, u6, x});
            cout << v1 << " " << v2 << " " << v3 << " " << v4 << " " << v5 << " " << v6 << endl;
            if ((v1 == -1 || K[v1] == -1) && (v2 == -1 || K[v2] == -1) &&
                    (v3 == -1 || K[v3] == -1) && (v4 == -1 || K[v4] == -1) &&
                    (v5 == -1 || K[v5] == -1) && (v6 == -1 || K[v6] == -1)) {
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
            }
            else if (hierarchy)
                createSkeleton (t, {v1, v2, v3, v4, v5, v6}, &nSubcores, K, skeleton, component, unassigned, relations);
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






