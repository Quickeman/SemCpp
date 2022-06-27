# SemCpp
A C++ interface for using POSIX semaphores

This is a simple C++ wrapper/interface for creating and manipulating POSIX semaphores.

Requires C++11 or later.


## Including SemCpp in your Project
### CMake-based projects

Simply add:
```
add_subdirectory(<SemCpp-root-dir>)
include_directories(<SemCpp-root-dir>/include)
target_link_libraries(<your-compile-target> semcpp)
```
to your `CMakeLists.txt` file.
