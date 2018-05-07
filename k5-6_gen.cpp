//
// Created by yifan on 5/6/18.
//

#include <unordered_set>
#include "main.h"
#include "clique_util.h"

const int ROUNDS = 4;

void traverse_cliqueTree (com_node* parent, vector<com_node>& ancestors, Graph& orderedGraph, Graph& orderedCs, vector<v5c>& rcs, vector<vertex>& xrcs, Graph& scs, vector<int>& id_in_level, int rounds) {
    if(rounds < 0) return;
    for(int i = 0;i < parent->nodes.size();i++) {
        com_node *u = &parent->nodes[i];
        ancestors.push_back(*u);
        if(rounds > 0) {
            traverse_cliqueTree(u, ancestors, orderedGraph, orderedCs, rcs, xrcs, scs, id_in_level, rounds-1);
        }
        else {
            int counter = id_in_level.back();
            vector<vector<vertex>> vList;
            for(int i = 0;i < ancestors.size();i++) {
                vList.push_back(orderedGraph[ancestors[i].val]);
            }
            vector<vertex> commonNeighbors;
            intersectionR(vList, commonNeighbors);

            for (auto x : commonNeighbors) {
                (*u).nodes.push_back(com_node(x, u->order+1, counter));
                counter++;
                orderedCs[(*u).id].push_back (x);
                scs[(*u).id].push_back (0);
                v5c cl = make_tuple (ancestors[0].val, ancestors[1].val, ancestors[2].val, ancestors[3].val, (vertex)x);
                rcs.push_back (cl);
            }
            id_in_level.back() = counter;
            xrcs.push_back(counter);
        }
        ancestors.pop_back();
    }
}

void create_rcliqueList (com_node *root, Graph& orderedGraph, Graph& orderedCs, vector<v5c>& rcs, vector<vertex>& xrcs, Graph& scs, vector<int>& id_in_level) {
    vector<com_node> ancestors;
    for(size_t i1 = 0;i1 < root->nodes.size();i1++) {
        com_node *u1 = &root->nodes[i1];
        ancestors.push_back(*u1);
        traverse_cliqueTree(u1, ancestors, orderedGraph, orderedCs, rcs, xrcs, scs, id_in_level, ROUNDS-2);
        ancestors.pop_back();
    }
}


void create_cliqueList(com_node *root, com_node* parent, vector<com_node>& ancestors, Graph& orderedGraph, Graph& orderedCs, vector<v5c>& rcs, vector<vertex>& xrcs, Graph& scs, vector<int>& id_in_level, int rounds) {
    if(rounds > ROUNDS) {
        return;
    }
    vector<vector<vertex>> vList;
    int counter = id_in_level[rounds-3];
    for(int p = 0;p < ancestors.size();p++) {
        vList.push_back(orderedGraph[ancestors[p].val]);
    }
    vector<vertex> commonNeighbors;
    intersectionR(vList, commonNeighbors);
    for(auto w: commonNeighbors) {
        com_node child(w, parent->order+1, counter);
        counter++;
        parent->nodes.push_back(child);
        ancestors.push_back(static_cast<com_node &&>(parent->nodes.back()));
        create_cliqueList(root, &(*parent).nodes.back(), ancestors, orderedGraph, orderedCs, rcs, xrcs, scs, id_in_level, rounds + 1);
        ancestors.pop_back();
    }
    id_in_level[rounds-3] = counter;
}

// per 7-clique
lol count_cliques_gen (com_node *root, Graph& graph, Graph& orderedGraph, Graph& orderedCs, vector<v5c>& rcs, Graph& scs) {
    lol fc = 0;
    for (auto t : rcs) {
        vertex i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;
        vertex u1 = get<0>(t);
        vertex u2 = get<1>(t);
        vertex u3 = get<2>(t);
        vertex u4 = get<3>(t);
        vertex u5 = get<4>(t);
        while (i1 < orderedGraph[u1].size() && i2 < orderedGraph[u2].size() && i3 < orderedGraph[u3].size()
               && i4 < orderedGraph[u4].size() && i5 < orderedGraph[u5].size()) {
            vertex a1 = orderedGraph[u1][i1];
            vertex a2 = orderedGraph[u2][i2];
            vertex a3 = orderedGraph[u3][i3];
            vertex a4 = orderedGraph[u4][i4];
            vertex a5 = orderedGraph[u5][i5];

            if (a1 == a2 && a1 == a3 && a1 == a4 && a1 == a5) {
                vertex x = a1;
                vector<vertex> clique(ROUNDS+1);
                increment_clique(root, {u1, u2, u3, u4, u5, x}, orderedCs, scs, clique, 0, 0, 1);
                cout << u1 << " " << u2 << " " << u3 << " " << u4 << " " << u5 << " : " << x << endl;
                i1++; i2++; i3++; i4++, i5++;
                fc++;
            }
            else {
                vertex mn = max ({a1, a2, a3, a4, a5});
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
            }
        }
    }
    return fc;
}


