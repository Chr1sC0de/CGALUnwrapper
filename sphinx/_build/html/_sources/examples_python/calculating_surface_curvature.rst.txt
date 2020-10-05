Calculating Surface Curvature
-----------------------------

The surface curvature can be calculated directly from the mesh,

.. code-block:: Python

   import CGALMethods as CM

   surface_mesh = CM.SurfaceMesh("resources/WALL_400.vtk", scale=1000)
   print("Surface mesh prior curvature calculation", surface_mesh)
   surface_mesh.calculate_curvature(4, 4, 300, True)
   surface_mesh.write_vtk("resources/WALL_400_python_write.vtk")
   print("Surface mesh after curvature calculation", surface_mesh)

Notice how a scale can be applied. The scale multiplies the units of the mesh vertices.
The data is saved to the mesh as property which can be accessed directly by calling the `get_property` property method