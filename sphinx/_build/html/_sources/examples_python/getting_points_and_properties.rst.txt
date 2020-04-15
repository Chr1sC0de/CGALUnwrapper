Accessing Points and Properties as Aumpy Arrays
-----------------------------------------------

Points and properties can be accessed directly from the CGALMesh as numpy arrays

.. code-block:: Python

   import CGALMethods as CM
   import numpy as np

   if __name__ == "__main__":
      surface_mesh = CM.SurfaceMesh("resources/WALL_400.vtk")
      print("the surfacemesh points", surface_mesh.points().shape)
      for name in surface_mesh.property_names():
         prop = surface_mesh.get_property(name)
         print(name, "has shape", prop.shape)

the points are accessed via the `points` method whilst the properties are accessed by individual names via
the `get_property` command