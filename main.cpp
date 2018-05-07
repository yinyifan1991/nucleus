#include "main.h"

int main (int argc, char *argv[]) {

	const auto t1 = chrono::steady_clock::now();
	/*
	if (argc < 3) {
		fprintf(stderr, "usage: %s "
				"\n <filename>"
				"\n <nucleus type: 12, 13, 14, 23, 24, 34, 45>"
				"\n <hierarchy?: YES or NO>\n", argv[0]);
		exit(1);
	}

	char *filename = argv[1];
	string tmp (argv[1]);
	string gname = tmp.substr (tmp.find_last_of("/") + 1);


	string nd (argv[2]);
	if (!(nd == "12" || nd == "13" || nd == "14" || nd == "23" || nd == "24" || nd == "34" || nd == "45")) {
		printf ("Invalid algorithm, options are 12, 13, 14, 23, 24, and 34\n");
		exit(1);
	}
	*/
	// read the graph, give sorted edges in graph
	/*
	edge nEdge = 0;
	Graph graph;
	readGraph<vertex, edge> (filename, graph, &nEdge);
	string hrc (argv[3]);
	string vfile = gname + "_" + nd;
	string out_file;
	 */
	char *filename = "/home/yifan/CLionProjects/nd/meta_Video_Games40.mtx";
	string tmp ("meta_Video_Games40.mtx");
	//string gname = tmp.substr (tmp.find_last_of("/") + 1);
    string gname (filename);
	string nd ("45gen");
	edge nEdge = 0;
	Graph graph;
	readGraph<vertex, edge> (filename, graph, &nEdge);
	string hrc ("NO");
	string vfile = gname + "_" + nd;
	string out_file;

	bool hierarchy = (hrc == "YES" ? true : false);
	if (hierarchy)
		out_file = vfile + "_Hierarchy";
	else
		out_file = vfile + "_K";

	FILE* fp = fopen (out_file.c_str(), "w");
	vertex maxK; // maximum K value in the graph
	vector<vertex> K;

	if (nd == "12")
		base_kcore (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
	else if (nd == "13")
		base_k13 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
	else if (nd == "14")
		base_k14 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
	else if (nd == "23") {
		base_ktruss (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
		//		base_ktruss_storeTriangles (graph, hierarchy, nEdge/2, K, &maxK, vfile, fp);
	}
	else if (nd == "24")
		base_k24 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
	else if (nd == "34")
		base_k34 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "45")
        base_k45 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "56")
        base_k56 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "67")
        base_k67 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "78")
        base_k78 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "89")
        base_k89 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "9n10")
        base_k9n10 (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "45gen")
        base_k45_gen (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "56gen")
        base_k56_gen (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "67gen")
        base_k67_gen (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "78gen")
        base_k78_gen (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "89gen")
        base_k89_gen (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "9n10gen")
        base_k9n10_gen (graph, hierarchy, nEdge, K, &maxK, vfile, fp);
    else if (nd == "45file")
        base_k45_file (graph, hierarchy, nEdge, K, &maxK, vfile, fp, filename);
    else if (nd == "56file")
        base_k56_file (graph, hierarchy, nEdge, K, &maxK, vfile, fp, filename);
    else if (nd == "67file")
        base_k67_file (graph, hierarchy, nEdge, K, &maxK, vfile, fp, filename);
    else if (nd == "78file")
        base_k78_file (graph, hierarchy, nEdge, K, &maxK, vfile, fp, filename);
    else if (nd == "89file")
        base_k89_file (graph, hierarchy, nEdge, K, &maxK, vfile, fp, filename);
    else if (nd == "9n10file")
        base_k9n10_file (graph, hierarchy, nEdge, K, &maxK, vfile, fp, filename);

#ifdef DUMP_K
	string kfile = vfile + "_K_values";
	FILE* kf = fopen (kfile.c_str(), "w");
	for (vertex i = 0; i < K.size(); i++)
		fprintf (kf, "%lld\n", K[i]);
	fclose (kf);
#endif

	const auto t2 = chrono::steady_clock::now();
	printf ("%s\t|V|: %d\t|E|: %d\tmaxK for %s-nucleus: %d\n", gname.c_str(), graph.size(), nEdge, nd.c_str(), maxK);
	print_time (fp, "End-to-end Time: ", t2 - t1);
	fclose (fp);

	return 0;
}
