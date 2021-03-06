//
// Created by yifan on 3/25/18.
//

#include <unordered_set>
#include "main.h"
#include "clique_util.h"

// per 5-clique
lol count6cliques (com_node *root, Graph& graph, Graph& orderedGraph, Graph& ordered5Cs, vector<v5c>& r5cs, Graph& s6cs) {
    //cout << "start count 5 cliques" << endl;
    lol fc = 0;
    for (auto t : r5cs) {
        vertex i = 0, j = 0, k = 0, l = 0, m = 0;
        vertex u = get<0>(t);
        vertex v = get<1>(t);
        vertex w = get<2>(t);
        vertex o = get<3>(t);
        vertex p = get<4>(t);
        while (i < orderedGraph[u].size() && j < orderedGraph[v].size() && k < orderedGraph[w].size() && l < orderedGraph[o].size() && m < orderedGraph[p].size()) {
            vertex a = orderedGraph[u][i];
            vertex b = orderedGraph[v][j];
            vertex c = orderedGraph[w][k];
            vertex d = orderedGraph[o][l];
            vertex e = orderedGraph[p][m];

            if (a == b && a == c && a == d && a == e) {
                vertex x = a;
                vector<vertex> clique(5);
                increment_clique(root, {u, v, w, o, p, x}, ordered5Cs, s6cs, clique, 0, 0, 1);
                //cout << u << " " << v << " " << w << " " << o << " " << p << " " << x << endl;
                i++; j++; k++; l++, m++;
                fc++;
            }
            else {
                vertex mn = max ({a, b, c, d, e});
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
            }
        }
    }
    return fc;
}

void base_k56 (Graph& graph, bool hierarchy, edge nEdge, vector<vertex>& K, vertex* max45, string vfile, FILE* fp) {

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
    int clique_num = 0;
    create_4cliqueList (&root, orderedGraph, &clique_num);
    cout << "created 4-cliques" << endl;
    // Creating 5-clique list
    Graph ordered5Cs (clique_num); // like orderedGraph: each vector<vertex> is the list of rth vertices, ur, in the cliques of i-th r-1 cliques, u1 - u2 - u3... s.t. u1 < v2... < ur (deg ordering)
    vector<v5c> r5cs; // like el: list of the r-cliques aligned to the order in orderedCs
    vector<vertex> xr5cs; // like xel: indices in rcs that starts the r-clique list for an (r-1)-clique
    Graph s6cs (clique_num); // s-clique counts of each r-clique in the orderedCs structure
    create_5cliqueList (&root, orderedGraph, ordered5Cs, r5cs, xr5cs, s6cs);
    cout << "created 5-cliques" << endl;

    const auto t2 = chrono::steady_clock::now();

    print_time (fp, "Triangle enumeration: ", t2 - t1);

    const auto f1 = chrono::steady_clock::now();
    // 4-clique counting for each triangle
    lol fc = count6cliques (&root, graph, orderedGraph, ordered5Cs, r5cs, s6cs);
    cout << "fc: " << fc << endl;
    fprintf (fp, "# 5-cliques: %lld\n", fc);
    const auto f2 = chrono::steady_clock::now();
    print_time (fp, "5-clique counting: ", f2 - f1);

    // Peeling
    const auto p1 = chrono::steady_clock::now();
    K.resize (r5cs.size(), -1);
    Naive_Bucket nBucket;
    nBucket.Initialize (clique_num, r5cs.size()); // maximum 4-clique count of a triangle is nVtx

    vertex id = 0;
    for (size_t i = 0; i < s6cs.size(); i++)
        for (size_t j = 0; j < s6cs[i].size(); j++) {
            if (s6cs[i][j] > 0) {
                nBucket.Insert(xr5cs[i] + j, s6cs[i][j]);
                //cout << i << " " << j << " " << xr5cs[i] + j<< " " << s6cs[i][j] << endl;
            }
            else
                K[xr5cs[i] + j] = 0;
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
        component.resize (r5cs.size(), -1);
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

        vertex u = get<0> (r5cs[t]);
        vertex v = get<1> (r5cs[t]);
        vertex w = get<2> (r5cs[t]);
        vertex o = get<3> (r5cs[t]);
        vertex m = get<4> (r5cs[t]);
        vector<vector<vertex>> vList;
        vList.push_back(orderedGraph[u]);
        vList.push_back(orderedGraph[v]);
        vList.push_back(orderedGraph[w]);
        vList.push_back(orderedGraph[o]);
        vList.push_back(orderedGraph[m]);
        vector<vertex> commonNeighbors;
        //threeWay (graph[u], graph[v], graph[w], commonNeighbors);
        intersectionR(vList, commonNeighbors);
        cout << "bfs" << endl;
        for (auto x : commonNeighbors) { // decrease the FC of the neighbor triangles with greater FC
            /*
            vertex p = getTriangleId (u, v, x, xtris, el, xel, orderedTris, graph);
            vertex r = getTriangleId (u, w, x, xtris, el, xel, orderedTris, graph);
            vertex s = getTriangleId (v, w, x, xtris, el, xel, orderedTris, graph);
             */
            vertex p = getId(&root, {u, v, w, o, x});
            vertex r = getId(&root, {u, v, w, m, x});
            vertex s = getId(&root, {u, v, o, m, x});
            vertex q = getId(&root, {u, w, o, m, x});
            vertex z = getId(&root, {v, w, o, m, x});
            cout << p << " " << r << " " << s << " " << q << " " << t << endl;
            if ((p == -1 || K[p] == -1) && (r == -1 || K[r] == -1) && (s == -1 || K[s] == -1) && (q == -1 || K[q] == -1) && (z == -1 || K[z] == -1)) {
                if (p != -1 && nBucket.CurrentValue(p) > fc_t)
                    nBucket.DecVal(p);
                if (r != -1 && nBucket.CurrentValue(r) > fc_t)
                    nBucket.DecVal(r);
                if (s != -1 && nBucket.CurrentValue(s) > fc_t)
                    nBucket.DecVal(s);
                if (q != -1 && nBucket.CurrentValue(q) > fc_t)
                    nBucket.DecVal(q);
                if (z != -1 && nBucket.CurrentValue(z) > fc_t)
                    nBucket.DecVal(z);
            }
            else if (hierarchy)
                createSkeleton (t, {p, r, s, q, z}, &nSubcores, K, skeleton, component, unassigned, relations);
        }
        cout << "out common neighbor loop" << endl;
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




