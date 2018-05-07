//
// Created by yifan on 3/25/18.
//

#include <unordered_set>
#include "main.h"
#include "clique_util.h"

// per 4-clique
lol count8cliques (com_node *root, Graph& graph, Graph& orderedGraph, Graph& ordered7Cs, vector<v7c>& r7cs, Graph& s8cs) {
    //cout << "start count 5 cliques" << endl;
    lol fc = 0;
    for (auto t : r7cs) {
        vertex i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0;
        vertex u1 = get<0>(t);
        vertex u2 = get<1>(t);
        vertex u3 = get<2>(t);
        vertex u4 = get<3>(t);
        vertex u5 = get<4>(t);
        vertex u6 = get<5>(t);
        vertex u7 = get<6>(t);
        while (i1 < orderedGraph[u1].size() && i2 < orderedGraph[u2].size() && i3 < orderedGraph[u3].size()
               && i4 < orderedGraph[u4].size() && i5 < orderedGraph[u5].size() && i6 < orderedGraph[u6].size() &&
                i7 < orderedGraph[u7].size()) {
            vertex a1 = orderedGraph[u1][i1];
            vertex a2 = orderedGraph[u2][i2];
            vertex a3 = orderedGraph[u3][i3];
            vertex a4 = orderedGraph[u4][i4];
            vertex a5 = orderedGraph[u5][i5];
            vertex a6 = orderedGraph[u6][i6];
            vertex a7 = orderedGraph[u7][i7];

            if (a1 == a2 && a1 == a3 && a1 == a4 && a1 == a5 && a1 == a6 && a1 == a7) {
                vertex x = a1;
                vector<vertex> clique(7);
                increment_clique(root, {u1, u2, u3, u4, u5, u6, u7, x}, ordered7Cs, s8cs, clique, 0, 0, 1);
                //cout << u1 << " " << u2 << " " << u3 << " " << u4 << " " << u5 << " " << u6 << " " << " " << u7 << " " << x << endl;
                i1++; i2++; i3++; i4++, i5++, i6++, i7++;
                fc++;
            }
            else {
                vertex mn = max ({a1, a2, a3, a4, a5, a6, a7});
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
            }
        }
    }
    return fc;
}

void base_k78 (Graph& graph, bool hierarchy, edge nEdge, vector<vertex>& K, vertex* max45, string vfile, FILE* fp) {

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
    int clique_num = 0;
    create_6cliqueList (&root, orderedGraph, &clique_num);
    cout << "created 6-cliques" << endl;
    // Creating 6-clique list
    Graph ordered7Cs (clique_num); // like orderedGraph: each vector<vertex> is the list of rth vertices, ur, in the cliques of i-th r-1 cliques, u1 - u2 - u3... s.t. u1 < v2... < ur (deg ordering)
    vector<v7c> r7cs; // like el: list of the r-cliques aligned to the order in orderedCs
    vector<vertex> xr7cs; // like xel: indices in rcs that starts the r-clique list for an (r-1)-clique
    Graph s8cs (clique_num); // s-clique counts of each r-clique in the orderedCs structure
    create_7cliqueList (&root, orderedGraph, ordered7Cs, r7cs, xr7cs, s8cs);
    cout << "created 7-cliques" << endl;

    const auto t2 = chrono::steady_clock::now();

    print_time (fp, "Triangle enumeration: ", t2 - t1);

    const auto f1 = chrono::steady_clock::now();
    // 7-clique counting for each 6-clique
    lol fc = count8cliques (&root, graph, orderedGraph, ordered7Cs, r7cs, s8cs);
    cout << "fc: " << fc << endl;
    fprintf (fp, "# 6-cliques: %lld\n", fc);
    const auto f2 = chrono::steady_clock::now();
    print_time (fp, "6-clique counting: ", f2 - f1);

    // Peeling
    const auto p1 = chrono::steady_clock::now();
    K.resize (r7cs.size(), -1);
    Naive_Bucket nBucket;
    nBucket.Initialize (clique_num, r7cs.size()); // maximum 4-clique count of a triangle is nVtx

    vertex id = 0;
    for (size_t i = 0; i < s8cs.size(); i++)
        for (size_t j = 0; j < s8cs[i].size(); j++) {
            if (s8cs[i][j] > 0) {
                nBucket.Insert(xr7cs[i] + j, s8cs[i][j]);
                cout << i << " " << j << " " << xr7cs[i] + j<< " " << s8cs[i][j] << endl;
            }
            else
                K[xr7cs[i] + j] = 0;
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
        component.resize (r7cs.size(), -1);
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

        vertex u1 = get<0> (r7cs[t]);
        vertex u2 = get<1> (r7cs[t]);
        vertex u3 = get<2> (r7cs[t]);
        vertex u4 = get<3> (r7cs[t]);
        vertex u5 = get<4> (r7cs[t]);
        vertex u6 = get<5> (r7cs[t]);
        vertex u7 = get<6> (r7cs[t]);
        vector<vector<vertex>> vList;
        vList.push_back(orderedGraph[u1]);
        vList.push_back(orderedGraph[u2]);
        vList.push_back(orderedGraph[u3]);
        vList.push_back(orderedGraph[u4]);
        vList.push_back(orderedGraph[u5]);
        vList.push_back(orderedGraph[u6]);
        vList.push_back(orderedGraph[u7]);
        vector<vertex> commonNeighbors;
        //threeWay (graph[u], graph[v], graph[w], commonNeighbors);
        intersectionR(vList, commonNeighbors);
        for (auto x : commonNeighbors) { // decrease the FC of the neighbor triangles with greater FC
            vertex v1 = getId(&root, {u1, u2, u3, u4, u5, u6, x});
            vertex v2 = getId(&root, {u1, u2, u3, u4, u5, u7, x});
            vertex v3 = getId(&root, {u1, u2, u3, u4, u6, u7, x});
            vertex v4 = getId(&root, {u1, u2, u3, u5, u6, u7, x});
            vertex v5 = getId(&root, {u1, u2, u4, u5, u6, u7, x});
            vertex v6 = getId(&root, {u1, u3, u4, u5, u6, u7, x});
            vertex v7 = getId(&root, {u2, u3, u4, u5, u6, u7, x});
            if ((v1 == -1 || K[v1] == -1) && (v2 == -1 || K[v2] == -1) &&
                (v3 == -1 || K[v3] == -1) && (v4 == -1 || K[v4] == -1) &&
                (v5 == -1 || K[v5] == -1) && (v6 == -1 || K[v6] == -1) &&
                (v7 == -1 || K[v7] == -1)) {
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
            }
            else if (hierarchy)
                createSkeleton (t, {v1, v2, v3, v4, v5, v6, v7}, &nSubcores, K, skeleton, component, unassigned, relations);
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







