# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/hchc/projects/GitFile/jmuduo/Pratice_muduo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hchc/projects/GitFile/jmuduo/build/release-cpp11

# Include any dependencies generated for this target.
include tests/CMakeFiles/Log_test2.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/Log_test2.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/Log_test2.dir/flags.make

tests/CMakeFiles/Log_test2.dir/Log_test2.cc.o: tests/CMakeFiles/Log_test2.dir/flags.make
tests/CMakeFiles/Log_test2.dir/Log_test2.cc.o: /home/hchc/projects/GitFile/jmuduo/Pratice_muduo/tests/Log_test2.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hchc/projects/GitFile/jmuduo/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/Log_test2.dir/Log_test2.cc.o"
	cd /home/hchc/projects/GitFile/jmuduo/build/release-cpp11/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Log_test2.dir/Log_test2.cc.o -c /home/hchc/projects/GitFile/jmuduo/Pratice_muduo/tests/Log_test2.cc

tests/CMakeFiles/Log_test2.dir/Log_test2.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Log_test2.dir/Log_test2.cc.i"
	cd /home/hchc/projects/GitFile/jmuduo/build/release-cpp11/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hchc/projects/GitFile/jmuduo/Pratice_muduo/tests/Log_test2.cc > CMakeFiles/Log_test2.dir/Log_test2.cc.i

tests/CMakeFiles/Log_test2.dir/Log_test2.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Log_test2.dir/Log_test2.cc.s"
	cd /home/hchc/projects/GitFile/jmuduo/build/release-cpp11/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hchc/projects/GitFile/jmuduo/Pratice_muduo/tests/Log_test2.cc -o CMakeFiles/Log_test2.dir/Log_test2.cc.s

# Object files for target Log_test2
Log_test2_OBJECTS = \
"CMakeFiles/Log_test2.dir/Log_test2.cc.o"

# External object files for target Log_test2
Log_test2_EXTERNAL_OBJECTS =

bin/Log_test2: tests/CMakeFiles/Log_test2.dir/Log_test2.cc.o
bin/Log_test2: tests/CMakeFiles/Log_test2.dir/build.make
bin/Log_test2: lib/libmuduo_base.a
bin/Log_test2: tests/CMakeFiles/Log_test2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hchc/projects/GitFile/jmuduo/build/release-cpp11/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/Log_test2"
	cd /home/hchc/projects/GitFile/jmuduo/build/release-cpp11/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Log_test2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/Log_test2.dir/build: bin/Log_test2

.PHONY : tests/CMakeFiles/Log_test2.dir/build

tests/CMakeFiles/Log_test2.dir/clean:
	cd /home/hchc/projects/GitFile/jmuduo/build/release-cpp11/tests && $(CMAKE_COMMAND) -P CMakeFiles/Log_test2.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/Log_test2.dir/clean

tests/CMakeFiles/Log_test2.dir/depend:
	cd /home/hchc/projects/GitFile/jmuduo/build/release-cpp11 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hchc/projects/GitFile/jmuduo/Pratice_muduo /home/hchc/projects/GitFile/jmuduo/Pratice_muduo/tests /home/hchc/projects/GitFile/jmuduo/build/release-cpp11 /home/hchc/projects/GitFile/jmuduo/build/release-cpp11/tests /home/hchc/projects/GitFile/jmuduo/build/release-cpp11/tests/CMakeFiles/Log_test2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/Log_test2.dir/depend

