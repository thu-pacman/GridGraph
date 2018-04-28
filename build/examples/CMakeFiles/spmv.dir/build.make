# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/abdalwhab/S2/BigData/GridGraph/GridGraph

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/spmv.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/spmv.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/spmv.dir/flags.make

examples/CMakeFiles/spmv.dir/spmv.cpp.o: examples/CMakeFiles/spmv.dir/flags.make
examples/CMakeFiles/spmv.dir/spmv.cpp.o: ../examples/spmv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/spmv.dir/spmv.cpp.o"
	cd /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spmv.dir/spmv.cpp.o -c /home/abdalwhab/S2/BigData/GridGraph/GridGraph/examples/spmv.cpp

examples/CMakeFiles/spmv.dir/spmv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spmv.dir/spmv.cpp.i"
	cd /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/abdalwhab/S2/BigData/GridGraph/GridGraph/examples/spmv.cpp > CMakeFiles/spmv.dir/spmv.cpp.i

examples/CMakeFiles/spmv.dir/spmv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spmv.dir/spmv.cpp.s"
	cd /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/abdalwhab/S2/BigData/GridGraph/GridGraph/examples/spmv.cpp -o CMakeFiles/spmv.dir/spmv.cpp.s

examples/CMakeFiles/spmv.dir/spmv.cpp.o.requires:

.PHONY : examples/CMakeFiles/spmv.dir/spmv.cpp.o.requires

examples/CMakeFiles/spmv.dir/spmv.cpp.o.provides: examples/CMakeFiles/spmv.dir/spmv.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/spmv.dir/build.make examples/CMakeFiles/spmv.dir/spmv.cpp.o.provides.build
.PHONY : examples/CMakeFiles/spmv.dir/spmv.cpp.o.provides

examples/CMakeFiles/spmv.dir/spmv.cpp.o.provides.build: examples/CMakeFiles/spmv.dir/spmv.cpp.o


# Object files for target spmv
spmv_OBJECTS = \
"CMakeFiles/spmv.dir/spmv.cpp.o"

# External object files for target spmv
spmv_EXTERNAL_OBJECTS =

bin/spmv: examples/CMakeFiles/spmv.dir/spmv.cpp.o
bin/spmv: examples/CMakeFiles/spmv.dir/build.make
bin/spmv: /usr/lib/gcc/x86_64-linux-gnu/7/libgomp.so
bin/spmv: /usr/lib/x86_64-linux-gnu/libpthread.so
bin/spmv: examples/CMakeFiles/spmv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/spmv"
	cd /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spmv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/spmv.dir/build: bin/spmv

.PHONY : examples/CMakeFiles/spmv.dir/build

examples/CMakeFiles/spmv.dir/requires: examples/CMakeFiles/spmv.dir/spmv.cpp.o.requires

.PHONY : examples/CMakeFiles/spmv.dir/requires

examples/CMakeFiles/spmv.dir/clean:
	cd /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/spmv.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/spmv.dir/clean

examples/CMakeFiles/spmv.dir/depend:
	cd /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/abdalwhab/S2/BigData/GridGraph/GridGraph /home/abdalwhab/S2/BigData/GridGraph/GridGraph/examples /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/examples /home/abdalwhab/S2/BigData/GridGraph/GridGraph/build/examples/CMakeFiles/spmv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/spmv.dir/depend

