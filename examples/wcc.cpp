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
		fprintf(stderr, "usage: wcc [path] [memory budget in GB]\n");
		exit(-1);
	}
	std::string path = argv[1];
	long memory_bytes = (argc>=3)?atol(argv[2])*1024l*1024l*1024l:8l*1024l*1024l*1024l;

	Graph graph(path);
	graph.set_memory_bytes(memory_bytes);
	Bitmap * active_in = graph.alloc_bitmap();
	Bitmap * active_out = graph.alloc_bitmap();
	BigVector<VertexId> label(graph.path+"/label", graph.vertices);
	graph.set_vertex_data_bytes( graph.vertices * sizeof(VertexId) );

	active_out->fill();
	VertexId active_vertices = graph.stream_vertices<VertexId>([&](VertexId i){
		label[i] = i;
		return 1;
	});

	double start_time = get_time();
	int iteration = 0;
	while (active_vertices!=0) {
		iteration++;
		printf("%7d: %d\n", iteration, active_vertices);
		std::swap(active_in, active_out);
		active_out->clear();
		graph.hint(label);
		active_vertices = graph.stream_edges<VertexId>([&](Edge & e){
			if (label[e.source]<label[e.target]) {
				if (write_min(&label[e.target], label[e.source])) {
					active_out->set_bit(e.target);
					return 1;
				}
			}
			return 0;
		}, active_in);
	}
	double end_time = get_time();

	BigVector<VertexId> label_stat(graph.path+"/label_stat", graph.vertices);
	label_stat.fill(0);
	graph.stream_vertices<VertexId>([&](VertexId i){
		write_add(&label_stat[label[i]], 1);
		return 1;
	});
	VertexId components = graph.stream_vertices<VertexId>([&](VertexId i){
		return label_stat[i]!=0;
	});
	printf("%d components found in %.2f seconds\n", components, end_time - start_time);

	return 0;
}
