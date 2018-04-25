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

cmake_minimum_required(VERSION 3.9)
project(GridGraph)

#GLOBALS
set(CMAKE_CXX_STANDARD 14)
set(OPTS -march=native -Wall -Wextra  -Wno-unused-parameter -O3)
file(GLOB CORE_HEADERS "core/*.hpp")
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)

option(BUILD_EXAMPLES "build examples" ON)

add_subdirectory(tools)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-DLINUX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_definitions(-DDARWIN)
endif ()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()