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
include libs/Base64/CMakeFiles/Base64.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libs/Base64/CMakeFiles/Base64.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/Base64/CMakeFiles/Base64.dir/progress.make

# Include the compile flags for this target's objects.
include libs/Base64/CMakeFiles/Base64.dir/flags.make

libs/Base64/CMakeFiles/Base64.dir/src/Base64.cpp.o: libs/Base64/CMakeFiles/Base64.dir/flags.make
libs/Base64/CMakeFiles/Base64.dir/src/Base64.cpp.o: /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/Base64/src/Base64.cpp
libs/Base64/CMakeFiles/Base64.dir/src/Base64.cpp.o: libs/Base64/CMakeFiles/Base64.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/Base64/CMakeFiles/Base64.dir/src/Base64.cpp.o"
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/Base64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Base64/CMakeFiles/Base64.dir/src/Base64.cpp.o -MF CMakeFiles/Base64.dir/src/Base64.cpp.o.d -o CMakeFiles/Base64.dir/src/Base64.cpp.o -c /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/Base64/src/Base64.cpp

libs/Base64/CMakeFiles/Base64.dir/src/Base64.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Base64.dir/src/Base64.cpp.i"
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/Base64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/Base64/src/Base64.cpp > CMakeFiles/Base64.dir/src/Base64.cpp.i

libs/Base64/CMakeFiles/Base64.dir/src/Base64.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Base64.dir/src/Base64.cpp.s"
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/Base64 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/Base64/src/Base64.cpp -o CMakeFiles/Base64.dir/src/Base64.cpp.s

# Object files for target Base64
Base64_OBJECTS = \
"CMakeFiles/Base64.dir/src/Base64.cpp.o"

# External object files for target Base64
Base64_EXTERNAL_OBJECTS =

libs/Base64/libBase64.a: libs/Base64/CMakeFiles/Base64.dir/src/Base64.cpp.o
libs/Base64/libBase64.a: libs/Base64/CMakeFiles/Base64.dir/build.make
libs/Base64/libBase64.a: libs/Base64/CMakeFiles/Base64.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libBase64.a"
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/Base64 && $(CMAKE_COMMAND) -P CMakeFiles/Base64.dir/cmake_clean_target.cmake
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/Base64 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Base64.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/Base64/CMakeFiles/Base64.dir/build: libs/Base64/libBase64.a
.PHONY : libs/Base64/CMakeFiles/Base64.dir/build

libs/Base64/CMakeFiles/Base64.dir/clean:
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/Base64 && $(CMAKE_COMMAND) -P CMakeFiles/Base64.dir/cmake_clean.cmake
.PHONY : libs/Base64/CMakeFiles/Base64.dir/clean

libs/Base64/CMakeFiles/Base64.dir/depend:
	cd /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yehorfur/Desktop/smptIMAPcl/smtp-client /home/yehorfur/Desktop/smptIMAPcl/smtp-client/libs/Base64 /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/Base64 /home/yehorfur/Desktop/smptIMAPcl/smtp-client/build/libs/Base64/CMakeFiles/Base64.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : libs/Base64/CMakeFiles/Base64.dir/depend

