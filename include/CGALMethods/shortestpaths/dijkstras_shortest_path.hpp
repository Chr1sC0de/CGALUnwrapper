#pragma once
#include <CGALMethods/types.hpp>
#include <CGAL/boost/graph/dijkstra_shortest_paths.h>


namespace CGALMethods {
namespace shortestpaths{
namespace dijkstra{

    namespace {
        using namespace CGALMethods::types;
    }

    // -------------------------------------------------------------------------------------
    PairUmapdoubleVertexindex_3 shortest_distance_and_predecessor_umaps_from_source(
        Mesh_3 & mesh, const Vertexindex_3 source
    ){
        PairUmapdoubleVertexindex_3 pair_umap_double_vertex_index;

        VertexIdPropertyMap_3 vertex_index_property_map = get(boost::vertex_index, mesh);

        VectorVertexindex_3 vector_predecessor_indexes(mesh.num_vertices());
        VectorDouble vector_distances(mesh.num_vertices());

        IteratorPmapVertexindex_3 predecessor_pmap(
            vector_predecessor_indexes.begin(), vertex_index_property_map
        );
        IteratorPmapdouble_3 distance_pmap(
            vector_distances.begin(), vertex_index_property_map
        );

        boost::dijkstra_shortest_paths(
            mesh, source, distance_map(distance_pmap).predecessor_map(predecessor_pmap)
        );

        UmapVertexindexDouble_3 distance_umap;
        UmapVertexindex_3 predecessor_umap;

        for (Vertexindex_3 vd : mesh.vertices()){
            distance_umap[vd] = get(distance_pmap, vd);
            predecessor_umap[vd] = get(predecessor_pmap, vd);
        }

        pair_umap_double_vertex_index.first = distance_umap;
        pair_umap_double_vertex_index.second = predecessor_umap;

        return pair_umap_double_vertex_index;
    }
    // -------------------------------------------------------------------------------------
    VectorVertexindex_3 shortest_path_between_two_vertices(
        UmapVertexindex_3 & predecessor_umap,
        const Vertexindex_3 start_index, const Vertexindex_3 end_index
    ){

        /*
            when calcualting the shortest distance between two vertices with a predecessor umap
            the predecessor umap must be calculated using the start index as the source !!!!!!!!!!
        */

        VectorVertexindex_3 vector_vertex_path;
        Vertexindex_3 query_vertex = end_index;

        while (query_vertex != start_index){
            vector_vertex_path.insert(vector_vertex_path.begin(), query_vertex);
            query_vertex = predecessor_umap[query_vertex];
        }

        vector_vertex_path.insert(vector_vertex_path.begin(), query_vertex);

        return vector_vertex_path;
    }

    // -------------------------------------------------------------------------------------

    VectorVertexindex_3 shortest_path_between_two_vertices(
        Mesh_3 & mesh, Vertexindex_3 start_index, Vertexindex_3 end_index
    ){
        PairUmapdoubleVertexindex_3 pair_umap_double_vertex_index =
            shortest_distance_and_predecessor_umaps_from_source(mesh, start_index);

        UmapVertexindex_3 predecessor_umap = pair_umap_double_vertex_index.second;

        return shortest_path_between_two_vertices(
            predecessor_umap, start_index, end_index
        );

    }

}
}
}
