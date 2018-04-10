//
// Created by yifan on 3/17/18.
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

// per 4-clique
lol count5cliques (com_node *root, Graph& graph, Graph& orderedGraph, Graph& ordered4Cs, vector<v4c>& r4cs, Graph& s5cs) {
    //cout << "start count 5 cliques" << endl;
    lol fc = 0;
    for (auto t : r4cs) {
        vertex i = 0, j = 0, k = 0, l = 0;
        vertex u = get<0>(t);
        vertex v = get<1>(t);
        vertex w = get<2>(t);
        vertex o = get<3>(t);
        while (i < orderedGraph[u].size() && j < orderedGraph[v].size() && k < orderedGraph[w].size() && l < orderedGraph[o].size()) {
            vertex a = orderedGraph[u][i];
            vertex b = orderedGraph[v][j];
            vertex c = orderedGraph[w][k];
            vertex d = orderedGraph[o][l];

            if (a == b && a == c && a == d) {
                vertex x = a;
                vector<vertex> clique(4);
                increment_clique(root, {u, v, w, o, x}, ordered4Cs, s5cs, clique, 0, 0, 1);
                cout << u << " " << v << " " << w << " " << o << " " << " " << x << endl;
                i++; j++; k++; l++;
                fc++;
            }
            else {
                vertex m = max ({a, b, c, d});
                if (a != m)
                    i++;
                if (b != m)
                    j++;
                if (c != m)
                    k++;
                if (d != m)
                    l++;
            }
        }
    }
    return fc;
}

void base_k45 (Graph& graph, bool hierarchy, edge nEdge, vector<vertex>& K, vertex* max45, string vfile, FILE* fp) {

    const auto t1 = chrono::steady_clock::now();
    vertex nVtx = graph.size();

    // Create directed graph from low degree vertices to higher degree vertices AND prepare a CSR-like structure to index the edges
    Graph orderedGraph;
    com_node root(-1, 0, -1);
    //createOrderedIndexEdges (graph, el, xel, orderedGraph);
    createOrderedIndexEdgesNode(graph, &root, orderedGraph);
    cout << "created edges" << endl;
    // Creating triangle list
    int triangle_num = 0;
    create_triangleList (&root, orderedGraph, &triangle_num);
    cout << "created triangles" << endl;
    // Creating 4-clique list
    Graph ordered4Cs (triangle_num); // like orderedGraph: each vector<vertex> is the list of rth vertices, ur, in the cliques of i-th r-1 cliques, u1 - u2 - u3... s.t. u1 < v2... < ur (deg ordering)
    vector<v4c> r4cs; // like el: list of the r-cliques aligned to the order in orderedCs
    vector<vertex> xr4cs; // like xel: indices in rcs that starts the r-clique list for an (r-1)-clique
    Graph s5cs (triangle_num); // s-clique counts of each r-clique in the orderedCs structure
    create_4cliqueList (&root, orderedGraph, ordered4Cs, r4cs, xr4cs, s5cs);
    cout << "created 4-cliques" << endl;
    const auto t2 = chrono::steady_clock::now();

    print_time (fp, "Triangle enumeration: ", t2 - t1);

    const auto f1 = chrono::steady_clock::now();
    // 4-clique counting for each triangle
    lol fc = count5cliques (&root, graph, orderedGraph, ordered4Cs, r4cs, s5cs);
    cout << "fc: " << fc << endl;
    fprintf (fp, "# 4-cliques: %lld\n", fc);
    const auto f2 = chrono::steady_clock::now();
    print_time (fp, "4-clique counting: ", f2 - f1);

    // Peeling
    const auto p1 = chrono::steady_clock::now();
    K.resize (r4cs.size(), -1);
    Naive_Bucket nBucket;
    nBucket.Initialize (triangle_num, r4cs.size()); // maximum 4-clique count of a triangle is nVtx

    vertex id = 0;
    for (size_t i = 0; i < s5cs.size(); i++)
        for (size_t j = 0; j < s5cs[i].size(); j++) {
            if (s5cs[i][j] > 0) {
                nBucket.Insert(xr4cs[i] + j, s5cs[i][j]);
                cout << i << " " << j << " " << xr4cs[i] + j<< " " << s5cs[i][j] << endl;
            }
            else
                K[xr4cs[i] + j] = 0;
            id++;
        }
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
        component.resize (r4cs.size(), -1);
    }

    vertex monitor = 0;
    while (true) {
        edge t;
        edge val ;
        cout << "prepare to popmin" << endl;
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

        vertex u = get<0> (r4cs[t]);
        vertex v = get<1> (r4cs[t]);
        vertex w = get<2> (r4cs[t]);
        vertex o = get<3> (r4cs[t]);
        vector<vector<vertex>> vList;
        vList.push_back(orderedGraph[u]);
        vList.push_back(orderedGraph[v]);
        vList.push_back(orderedGraph[w]);
        vList.push_back(orderedGraph[o]);
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
            vertex p = getId(&root, {u, v, w, x});
            cout << "p: " << p << " ";
            vertex r = getId(&root, {u, v, o, x});
            cout << "r: " << r <<  " ";
            vertex s = getId(&root, {u, w, o, x});
            cout << "s: " << s << " ";
            vertex q = getId(&root, {v, w, o, x});
            cout << "q: " << q << endl;
            //cout << p << " " << r << " " << s << " " << q << endl;
            if ((p == -1 || K[p] == -1) && (r == -1 || K[r] == -1) && (s == -1 || K[s] == -1) && (q == -1 || K[q] == -1)) {
                if (p != -1 && nBucket.CurrentValue(p) > fc_t)
                    nBucket.DecVal(p);
                if (r != -1 && nBucket.CurrentValue(r) > fc_t)
                    nBucket.DecVal(r);
                if (s != -1 && nBucket.CurrentValue(s) > fc_t)
                    nBucket.DecVal(s);
                if (q != -1 && nBucket.CurrentValue(q) > fc_t)
                    nBucket.DecVal(q);
            }
            else if (hierarchy)
                createSkeleton (t, {p, r, s, q}, &nSubcores, K, skeleton, component, unassigned, relations);
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