// create 3 to r-clique then count s-clique
lol create_cliques_and_count(com_node *root, Graph& graph, Graph& orderedGraph, Graph& orderedCs, vector<v5c>& rcs, vector<vertex>& xrcs, Graph& scs, int *clique_num) {
    vector<com_node> ancestors; // store all the vertices before the rth vertex of a r-clique
    vector<int> id_in_level(ROUNDS-1); // record total number of n-cliques where n = 3, 4, 5, 6
    int rounds = 3;
    for(size_t i1 = 0;i1 < (*root).nodes.size();i1++) {
        com_node *u1 = &root->nodes[i1];
        ancestors.push_back(*u1);
        for(size_t i2 = 0;i2 < u1->nodes.size();i2++) {
            com_node *u2 = &u1->nodes[i2];
            ancestors.push_back(*u2);
            create_cliqueList(root, u2, ancestors, orderedGraph, orderedCs, rcs, xrcs, scs, id_in_level, rounds);
            ancestors.pop_back();
        }
        ancestors.pop_back();
    }
    // finished creating (r-1)-clique list and count the total number of (r-1)-cliques
    *clique_num = id_in_level[ROUNDS-3];
    orderedCs.resize(*clique_num);
    scs.resize(*clique_num);
    // create r-clique list
    create_rcliqueList (root, orderedGraph, orderedCs, rcs, xrcs, scs, id_in_level);
    return count_cliques_gen (root, graph, orderedGraph, orderedCs, rcs, scs);
}

void base_k56_gen (Graph& graph, bool hierarchy, edge nEdge, vector<vertex>& K, vertex* max45, string vfile, FILE* fp) {

    const auto t1 = chrono::steady_clock::now();
    vertex nVtx = graph.size();

    // Create directed graph from low degree vertices to higher degree vertices AND prepare a CSR-like structure to index the edges
    Graph orderedGraph;
    com_node root(-1, 0, -1); // dummy node
    createOrderedIndexEdgesNode(graph, &root, orderedGraph);

    const auto t2 = chrono::steady_clock::now();

    print_time (fp, "Triangle enumeration: ", t2 - t1);

    const auto f1 = chrono::steady_clock::now();

    Graph orderedCs; // like orderedGraph: each vector<vertex> is the list of rth vertices, ur, in the cliques of i-th r-1 cliques, u1 - u2 - u3... s.t. u1 < v2... < ur (deg ordering)
    vector<v5c> rcs; // like el: list of the r-cliques aligned to the order in orderedCs
    vector<vertex> xrcs; // like xel: indices in rcs that starts the r-clique list for an (r-1)-clique
    Graph scs; // s-clique counts of each r-clique in the orderedCs structure
    int clique_num = 0; // total number of (r-1)-clique

    xrcs.push_back(0);

    // create 3 to r-clique and count s-clique
    lol fc = create_cliques_and_count(&root, graph, orderedGraph, orderedCs, rcs, xrcs, scs, &clique_num);


    cout << "fc: " << fc << endl;
    fprintf (fp, "# 6-cliques: %lld\n", fc);
    const auto f2 = chrono::steady_clock::now();
    print_time (fp, "6-clique counting: ", f2 - f1);

    // Peeling
    const auto p1 = chrono::steady_clock::now();
    K.resize (rcs.size(), -1);
    Naive_Bucket nBucket;
    nBucket.Initialize (clique_num, rcs.size()); // maximum 4-clique count of a triangle is nVtx
    vertex id = 0;
    for (size_t i = 0; i < scs.size(); i++) {
        for (size_t j = 0; j < scs[i].size(); j++) {
            if (scs[i][j] > 0) {
                nBucket.Insert(xrcs[i] + j, scs[i][j]);
            }
            else
                K[xrcs[i] + j] = 0;
            id++;
        }
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
        vector<vector<vertex>> vList;
        vList.push_back(orderedGraph[u1]);
        vList.push_back(orderedGraph[u2]);
        vList.push_back(orderedGraph[u3]);
        vList.push_back(orderedGraph[u4]);
        vList.push_back(orderedGraph[u5]);
        vector<vertex> commonNeighbors;
        intersectionR(vList, commonNeighbors);
        for (auto x : commonNeighbors) { // decrease the FC of the neighbor triangles with greater FC
            vertex v1 = getId(&root, {u1, u2, u3, u4, x});
            vertex v2 = getId(&root, {u1, u2, u3, u5, x});
            vertex v3 = getId(&root, {u1, u2, u4, u5, x});
            vertex v4 = getId(&root, {u1, u3, u4, u5, x});
            vertex v5 = getId(&root, {u2, u3, u4, u5, x});
            if ((v1 == -1 || K[v1] == -1) && (v2 == -1 || K[v2] == -1) &&
                (v3 == -1 || K[v3] == -1) && (v4 == -1 || K[v4] == -1) &&
                (v5 == -1 || K[v5] == -1)) {
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
            }
            else if (hierarchy)
                createSkeleton (t, {v1, v2, v3, v4, v5}, &nSubcores, K, skeleton, component, unassigned, relations);
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
        print_time (fp, "Total 5,6 nucleus decomposition time (excluding density computation): ", (p2 - p1) + (f2 - f1) + (t2 - t1) + (b2 - b1));

        fprintf (fp, "# subcores: %d\t\t # subsubcores: %d\t\t |V|: %d\n", nSubcores, skeleton.size(), graph.size());

        const auto d1 = chrono::steady_clock::now();
        //helpers hp (&tris);
        //presentNuclei (34, skeleton, component, graph, nEdge, hp, vfile, fp);
        const auto d2 = chrono::steady_clock::now();

        print_time (fp, "Total 5,6 nucleus decomposition time: ", (p2 - p1) + (f2 - f1) + (t2 - t1) + (b2 - b1) + (d2 - d1));
    }
}

