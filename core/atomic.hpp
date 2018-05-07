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

#ifndef ATOMIC_H
#define ATOMIC_H

#include <cstdio>
#include <cstdlib>
#include <cassert>

template <class ET>
inline bool cas(ET *ptr, ET oldv, ET newv) {
	if (sizeof(ET) == sizeof(uint64_t)) {
		return __sync_bool_compare_and_swap((uint64_t*)ptr, (uint64_t)oldv, (uint64_t)newv);
	} else if (sizeof(ET) == sizeof(uint32_t)) {
		return __sync_bool_compare_and_swap((uint32_t*)ptr, (uint32_t)oldv, (uint32_t)newv);
	} else {
		assert(false);
	}
}

template <class ET>
inline bool write_min(ET *a, ET b) {
	ET c; bool r=0;
	do c = *a;
	while (c > b && !(r=cas(a,c,b)));
	return r;
}

template <class ET>
inline void write_add(ET *a, ET b) {
	volatile ET newV, oldV;
	do {oldV = *a; newV = oldV + b;}
	while (!cas(a, oldV, newV));
}

inline void *memalign(size_t alignment, size_t size) {
	void *ret;
	if (posix_memalign(&ret, alignment, size) != 0) {
		ret = nullptr;
	};
	return ret;
}

#endif
