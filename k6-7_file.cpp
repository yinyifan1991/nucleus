//
// Created by yifan on 5/6/18.
//

#include <unordered_set>
#include "main.h"
#include "file_util.h"

#define MAXLINE 1000000

unordered_map<v5c, int> clique5r_map; // store ids for (r-2)-clique
unordered_map<v6c, int> clique6s_map; // store ids for (r-1)-clique

void incrementCN(v5c cl, vertex u, Graph& orderedCs, Graph& scs) {
    if(clique5r_map.find(cl) != clique5r_map.end()) {
        int rid = clique5r_map[cl];
        for(vertex j = 0;j < orderedCs[rid].size();j++) {
            if(orderedCs[rid][j] == u) {
                scs[rid][j]++;
                break;
            }
        }
    }
}
// increment the count of s-cliques belonging to each r-clique
void increment_7clique(initializer_list<vertex> elements, Graph& orderedCs, Graph& scs) {
    for(size_t i1 = 0;i1 <= 1;i1++) {
        vertex u1 = elements.begin()[i1];
        for(size_t i2 = i1+1;i2 <= 2;i2++) {
            vertex u2 = elements.begin()[i2];
            for(size_t i3 = i2+1;i3 <= 3;i3++) {
                vertex u3 = elements.begin()[i3];
                for(size_t i4 = i3+1;i4 <= 4;i4++) {
                    vertex u4 = elements.begin()[i4];
                    for(size_t i5 = i4+1;i5 <= 5;i5++) {
                        vertex u5 = elements.begin()[i5];
                        v5c cl = make_tuple(u1, u2, u3, u4, u5);
                        for(size_t i6 = i5+1;i6 <= 6;i6++) {
                            vertex u6 = elements.begin()[i6];
                            incrementCN(cl, u6, orderedCs, scs);
                        }
                    }
                }
            }
        }
    }
}
// obtain the id of a particular r-clique
vertex getId(v6c cl) {
    if(clique6s_map.find(cl) != clique6s_map.end()) return clique6s_map[cl];
    return -1;
}



// per 7-clique
lol count_cliques (Graph& graph, Graph& orderedGraph, Graph& orderedCs, vector<v6c>& rcs, Graph& scs) {
    lol fc = 0;
    for (auto t : rcs) {
        vertex i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0;
        vertex u1 = get<0>(t);
        vertex u2 = get<1>(t);
        vertex u3 = get<2>(t);
        vertex u4 = get<3>(t);
        vertex u5 = get<4>(t);
        vertex u6 = get<5>(t);
        while (i1 < orderedGraph[u1].size() && i2 < orderedGraph[u2].size() && i3 < orderedGraph[u3].size()
               && i4 < orderedGraph[u4].size() && i5 < orderedGraph[u5].size() && i6 < orderedGraph[u6].size()) {
            vertex a1 = orderedGraph[u1][i1];
            vertex a2 = orderedGraph[u2][i2];
            vertex a3 = orderedGraph[u3][i3];
            vertex a4 = orderedGraph[u4][i4];
            vertex a5 = orderedGraph[u5][i5];
            vertex a6 = orderedGraph[u6][i6];

            if (a1 == a2 && a1 == a3 && a1 == a4 && a1 == a5 && a1 == a6) {
                vertex x = a1;
                increment_7clique({u1, u2, u3, u4, u5, u6, x}, orderedCs, scs);
                i1++; i2++; i3++; i4++, i5++, i6++;
                fc++;
            }
            else {
                vertex mn = max ({a1, a2, a3, a4, a5, a6});
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
            }
        }
    }
    return fc;
}

