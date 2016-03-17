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

#define K 64

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
	BigVector<unsigned long [2]> visited(graph.path+"/visited", graph.vertices);
	BigVector<VertexId> radii(graph.path+"/radii", graph.vertices);
	graph.set_vertex_data_bytes( graph.vertices * ( sizeof(VertexId) + sizeof(long) * 2 ) );

	srand(time(NULL));

	double start_time = get_time();
	int iteration;
	VertexId active_vertices;
	VertexId max_radii;

	active_out->clear();
	graph.stream_vertices<VertexId>([&](VertexId i){
		visited[i][0] = 0ul;
		visited[i][1] = 0ul;
		radii[i] = -1;
		return 0;
	});
	for (int k=0;k<K;k++) {
		VertexId vid = rand() % graph.vertices;
		visited[vid][1] |= (1ul << k);
		radii[vid] = 0;
		active_out->set_bit(vid);
	}
	iteration = 0;
	active_vertices = K;
	while (active_vertices > 0) {
		iteration++;
		printf("%7d: %d\n", iteration, active_vertices);
		int now = iteration % 2;
		int next = 1 - now;
		std::swap(active_in, active_out);
		active_out->clear();
		active_vertices = graph.stream_edges<VertexId>([&](Edge & e) {
			if (visited[e.target][now] != visited[e.source][now]) {
				__sync_fetch_and_or( &visited[e.target][next], visited[e.source][now] );
				VertexId old_radii = radii[e.target];
				if (radii[e.target]!=iteration) {
					if (cas(&radii[e.target], old_radii, iteration)) {
						active_out->set_bit(e.target);
						return 1;
					}
				}
			}
			return 0;
		}, active_in);
		active_vertices = graph.stream_vertices<VertexId>([&](VertexId i){
			visited[i][now] = visited[i][next];
			return 1;
		}, active_out); // necessary?
	}
	max_radii = 0;
	for (VertexId i=0;i<graph.vertices;i++) {
		if (max_radii < radii[i]) {
			max_radii = radii[i];
		}
	}
	std::vector<VertexId> candidates;
	VertexId threshold = 0;
	while (candidates.size()<K) {
		for (VertexId i=0;i<graph.vertices;i++) {
			if (radii[i]==max_radii-threshold) candidates.push_back(i);
		}
		threshold++;
	}
	printf("radii:%d\n", max_radii);

	active_out->clear();
	graph.stream_vertices<VertexId>([&](VertexId i){
		visited[i][0] = 0ul;
		visited[i][1] = 0ul;
		radii[i] = -1;
		return 0;
	});
	for (int k=0;k<K;k++) {
		VertexId vid = candidates[k];
		visited[vid][1] |= (1ul << k);
		radii[vid] = 0;
		active_out->set_bit(vid);
	}
	iteration = 0;
	active_vertices = K;
	while (active_vertices > 0) {
		iteration++;
		printf("%7d: %d\n", iteration, active_vertices);
		int now = iteration % 2;
		int next = 1 - now;
		std::swap(active_in, active_out);
		active_out->clear();
		active_vertices = graph.stream_edges<VertexId>([&](Edge & e) {
			if (visited[e.target][now] != visited[e.source][now]) {
				__sync_fetch_and_or( &visited[e.target][next], visited[e.source][now] );
				VertexId old_radii = radii[e.target];
				if (radii[e.target]!=iteration) {
					if (cas(&radii[e.target], old_radii, iteration)) {
						active_out->set_bit(e.target);
						return 1;
					}
				}
			}
			return 0;
		}, active_in);
		active_vertices = graph.stream_vertices<VertexId>([&](VertexId i){
			visited[i][now] = visited[i][next];
			return 1;
		}, active_out); // necessary?
	}
	max_radii = 0;
	for (VertexId i=0;i<graph.vertices;i++) {
		if (max_radii < radii[i]) {
			max_radii = radii[i];
		}
	}
	printf("radii: %d\n", max_radii);

	double end_time = get_time();
	printf("radii: %d\n", max_radii);
	printf("time: %.2f seconds\n", end_time - start_time);

	return 0;
}

