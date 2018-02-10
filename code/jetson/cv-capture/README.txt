Setting up the Makefile the first time you check out this directory.

Checkout the following 2 files:
code/jetson/cv-capture/CMakeLists.txt
code/jetson/cv-capture/capture.cpp

>cd to code/jetson/cv-capture/

>Create a build directory
mkdir build

>cd to the build directory
cd build

>run cmake and pass in the name of the directory above build
cmake ..

>Build will now have a Makefile in it.  Run make to build the code.
make