void base_k67_file (Graph& graph, bool hierarchy, edge nEdge, vector<vertex>& K, vertex* max45, string vfile, FILE* fp, string file_name) {

    const auto t1 = chrono::steady_clock::now();
    vertex nVtx = graph.size();

    // Create directed graph from low degree vertices to higher degree vertices AND prepare a CSR-like structure to index the edges
    Graph orderedGraph;
    //createOrderedIndexEdges (graph, el, xel, orderedGraph);
    createOrderedGraph (graph, orderedGraph, file_name);
    cout << "created edges" << endl;
    // Creating triangle list
    create_triangleList (orderedGraph,file_name);
    cout << "created triangles" << endl;
    // Creating 4-clique list
    create_4cliqueList (orderedGraph, file_name);
    cout << "created 4-cliques" << endl;
    // Creating 5-clique list
    int clique_num = 0;
    create_5cliqueList (orderedGraph, &clique_num, file_name, clique5r_map);
    cout << "created 5-cliques" << endl;
    // Creating 6-clique list
    Graph orderedCs (clique_num); // like orderedGraph: each vector<vertex> is the list of rth vertices, ur, in the cliques of i-th r-1 cliques, u1 - u2 - u3... s.t. u1 < v2... < ur (deg ordering)
    vector<v6c> rcs; // like el: list of the r-cliques aligned to the order in orderedCs
    vector<vertex> xrcs; // like xel: indices in rcs that starts the r-clique list for an (r-1)-clique
    Graph scs (clique_num); // s-clique counts of each r-clique in the orderedCs structure
    create_6cliqueList (orderedGraph, orderedCs, rcs, xrcs, scs, file_name, clique6s_map);
    cout << "created 6-cliques" << endl;

    const auto t2 = chrono::steady_clock::now();

    print_time (fp, "Triangle enumeration: ", t2 - t1);

    const auto f1 = chrono::steady_clock::now();
    // s-clique counting for each r-clique
    lol fc = count_cliques (graph, orderedGraph, orderedCs, rcs, scs);
    fprintf (fp, "# 7-cliques: %lld\n", fc);
    const auto f2 = chrono::steady_clock::now();
    print_time (fp, "7-clique counting: ", f2 - f1);

    // Peeling
    const auto p1 = chrono::steady_clock::now();
    K.resize (rcs.size(), -1);
    Naive_Bucket nBucket;
    nBucket.Initialize (clique_num, rcs.size()); // maximum 4-clique count of a triangle is nVtx

    vertex id = 0;
    for (size_t i = 0; i < scs.size(); i++)
        for (size_t j = 0; j < scs[i].size(); j++) {
            if (scs[i][j] > 0) {
                nBucket.Insert(xrcs[i] + j, scs[i][j]);
            }
            else
                K[xrcs[i] + j] = 0;
            id++;
        }
    vertex fc_t = 0;
    // required for hierarchy
    vertex cid; // subcore id number
    vector<subcore> skeleton; // equal K valued cores
    vector<vertex> component; // subcore ids for each vertex
    vector<vp> relations;
    vector<vertex> unassigned;
    vertex nSubcores;
    if (hierarchy) {
        cid = 0;
        nSubcores = 0;
        component.resize (rcs.size(), -1);
    }

    vertex monitor = 0;
    while (true) {
        edge t;
        edge val ;
        if (nBucket.PopMin(&t, &val)) // if the bucket is empty
            break;
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

        vertex u1 = get<0> (rcs[t]);
        vertex u2 = get<1> (rcs[t]);
        vertex u3 = get<2> (rcs[t]);
        vertex u4 = get<3> (rcs[t]);
        vertex u5 = get<4> (rcs[t]);
        vertex u6 = get<5> (rcs[t]);
        vector<vector<vertex>> vList;
        vList.push_back(orderedGraph[u1]);
        vList.push_back(orderedGraph[u2]);
        vList.push_back(orderedGraph[u3]);
        vList.push_back(orderedGraph[u4]);
        vList.push_back(orderedGraph[u5]);
        vList.push_back(orderedGraph[u6]);
        vector<vertex> commonNeighbors;
        intersectionR(vList, commonNeighbors);
        for (auto x : commonNeighbors) { // decrease the FC of the neighbor triangles with greater FC
            vertex v1 = getId(make_tuple(u1, u2, u3, u4, u5, x));
            vertex v2 = getId(make_tuple(u1, u2, u3, u4, u6, x));
            vertex v3 = getId(make_tuple(u1, u2, u3, u5, u6, x));
            vertex v4 = getId(make_tuple(u1, u2, u4, u5, u6, x));
            vertex v5 = getId(make_tuple(u1, u3, u4, u5, u6, x));
            vertex v6 = getId(make_tuple(u2, u3, u4, u5, u6, x));
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
        print_time (fp, "Total 6,5 nucleus decomposition time (excluding density computation): ", (p2 - p1) + (f2 - f1) + (t2 - t1) + (b2 - b1));

        fprintf (fp, "# subcores: %d\t\t # subsubcores: %d\t\t |V|: %d\n", nSubcores, skeleton.size(), graph.size());

        const auto d1 = chrono::steady_clock::now();
        //helpers hp (&tris);
        //presentNuclei (34, skeleton, component, graph, nEdge, hp, vfile, fp);
        const auto d2 = chrono::steady_clock::now();

        print_time (fp, "Total 6,7 nucleus decomposition time: ", (p2 - p1) + (f2 - f1) + (t2 - t1) + (b2 - b1) + (d2 - d1));
    }
}









