# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/yehorfur/Desktop/smptIMAPcl/smtp-client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build

# Include any dependencies generated for this target.
include libs/SmartSocket/CMakeFiles/SmartSocket.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libs/SmartSocket/CMakeFiles/SmartSocket.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/SmartSocket/CMakeFiles/SmartSocket.dir/progress.make

# Include the compile flags for this target's objects.
include libs/SmartSocket/CMakeFiles/SmartSocket.dir/flags.make

libs/SmartSocket/CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o: libs/SmartSocket/CMakeFiles/SmartSocket.dir/flags.make
libs/SmartSocket/CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o: /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/SmartSocket/src/SmartSocket.cpp
libs/SmartSocket/CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o: libs/SmartSocket/CMakeFiles/SmartSocket.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/SmartSocket/CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o"
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/SmartSocket && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/SmartSocket/CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o -MF CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o.d -o CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o -c /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/SmartSocket/src/SmartSocket.cpp

libs/SmartSocket/CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.i"
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/SmartSocket && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/SmartSocket/src/SmartSocket.cpp > CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.i

libs/SmartSocket/CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.s"
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/SmartSocket && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/SmartSocket/src/SmartSocket.cpp -o CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.s

# Object files for target SmartSocket
SmartSocket_OBJECTS = \
"CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o"

# External object files for target SmartSocket
SmartSocket_EXTERNAL_OBJECTS =

libs/SmartSocket/libSmartSocket.a: libs/SmartSocket/CMakeFiles/SmartSocket.dir/src/SmartSocket.cpp.o
libs/SmartSocket/libSmartSocket.a: libs/SmartSocket/CMakeFiles/SmartSocket.dir/build.make
libs/SmartSocket/libSmartSocket.a: libs/SmartSocket/CMakeFiles/SmartSocket.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libSmartSocket.a"
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/SmartSocket && $(CMAKE_COMMAND) -P CMakeFiles/SmartSocket.dir/cmake_clean_target.cmake
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/SmartSocket && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SmartSocket.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/SmartSocket/CMakeFiles/SmartSocket.dir/build: libs/SmartSocket/libSmartSocket.a
.PHONY : libs/SmartSocket/CMakeFiles/SmartSocket.dir/build

libs/SmartSocket/CMakeFiles/SmartSocket.dir/clean:
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/SmartSocket && $(CMAKE_COMMAND) -P CMakeFiles/SmartSocket.dir/cmake_clean.cmake
.PHONY : libs/SmartSocket/CMakeFiles/SmartSocket.dir/clean

libs/SmartSocket/CMakeFiles/SmartSocket.dir/depend:
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yehorfur/Desktop/smptIMAPcl/smtp-client /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/SmartSocket /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/SmartSocket /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/SmartSocket/CMakeFiles/SmartSocket.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : libs/SmartSocket/CMakeFiles/SmartSocket.dir/depend

