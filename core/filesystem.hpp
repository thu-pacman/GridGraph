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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

inline bool file_exists(std::string filename) {
	struct stat st;
	return stat(filename.c_str(), &st)==0;
}

inline long file_size(std::string filename) {
	struct stat st;
	assert(stat(filename.c_str(), &st)==0);
	return st.st_size;
}

inline void create_directory(std::string path) {
	assert(mkdir(path.c_str(), 0764)==0 || errno==EEXIST);
}

// TODO: only on unix-like systems
inline void remove_directory(std::string path) {
	char command[1024];
	sprintf(command, "rm -rf %s", path.c_str());
	system(command);
}

#endif