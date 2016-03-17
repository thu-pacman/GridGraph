/*
Copyright (c) 2014-2015 Xiaowei Zhu, Tsinghua University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "core/graph.hpp"

int main(int argc, char ** argv) {
	if (argc<2) {
		fprintf(stderr, "usage: mis [path] [memory budget in GB]\n");
		exit(-1);
	}
	std::string path = argv[1];
	long memory_bytes = ((argc>=3)?atol(argv[2]):8l) * (1024l*1024l*1024l);

	Graph graph(path);
	graph.set_memory_bytes(memory_bytes);
	Bitmap * active_in = graph.alloc_bitmap();
	Bitmap * active_out = graph.alloc_bitmap();
	BigVector<bool> in_mis(graph.path+"/in_mis", graph.vertices);
	graph.set_vertex_data_bytes( graph.vertices * sizeof(bool) );

	active_out->fill();
	VertexId active_vertices = graph.stream_vertices<VertexId>([&](VertexId i){
		in_mis[i] = true;
		return 1;
	});

	double start_time = get_time();
	int iteration = 0;
	while (true) {
		iteration++;
		printf("%7d: %d\n", iteration, active_vertices);
		std::swap(active_in, active_out);
		graph.stream_edges<VertexId>([&](Edge & e) {
			if (e.source<e.target && in_mis[e.target]) {
				in_mis[e.target] = false;
			}
			return 0;
		}, active_in);
		active_out->clear();
		VertexId next_active_vertices = graph.stream_vertices<VertexId>([&](VertexId i){
			if (in_mis[i]) {
				active_out->set_bit(i);
				return 1;
			} else {
				in_mis[i] = true;
				return 0;
			}
		});
		if (active_vertices==next_active_vertices) break;
		active_vertices = next_active_vertices;
	}
	double end_time = get_time();
	printf("in_mis: %d\n", active_vertices);
	printf("time: %.2f seconds\n", end_time - start_time);

	return 0;
}

