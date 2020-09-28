# ArteryUnwrapper

CGAL wrapped code for unwrapping an artery. The code has currently been compiled for
release for visual studio 2019 x64. The version of pybind11 used only works with
python versions 3.7.x

## Installing Necessary C++ Libraries

If you find that the code is not working you may have to recompile the code.

The core libraries needed to re-compile the code:

1. [CGAL==5.0.2](https://www.cgal.org/).
2. [boost==1.71.0](https://www.boost.org/)
3. [Eigen==3.3.7](http://eigen.tuxfamily.org/index.php?title=Main_Page)
4. [VTK==8.0.2](https://vtk.org/)

The code was wrapped with the pybind11. The version used is included.

Generally I like to arrange my development folder using the following layout to help
with versioning.

````folders
.
|- modulename
   |- 1.0.0
      |- folder1
        |- folder1
          ...
      |- folder2
      |- folder3
        ...
   |- 1.1.0
      ...
...
````

My development folder for the current repo looks like the following,

````folders
.
|- CGAL
   |- 5.0.2
|- boost
   |- 1.71.0
|- Eigen
   |- 3.3.7
|- VTK
   |- 8.0.2

````

The CGAL windows installer can be found on the following release page [release
page](https://github.com/CGAL/cgal/releases). You may need to add the gmp library
`<CGALDev>/auxillary/gmp/lib` to your
[path](https://www.techjunkie.com/environment-variables-windows-10/) (link has been
included for windows users).

Historical versions of boost can be obtained from the [boost version
history](https://www.boost.org/users/history/) page. So that CMake can locate the
necessary folders the `BOOST_INCLUDEDIR=<boostDev>` and
`BOOST_LIBRARYDIR=<boostDev>/libs` environment variables must be set,

Versions of Eigen can be directly downloaded from the [home
page](http://eigen.tuxfamily.org/index.php?title=Main_Page). For Eigen to be visible
to CMake we require the `EIGEN3_INCLUDE_DIR=<eigenDev>` to be set.

VTK releases can be found on the VTK [downloads page](https://vtk.org/download/). The
following video shows how to [compile VTK with Visual
Studio](https://www.youtube.com/watch?v=IgvbhyDh8r0) for windows. For VTK to be
visible to CMake we require `VTK_DIR=<vtkBuild>` to be set, where `<vtkBuild>` is the
location of the location of the folder which contains the `VTKConfig.cmake` file.

## Compiling Code

For alternative devices the code can be re-compiled using CMake https://cmake.org/.
Make sure to compile in release mode as there is a significant performance difference
when compared to debug mode.

Example compile commands,

````powershell
// in the top directory
cd ./build
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -H{top_directory} -B{build_directory} -G Ninja
cmake --build {build_directory} --config Release --target all -- -j 10
````

Directions for compiling using visual studio code, https://code.visualstudio.com/, is
provided in the ./readme/vsc_install.pdf.

## Installing pymethods

The module depends on the pymethods library.

## Examples

Examples for running the code can be found in the ./examples directory