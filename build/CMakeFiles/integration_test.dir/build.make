# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nzengi/Desktop/skein3-AI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nzengi/Desktop/skein3-AI/build

# Include any dependencies generated for this target.
include CMakeFiles/integration_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/integration_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/integration_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/integration_test.dir/flags.make

CMakeFiles/integration_test.dir/examples/integration_test.cpp.o: CMakeFiles/integration_test.dir/flags.make
CMakeFiles/integration_test.dir/examples/integration_test.cpp.o: /home/nzengi/Desktop/skein3-AI/examples/integration_test.cpp
CMakeFiles/integration_test.dir/examples/integration_test.cpp.o: CMakeFiles/integration_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/nzengi/Desktop/skein3-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/integration_test.dir/examples/integration_test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/integration_test.dir/examples/integration_test.cpp.o -MF CMakeFiles/integration_test.dir/examples/integration_test.cpp.o.d -o CMakeFiles/integration_test.dir/examples/integration_test.cpp.o -c /home/nzengi/Desktop/skein3-AI/examples/integration_test.cpp

CMakeFiles/integration_test.dir/examples/integration_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/integration_test.dir/examples/integration_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nzengi/Desktop/skein3-AI/examples/integration_test.cpp > CMakeFiles/integration_test.dir/examples/integration_test.cpp.i

CMakeFiles/integration_test.dir/examples/integration_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/integration_test.dir/examples/integration_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nzengi/Desktop/skein3-AI/examples/integration_test.cpp -o CMakeFiles/integration_test.dir/examples/integration_test.cpp.s

# Object files for target integration_test
integration_test_OBJECTS = \
"CMakeFiles/integration_test.dir/examples/integration_test.cpp.o"

# External object files for target integration_test
integration_test_EXTERNAL_OBJECTS =

integration_test: CMakeFiles/integration_test.dir/examples/integration_test.cpp.o
integration_test: CMakeFiles/integration_test.dir/build.make
integration_test: libskein3_lib.a
integration_test: /usr/lib/x86_64-linux-gnu/libssl.so
integration_test: /usr/lib/x86_64-linux-gnu/libcrypto.so
integration_test: CMakeFiles/integration_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/nzengi/Desktop/skein3-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable integration_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/integration_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/integration_test.dir/build: integration_test
.PHONY : CMakeFiles/integration_test.dir/build

CMakeFiles/integration_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/integration_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/integration_test.dir/clean

CMakeFiles/integration_test.dir/depend:
	cd /home/nzengi/Desktop/skein3-AI/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nzengi/Desktop/skein3-AI /home/nzengi/Desktop/skein3-AI /home/nzengi/Desktop/skein3-AI/build /home/nzengi/Desktop/skein3-AI/build /home/nzengi/Desktop/skein3-AI/build/CMakeFiles/integration_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/integration_test.dir/depend

