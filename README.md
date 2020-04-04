# ArteryUnwrapper

CGAL wrapped code for unwrapping an artery. The code has currently been compiled for release for visual studio 2019 x64

## Compiling Code
For alternative devices the code can be re-compiled using cmake https://cmake.org/. Make sure to compile in release mode as there is a significant performance difference when compared to debug mode.

Example compile commands,

````
// in the top directory
cd ./build
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -H{top_directory} -B{build_directory} -G Ninja
cmake --build {build_directory} --config Release --target all -- -j 10
````

Directions for compiling using visual studio code, https://code.visualstudio.com/, is provided in the ./readme/vsc_install.pdf.

## Installing pymethods
The module depends on the pymethods library "link here"

## Examples
Examples for running the code can be found in the ./examples directory


