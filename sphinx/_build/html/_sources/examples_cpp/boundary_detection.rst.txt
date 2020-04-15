Boundary Detection
------------------

When analyzing a surface mesh it may be of interest to extract the vertices and the halfedges along
the boundaries,

.. code-block:: cpp

   #include <CGALMethods/boundary.hpp>
   #include <CGALMethods/IO/vtk.hpp>

   using namespace CGALMethods;


   int main() {
   std::string path("resources/WALL_400.vtk");

   types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);
   types::Mesh_3 mesh = mesh_n_property_meta_data.first;
   types::VectorPairString property_meta_data = mesh_n_property_meta_data.second;

   ...

to extract all the boundaries,

.. code-block:: cpp

   // to get the halfedges on the boundary
   types::VectorHalfedgeindex_3 boundary_halfedges = boundary::get_boundary_halfedges(mesh);


to divide halfedges into connnected components,

.. code-block:: cpp

   // from the boundary halfedges obtain sets of connected halfedges
   types::NestedVectorHalfedgeindex_3 connected_boundary_halfedges =
      boundary::get_connected_boundary_halfedges(mesh, boundary_halfedges);

to extract the halfedges without knowing the boundary_halfedges beforehand,

.. code-block:: cpp

   // obtain the connected halfedges without knowing the boundary edges
   types::NestedVectorHalfedgeindex_3 connected_boundary_halfedges_no_boundary_edges =
      boundary::get_connected_boundary_halfedges(mesh);

we can also extract the vertices from each halfedge using the source method of the mesh

.. code-block:: cpp

   int i = 0;
   Vertexindex_3 vertex = mesh.source(boundary_halfedges[i])

we can extract all the boundary vertices from the boundary halfedges,

.. code-block:: cpp

   // obtain the vertices along the boundary from the halfedges
   types:: VectorVertexindex_3 boundary_vertices_from_halfedges =  boundary::get_boundary_vertices(mesh, boundary_halfedges);

to do so without explicitly calculating the halfedges

.. code-block:: cpp

   // obtain the vertices along the boundary from the halfedges
   types:: VectorVertexindex_3 boundary_vertices =  boundary::get_boundary_vertices(mesh);

to obtain the boundary vertices connected by halfedges,

.. code-block:: cpp

   // to obtain the boundary vertices connected by halfedges
   types:: NestedVectorVertexindex_3 connected_boundary_vertices =
      boundary::get_connected_boundary_vertices(mesh, connected_boundary_halfedges);

without explicitly providing the connected halfedges

.. code-block:: cpp

   //without explicitly calculating the connected boundary halfedges
   connected_boundary_vertices =  boundary::get_connected_boundary_vertices(mesh);