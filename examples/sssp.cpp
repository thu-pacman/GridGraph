/*************************************************************************
    > File Name: sssp.cpp
    > Author: Mengxing Liu
    > Mail: liumengxing2010@qq.com 
    > Created Time: Sun Jun 19 19:23:36 2016
 ************************************************************************/

#include<iostream>
#include "core/graph.hpp"

using namespace std;
#define MAX_DEPTH 100000000

int main(int argc, char ** argv) {
	if (argc<3) {
		fprintf(stderr, "usage: bfs [path] [start vertex id] [memory budget in GB]\n");
		exit(-1);
	}
	std::string path = argv[1];
	VertexId start_vid = atoi(argv[2]);
	long memory_bytes = (argc>=4)?atol(argv[3])*1024l*1024l*1024l:8l*1024l*1024l*1024l;

	Graph graph(path);
	graph.set_memory_bytes(memory_bytes);
	Bitmap * active_in = graph.alloc_bitmap();
	Bitmap * active_out = graph.alloc_bitmap();
	BigVector<VertexId> depth(graph.path+"/depth", graph.vertices);
	graph.set_vertex_data_bytes( graph.vertices * sizeof(VertexId) );

	active_out->clear();
	active_out->set_bit(start_vid);
	depth.fill(MAX_DEPTH);
	depth[start_vid] = 0;
	VertexId active_vertices = 1;

	double start_time = get_time();
	int iteration = 0;
	while (active_vertices!=0 && iteration< 100) {
		iteration++;
		printf("%7d: %d\n", iteration, active_vertices);
		std::swap(active_in, active_out);
		active_out->clear();
		graph.hint(depth);

		active_vertices = graph.stream_edges<VertexId>([&](Edge & e){
			int r = depth[e.target];
			int n = depth[e.source]+ e.weight;
			if(n < r){
				if (cas(&depth[e.target], r, n)) {
					active_out->set_bit(e.target);
					return 1;
				}
			}
			return 0;
		}, active_in);
	}
	double end_time = get_time();

	int discovered_vertices = graph.stream_vertices<VertexId>([&](VertexId i){
		return depth[i] < MAX_DEPTH;
	});
	printf("discovered %d vertices from %d in %.2f seconds.\n", discovered_vertices, start_vid, end_time - start_time);

	for (int i=0; i<100; i++){
		printf("%d  %d\n", i, depth[i]);
	}
	return 0;
}