# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.20.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.20.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yangyang/Desktop/C++项目/YJson

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yangyang/Desktop/C++项目/YJson/build

# Include any dependencies generated for this target.
include CMakeFiles/yjson.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/yjson.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/yjson.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/yjson.dir/flags.make

CMakeFiles/yjson.dir/yjson.c.o: CMakeFiles/yjson.dir/flags.make
CMakeFiles/yjson.dir/yjson.c.o: ../yjson.c
CMakeFiles/yjson.dir/yjson.c.o: CMakeFiles/yjson.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yangyang/Desktop/C++项目/YJson/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/yjson.dir/yjson.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/yjson.dir/yjson.c.o -MF CMakeFiles/yjson.dir/yjson.c.o.d -o CMakeFiles/yjson.dir/yjson.c.o -c /Users/yangyang/Desktop/C++项目/YJson/yjson.c

CMakeFiles/yjson.dir/yjson.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/yjson.dir/yjson.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/yangyang/Desktop/C++项目/YJson/yjson.c > CMakeFiles/yjson.dir/yjson.c.i

CMakeFiles/yjson.dir/yjson.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/yjson.dir/yjson.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/yangyang/Desktop/C++项目/YJson/yjson.c -o CMakeFiles/yjson.dir/yjson.c.s

# Object files for target yjson
yjson_OBJECTS = \
"CMakeFiles/yjson.dir/yjson.c.o"

# External object files for target yjson
yjson_EXTERNAL_OBJECTS =

libyjson.a: CMakeFiles/yjson.dir/yjson.c.o
libyjson.a: CMakeFiles/yjson.dir/build.make
libyjson.a: CMakeFiles/yjson.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yangyang/Desktop/C++项目/YJson/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libyjson.a"
	$(CMAKE_COMMAND) -P CMakeFiles/yjson.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/yjson.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/yjson.dir/build: libyjson.a
.PHONY : CMakeFiles/yjson.dir/build

CMakeFiles/yjson.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/yjson.dir/cmake_clean.cmake
.PHONY : CMakeFiles/yjson.dir/clean

CMakeFiles/yjson.dir/depend:
	cd /Users/yangyang/Desktop/C++项目/YJson/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yangyang/Desktop/C++项目/YJson /Users/yangyang/Desktop/C++项目/YJson /Users/yangyang/Desktop/C++项目/YJson/build /Users/yangyang/Desktop/C++项目/YJson/build /Users/yangyang/Desktop/C++项目/YJson/build/CMakeFiles/yjson.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/yjson.dir/depend

