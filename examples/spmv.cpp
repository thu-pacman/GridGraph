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
		fprintf(stderr, "usage: spmv [path] [memory budget in GB]\n");
		exit(-1);
	}
	std::string path = argv[1];
	long memory_bytes = ((argc>=4)?atol(argv[3]):8l)*1024l*1024l*1024l;

	Graph graph(path);
	assert(graph.edge_type==1);
	graph.set_memory_bytes(memory_bytes);
	BigVector<float> input(graph.path+"/input", graph.vertices);
	BigVector<float> output(graph.path+"/output", graph.vertices);
	graph.set_vertex_data_bytes( (long) graph.vertices * ( sizeof(float) * 2 ) );

	double begin_time = get_time();
	graph.hint(input, output);
	graph.stream_vertices<float>(
		[&](VertexId i){
			input[i] = i;
			output[i] = 0;
			return 0;
		}, nullptr, 0,
		[&](std::pair<VertexId,VertexId> vid_range){
			input.load(vid_range.first, vid_range.second);
			output.load(vid_range.first, vid_range.second);
		},
		[&](std::pair<VertexId,VertexId> vid_range){
			input.save();
			output.save();
		}
	);
	graph.hint(input);
	graph.stream_edges<float>(
		[&](Edge & e){
			write_add(&output[e.target], input[e.source] * e.weight);
			return 0;
		}, nullptr, 0, 1,
		[&](std::pair<VertexId,VertexId> source_vid_range){
			input.lock(source_vid_range.first, source_vid_range.second);
		},
		[&](std::pair<VertexId,VertexId> source_vid_range){
			input.unlock(source_vid_range.first, source_vid_range.second);
		}
	);
	double end_time = get_time();

	printf("spmv took %.2f seconds\n", end_time - begin_time);
}
