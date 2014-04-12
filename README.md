libusbk-examples
================

Welcome to the libusbk examples that demonstrate the use of the excellent 
libusbk library available at http://libusbk.sourceforge.net/UsbK3/index.html.  
The purpose of this small project is to help isolate and identify unexpected 
behaviour between libusbk and custom application software under development.

This project is intended for MS Windows XP through MS Windows 8.1 and may
be compiled with MinGW or Visual Studio 2013.  

The project requires the following:
* cmake 2.8+: http://www.cmake.org/
* Visual Studio 2013 or MinGW http://www.mingw.org/
* libusbk: https://sourceforge.net/projects/libusbk/

To create an out-of-source build for Visual Studio 2013:

    mkdir [build_dir]
    cd [build_dir]
    cmake -G "Visual Studio 12 Win64" -DLIBUSBK_DIR=[libusbk_bin_dir] [repo_dir]

For example:

    C:\build\libusbk_examples>cmake -G "Visual Studio 12 Win64" -DLIBUSBK_DIR=C:\dev\libusbK-3.0.6.0-bin c:\mgl\libusbk_examples

The project is released under the permissive BSD license.
