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

    types::UmapVertexindexDouble_3 distance_umap = distance_predecessor_map_from_source.first;
    types::UmapVertexindex_3 predecessor_umap = distance_predecessor_map_from_source.second;

    types::VectorVertexindex_3 shortest_path_between_source_and_sink_from_distance_predecessor_map =
        dijkstra::shortest_path_between_two_vertices(
            predecessor_umap, source, sink
        );

    // alternatively, the distance and predecessor_umap is calculated internally
    types::VectorVertexindex_3 shortest_path_between_source_and_sink = dijkstra::shortest_path_between_two_vertices(
        mesh, source, sink
    );

    return 0;

}