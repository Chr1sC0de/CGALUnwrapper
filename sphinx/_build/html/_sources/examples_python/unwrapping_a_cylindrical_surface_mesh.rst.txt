Unwrapping a Cylindrical Surface Mesh
-------------------------------------

A cylindrical surface mesh can be directly unwrapped via the `unwrap_cylindrical_surface_mesh` method. The unwrapping occurs
by locating a seam within the cylindrical mesh that connects the inlet boundary to the outlet boundary. When the inlet boundary
is not specified it is assumed as the first boundary located by the program,

.. code-block:: Python

   import CGALMethods as CM

   surface_mesh = CM.SurfaceMesh("resources/WALL_400.vtk", scale=1000)
   print("the original mesh:\n", surface_mesh)
   unwrapped = CM.unwrap_cylindrical_surface_mesh(surface_mesh)
   unwrapped.write_vtk("resources/WALL_400_python_write.vtk")
   print("the unwrapped mesh:\n", unwrapped

To specify the inlet and outlet location we supply the origin of the inlet as some
form of 1d tuple, i.e. list, array etc,

.. code-block:: Python

   unwrapped_w_pt = CM.unwrap_cylindrical_surface_mesh(
      surface_mesh, *[0,0,-21], *[0,0,21])
   unwrapped_w_pt.write_vtk("resources/WALL_400_python_write.vtk")
   print("the unwrapped_w_pt mesh:\n", unwrapped)

