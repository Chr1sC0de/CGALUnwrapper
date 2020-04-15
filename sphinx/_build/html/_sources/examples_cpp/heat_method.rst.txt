Heat Method
-----------

The heat method estimates the shortest distance along a manifold of all vertices to a set of source vertices.

.. code-block:: cpp

   #include <CGALMethods/IO/vtk.hpp>
   #include <CGALMethods/shortestpaths/heatmethod.hpp>
   #include <CGALMethods/shortestpaths/dijkstras_shortest_path.hpp>
   #include <CGALMethods/utils.hpp>

   using namespace CGALMethods;
   using namespace shortestpaths;

   int main() {

      std::string path("resources/WALL_400.vtk");

      types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);
      types::Mesh_3 mesh = mesh_n_property_meta_data.first;
      types::VectorPairString property_meta_data = mesh_n_property_meta_data.second;

   ...

once the mesh has been loaded to calculate the shortest distance from a set of vertices,

.. code-block:: cpp

   // find the distances from a set of sources

   std::string path_write("resources/WALL_400_write_heatmethod_sources.vtk");

   types::VectorVertexindex_3 sources_0;

   for (int i=0; i<1000; i++){
      sources_0.push_back(types::Vertexindex_3(i));
   }

   types::PairString heat_meta_0 = heatmethod::map_vertex_distances_to_sources(mesh, sources_0, "v:heat_method");

   property_meta_data.push_back(heat_meta_0);

   IO::VTK::write(mesh, path_write, property_meta_data);

if we deisre the two closest points between two sets of sources

.. code-block:: cpp

   // find the closest vertices between two sets of sources

   types::VectorVertexindex_3 sources_1;
   types::VectorVertexindex_3 sources_2;

   for (int i=0; i<100; i++){
      sources_1.push_back(types::Vertexindex_3(i));
      sources_2.push_back(types::Vertexindex_3((i+5)*100));

   }

   types::TupleVertexVertexUmapvertex_3 vertices_and_predecessor_maps =
      heatmethod::closest_vertex_pair_between_sources(
         mesh, sources_1, sources_2
      );

   types::Vertexindex_3 source_1_vertex = std::get<0>(vertices_and_predecessor_maps);
   types::Vertexindex_3 source_2_vertex = std::get<1>(vertices_and_predecessor_maps);
   types::UmapVertexindex_3 predecessor_map = std::get<2>(vertices_and_predecessor_maps);

If we desire the shortest path between the two sources we can query the `predecessor_map`.
Alternatively the `heatmethod::shortest_path_between_source_pair` method can extract the
path directly from the tuple of the vertices and predecessor_map,

.. code-block:: cpp

   // if a vertex and predecessor map has been pre-calculated get the shortest path between the two vertices

   types::VectorVertexindex_3 shortest_path_between_sources_with_map =
      heatmethod::shortest_path_between_source_pair(
         vertices_and_predecessor_maps
      );

to directoy obtain the shortest path between the two sources the mesh and source sets can be passed in
as an alternative,

.. code-block:: cpp

   // if the verex and predecessor map has not been calculate
   types::VectorVertexindex_3 shortest_path_between_sources_no_map =
      heatmethod::shortest_path_between_source_pair(
         mesh, sources_1, sources_2
      );

