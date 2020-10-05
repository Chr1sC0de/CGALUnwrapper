Dijkstra's Shortest Path
------------------------

Finding the shortest distance along a mesh between the vertices from a source vertex can be performed using dijkstra's shortest
path algorithm.

.. code-block:: cpp

   #include <CGALMethods/IO/vtk.hpp>
   #include <CGALMethods/shortestpaths/dijkstras_shortest_path.hpp>

   using namespace CGALMethods;
   using namespace shortestpaths;

   int main(){

      std::string path("resources/WALL_400.vtk");

      types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);

      types::Mesh_3 mesh = mesh_n_property_meta_data.first;

      types::VectorPairString property_meta_data = mesh_n_property_meta_data.second;

      types::Vertexindex_3 source(0);
      types::Vertexindex_3 sink(79000);

      types::PairUmapdoubleVertexindex_3 distance_predecessor_map_from_source =
         dijkstra::shortest_distance_and_predecessor_umaps_from_source(mesh, source);

      types::VectorVertexindex_3 shortest_path_between_source_and_sink_from_distance_predecessor_map =
         dijkstra::shortest_path_between_two_vertices(
               mesh, predecessor_umap, source, sink
         );

      // alternatively, without accessing the distance and predecessor map

      types::VectorVertexindex_3 shortest_path_between_source_and_sink =
         dijkstra::shortest_path_between_two_vertices(
            mesh, source, sink
         );

   ...

The method `shortest_distance_and_predecessor_umaps_from_source` returns templated
`boost::unordered_map <https://www.boost.org/doc/libs/1_72_0/doc/html/unordered.html>`_
of the distance of the vertices from the source vertex as well as predecessor vertices
of the shortest path,

.. code-block:: cpp

      // the shortest distance map
      types::UmapVertexindexDouble_3 distance_umap = distance_predecessor_map_from_source.first;
      //for the following vertex distance map
      types::UmapVertexindex_3 predecessor_umap = distance_predecessor_map_from_source.second;


to extract the shortest path to any vertex from our source through the 'predecessor_umap' select a query vertex and
loop through the predecessor vertices,

.. code-block:: cpp

   VectorVertexindex_3 shortest_path;
   VectorVertexindex_3 query_vertex;

   while (query_vertex != source){
      vector_vertex_path.insert(query_vertex);
      query_vertex = predecessor_umap[query_vertex];
   }
   vector_vertex_path.insert(query_vertex);

accessing the shortest path from the unordered_map can also be performed via the `shortest_path_between_two_vertices` function
by feeding in a pre-calculated `predecessor_umap`

.. code-block:: cpp

   // accessing the shortest path from the pre-calulcated predecessor_umap
   types::VectorVertexindex_3 shortest_path_between_source_and_sink_from_distance_predecessor_map =
      dijkstra::shortest_path_between_two_vertices(
            mesh, predecessor_umap, source, sink
      );

Alternatively if the shortest path is desired directly, the process can be perfomed
via the 'shortest_path_between_two_vertices' by ommiting the predecessor map,

.. code-block:: cpp

   // alternatively, the distance and predecessor_umap is made internally with the following overloaded method
   types::VectorVertexindex_3 shortest_path_between_source_and_sink =
      dijkstra::shortest_path_between_two_vertices(mesh, source, sink);