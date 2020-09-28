Read and Write VTK to/from CGALMesh
-----------------------------------

python drastically simplifies the reading and writing process,

.. code-block:: python

   import CGALMethods as CM

   surface_mesh = CM.SurfaceMesh("resources/WALL_400.vtk")
   surface_mesh.write_vtk("resources/WALL_400_python_write.vtk")
   print("surface mesh __repr__:\n", surface_mesh)

The repr of the mesh shows the mesh details such as the properties and the number of vertices, halfeges, edges and faces.