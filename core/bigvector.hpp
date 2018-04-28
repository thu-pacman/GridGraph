/*
Copyright (c) 2014-2015 Xiaowei Zhu, Tsinghua University
Copyright (c) 2018 Hippolyte Barraud, Tsinghua University

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

#ifndef BIGVECTOR_H
#define BIGVECTOR_H

#include <cassert>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#ifdef USE_OPENMP
#include <omp.h>
#endif

#include <thread>
#include <string>

#include "core/filesystem.hpp"
#include "core/partition.hpp"

void *memalign(size_t alignment, size_t size) {
	void *ret;
	if (posix_memalign(&ret, alignment, size) != 0) {
	    ret = nullptr;
	};
	return ret;
}

template <typename T>
class BigVector {
	std::string path;
	bool is_open;
	bool in_memory = false;
	size_t begin_i = 0, end_i = 0;
	T * data_in_memory = NULL;
	static const long PAGESIZE = 4096;
public:
	int fd;
	T * data;
	size_t length;
	BigVector() {
		is_open = false;
		data = NULL;
		length = 0;
	}
	BigVector(std::string path, size_t length) {
		init(path, length);
	}
	BigVector(std::string path) {
		init(path);
	}
	~BigVector() {
		if (is_open && file_exists(path)) {
			close_mmap();
		}
	}
	void init(std::string path) {
		assert(file_exists(path));
		assert(file_size(path) % sizeof(T) == 0);
		init(path, file_size(path) / sizeof(T));
	}
	void init(std::string path, size_t length) {
		this->path = path;
		this->length = length;
		if (!file_exists(path)) {
			FILE * fout = fopen(path.c_str(), "wb");
			fclose(fout);
		}
		if (static_cast<unsigned>(file_size(path)) != sizeof(T) * length) {
			long file_length = sizeof(T) * length;
			assert(truncate(path.c_str(), file_length)!=-1);
			int fout = open(path.c_str(), O_WRONLY);
			void * buffer = memalign(PAGESIZE, PAGESIZE);
			for (long offset=0;offset<file_length;) {
				if (file_length - offset > PAGESIZE) {
					assert(write(fout, buffer, PAGESIZE)==PAGESIZE);
					offset += PAGESIZE;
				}
				else {
					assert(write(fout, buffer, file_length - offset)==file_length - offset);
					offset += file_length - offset;
				}
			}
			close(fout);
		}
		fd = open(path.c_str(), O_RDWR | O_SYNC);
		assert(fd!=-1);
		open_mmap();
	}
	void open_mmap() {
		//int ret = posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL); //This is mostly useless on modern system
		//assert(ret==0);
		data = (T *)mmap(NULL, sizeof(T) * length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		assert(data!=MAP_FAILED);
		is_open = true;
	}
	void close_mmap() {
		is_open = false;
		int ret = munmap(data, sizeof(T) * length);
		assert(ret==0);
	}
	void fill(const T & value) {
		int parallelism = std::thread::hardware_concurrency();
		#pragma omp parallel num_threads(parallelism)
		{
			size_t begin_i, end_i;
			std::tie(begin_i, end_i) = get_partition_range(length, omp_get_num_threads(), omp_get_thread_num());
			for (size_t i=begin_i;i<end_i;i++) {
				data[i] = value;
			}
		}
		#pragma omp barrier
	}
	T & operator[](size_t i) {
		if (in_memory) {
			if (!(i >= begin_i && i <= end_i)) {
				printf("%s %lu %lu %lu\n", path.c_str(), begin_i, i, end_i);
				exit(-1);
			}
			return data_in_memory[i - begin_i];
		} else {
			return data[i];
		}
	}
	void sync() {
		assert(msync(data, sizeof(T) * length, MS_SYNC)==0);
	}
	void lock(size_t begin_i, size_t end_i) {
		assert(mlock(data + begin_i, (end_i - begin_i) * sizeof(T))==0);
	}
	void unlock(size_t begin_i, size_t end_i) {
		assert(munlock(data + begin_i, (end_i - begin_i) * sizeof(T))==0);
	}
	void load(size_t begin_i, size_t end_i) {
		close_mmap();
		begin_i = begin_i * sizeof(T) / PAGESIZE * PAGESIZE / sizeof(T);
		this->begin_i = begin_i;
		this->end_i = end_i;
		in_memory = true;
		// data_in_memory = (T *)memalign(PAGESIZE, (end_i - begin_i) * sizeof(T) + PAGESIZE);
		// assert(data_in_memory!=NULL);
		data_in_memory = (T *)mmap(0, (end_i - begin_i) * sizeof(T) + PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		assert(data_in_memory!=MAP_FAILED);
		long end_offset = end_i * sizeof(T);
		long offset = begin_i * sizeof(T);
		long bytes;
		while (offset < end_offset) {
			bytes = pread(fd, data_in_memory + (offset / sizeof(T) - begin_i), (end_offset - offset + PAGESIZE - 1) / PAGESIZE * PAGESIZE, offset);
			if (bytes==-1) {
				printf("%ld %ld\n", offset, end_offset);
				printf("%s\n", strerror(errno));
				getchar();
				exit(-1);
			}
			offset += bytes;
		}
	}
	void save() {
		long end_offset = end_i * sizeof(T);
		long offset = begin_i * sizeof(T);
		long bytes;
		while (offset < end_offset) {
			bytes = pwrite(fd, data_in_memory + (offset / sizeof(T) - begin_i), (end_offset - offset + PAGESIZE - 1) / PAGESIZE * PAGESIZE, offset);
			if (bytes==-1) {
				printf("%ld %ld\n", offset, end_offset);
				printf("%s\n", strerror(errno));
				getchar();
				exit(-1);
			}
			offset += bytes;
		}
		int ret = munmap(data_in_memory, (end_i - begin_i) * sizeof(T) + PAGESIZE);
		assert(ret==0);
		in_memory = false;
		begin_i = 0;
		end_i = 0;
		open_mmap();
	}
};

#endif
