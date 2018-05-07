#[[Copyright (c) 2018 Hippolyte Barraud, Tsinghua University

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.]]

file(GLOB EXAMPLES_SRC "*.cpp")

find_package(OpenMP)

if (OpenMP_CXX_FOUND)
    add_definitions(-DUSE_OPENMP)
else()
    message(FATAL_ERROR "OpenMP could not be found. Aborting")
endif ()

foreach(EXAMPLE_SRC ${EXAMPLES_SRC})
    get_filename_component(TRAGET_NAME ${EXAMPLE_SRC} NAME_WE)
    message(STATUS "Configuring ${TRAGET_NAME}")

    add_executable(${TRAGET_NAME} ${EXAMPLE_SRC} ${CORE_HEADERS})
    target_compile_options(${TRAGET_NAME} PUBLIC ${OPTS})
    target_include_directories(${TRAGET_NAME} PUBLIC ..)

    if (${OpenMP_CXX_FOUND})
        target_link_libraries(${TRAGET_NAME} PUBLIC Threads::Threads OpenMP::OpenMP_CXX )
    else()
        target_link_libraries(${TRAGET_NAME} PUBLIC Threads::Threads)
    endif ()

endforeach(EXAMPLE_SRC)
