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
CMAKE_SOURCE_DIR = "/home/aman/Documents/networkLab/github/tic tac toe"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/aman/Documents/networkLab/github/tic tac toe/build"

# Include any dependencies generated for this target.
include CMakeFiles/server.out.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.out.dir/flags.make

CMakeFiles/server.out.dir/game_server.c.o: CMakeFiles/server.out.dir/flags.make
CMakeFiles/server.out.dir/game_server.c.o: ../game_server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/aman/Documents/networkLab/github/tic tac toe/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/server.out.dir/game_server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.out.dir/game_server.c.o   -c "/home/aman/Documents/networkLab/github/tic tac toe/game_server.c"

CMakeFiles/server.out.dir/game_server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.out.dir/game_server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/aman/Documents/networkLab/github/tic tac toe/game_server.c" > CMakeFiles/server.out.dir/game_server.c.i

CMakeFiles/server.out.dir/game_server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.out.dir/game_server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/aman/Documents/networkLab/github/tic tac toe/game_server.c" -o CMakeFiles/server.out.dir/game_server.c.s

CMakeFiles/server.out.dir/game_server.c.o.requires:

.PHONY : CMakeFiles/server.out.dir/game_server.c.o.requires

CMakeFiles/server.out.dir/game_server.c.o.provides: CMakeFiles/server.out.dir/game_server.c.o.requires
	$(MAKE) -f CMakeFiles/server.out.dir/build.make CMakeFiles/server.out.dir/game_server.c.o.provides.build
.PHONY : CMakeFiles/server.out.dir/game_server.c.o.provides

CMakeFiles/server.out.dir/game_server.c.o.provides.build: CMakeFiles/server.out.dir/game_server.c.o


# Object files for target server.out
server_out_OBJECTS = \
"CMakeFiles/server.out.dir/game_server.c.o"

# External object files for target server.out
server_out_EXTERNAL_OBJECTS =

server.out: CMakeFiles/server.out.dir/game_server.c.o
server.out: CMakeFiles/server.out.dir/build.make
server.out: CMakeFiles/server.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/aman/Documents/networkLab/github/tic tac toe/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable server.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.out.dir/build: server.out

.PHONY : CMakeFiles/server.out.dir/build

CMakeFiles/server.out.dir/requires: CMakeFiles/server.out.dir/game_server.c.o.requires

.PHONY : CMakeFiles/server.out.dir/requires

CMakeFiles/server.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.out.dir/clean

CMakeFiles/server.out.dir/depend:
	cd "/home/aman/Documents/networkLab/github/tic tac toe/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/aman/Documents/networkLab/github/tic tac toe" "/home/aman/Documents/networkLab/github/tic tac toe" "/home/aman/Documents/networkLab/github/tic tac toe/build" "/home/aman/Documents/networkLab/github/tic tac toe/build" "/home/aman/Documents/networkLab/github/tic tac toe/build/CMakeFiles/server.out.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/server.out.dir/depend

