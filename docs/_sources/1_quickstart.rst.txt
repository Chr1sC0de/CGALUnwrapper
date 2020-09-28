##########
Quickstart
##########

The code was developed to unwrap the coronary artery such that
a neural network is capable of predicting time averaged information along the wall. Because
of the neccessity for speed the c++ computational algorithms library CGAL was used, https://www.cgal.org/.

Nevertheless,  when using solely c++, further code development for a data parsing pipeline can be quite time consuming.
Python provides an abundance of functions which can be used for the data pipeling, also, as there is an abundance of machine learning
code available in pyton, a c++ python interface can be extremely useful for speeding up development

the CGAL methods have been bound using
`pybind11 <https://pybind11.readthedocs.io/en/stable/classes.html>`_ for ease of interaface between python.
Pybind11 provides convenient methods for a near seamless interface with pythons numpy library which has allowed us to minimize
time in python for time intensive routines.


Python Installation
-------------------

To install the python bound modules, in the top directory


.. code-block:: Powershell

    python -m pip install .


in development mode


.. code-block:: Powershell

    python -m pip install -e .


Compiling code
--------------

Before being able to compile the code the following template libraries must be added to the users path

- `CGAL <https://www.cgal.org/>`_
- `Boost <https://www.boost.org/>`_
- `Eigen <http://eigen.tuxfamily.org/index.php?title=Main_Page>`_
- `VTK <https://vtk.org/>`_


The code has currently been compiled for release for visual studio 2019 x64.
For alternative devices the code can be re-compiled using `cmake <https://cmake.org/>`_.
Make sure to compile in release mode as there is a significant performance difference when compared to debug mode.

.. code-block:: Powershell

    cd ./build
    cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -H{top_directory} -B{build_directory} -G Ninja
    cmake --build {build_directory} --config Release --target all -- -j 10

Directions for compiling using visual studio code can be found at `<https://code.visualstudio.com/>`_.