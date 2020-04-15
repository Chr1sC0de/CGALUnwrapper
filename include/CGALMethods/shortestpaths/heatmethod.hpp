# pragma once
#include <CGALMethods/shortestpaths/dijkstras_shortest_path.hpp>
#include <CGALMethods/IO/io.hpp>

#include <CGAL/Heat_method_3/Surface_mesh_geodesic_distances_3.h>

#include <limits>


namespace CGALMethods {
namespace shortestpaths{
namespace heatmethod {

    namespace {
        using namespace CGALMethods::types;
        using namespace std;
    }
    //-----------------------------------------------------------------------------------
    typedef CGAL::Heat_method_3::Surface_mesh_geodesic_distances_3<Mesh_3> HeatMethod;

    //-----------------------------------------------------------------------------------
    PairString map_vertex_distances_to_sources(
        Mesh_3 & mesh,
        HeatMethod & heat_object,
        const string map_name="v:heat_method",
        const bool clear_sources=true
    ){

        // for a heatmethod with sources calculate the vertex_distance_pmap

        MapVertexindexDouble_3 mesh_vertex_distance_pmap =
            mesh.add_property_map<Vertexindex_3, double>(map_name, 0).first;

        heat_object.estimate_geodesic_distances(mesh_vertex_distance_pmap);

        if (clear_sources) {
            heat_object.clear_sources();
        }

        return PairString(map_name, CGALMethods::IO::VERTEXSCALARMAP);

    }

    //-----------------------------------------------------------------------------------
    PairString map_vertex_distances_to_sources(
        Mesh_3 & mesh,
        HeatMethod & heat_object,
        const VectorVertexindex_3 vertex_index_iterator,
        const string map_name="v:heat_method",
        const bool clear_sources=true
    ){
        heat_object.add_sources(vertex_index_iterator);
        return map_vertex_distances_to_sources(mesh, heat_object, map_name, clear_sources);
    }

    //-----------------------------------------------------------------------------------
    PairString map_vertex_distances_to_sources(
        Mesh_3 & mesh,
        const VectorVertexindex_3 vertex_index_iterator,
        const string map_name="v:heat_method",
        const bool clear_sources=true
    ){
        HeatMethod heat_object(mesh);

        return map_vertex_distances_to_sources(
            mesh, heat_object, vertex_index_iterator, map_name, clear_sources);
    }

    //-----------------------------------------------------------------------------------
    TupleVertexVertexUmapvertex_3 closest_vertex_pair_between_sources(
        Mesh_3 & mesh,
        const VectorVertexindex_3 sources_1,
        const VectorVertexindex_3 sources_2,
        const string sources_name="v:heat_method"
    ){

        double shortest_distance = numeric_limits<double>::max();

        Vertexindex_3 closest_vertex_from_source_1_to_source_2;
        Vertexindex_3 closest_vertex_from_source_2_to_source_1;

        VectorVertexindex_3 shortest_path_source_1_to_source_2;

        double distance_placeholder(0);

        // get the vertex from source_1 closest to source_2
        map_vertex_distances_to_sources(mesh, sources_2, sources_name);
        MapVertexindexDouble_3 all_distances_to_sources_2 = mesh.property_map<Vertexindex_3, double>(sources_name).first;

        for (Vertexindex_3 vi: sources_1){

            distance_placeholder = get(all_distances_to_sources_2, vi);

            if (distance_placeholder < shortest_distance){
                closest_vertex_from_source_1_to_source_2 = vi;
                shortest_distance = distance_placeholder;
            }
        }

        // since we know the vertex from source_1, vertex_1, closest to source_2 we can use dijkstras shortest path to obtain
        // the shortest path from all vertices to vertex 1

        PairUmapdoubleVertexindex_3 distance_predecessor_map_from_source =
            dijkstra::shortest_distance_and_predecessor_umaps_from_source(
                mesh, closest_vertex_from_source_1_to_source_2);

        UmapVertexindexDouble_3 shortest_distance_umap = distance_predecessor_map_from_source.first;
        UmapVertexindex_3 predecessor_umap = distance_predecessor_map_from_source.second;

        // find the closest point on sources_2 to vertex_1
        shortest_distance = numeric_limits<double>::max();

        for (Vertexindex_3 vi: sources_2){
            distance_placeholder = shortest_distance_umap[vi];
            if (distance_placeholder < shortest_distance){
                closest_vertex_from_source_2_to_source_1 = vi;
                shortest_distance = distance_placeholder;
            }
        }

        return TupleVertexVertexUmapvertex_3(
            closest_vertex_from_source_1_to_source_2,
            closest_vertex_from_source_2_to_source_1,
            predecessor_umap
        );

    }
    //-----------------------------------------------------------------------------------
    TupleVertexVertexUmapvertex_3 closest_vertex_pair_between_sources(
        Mesh_3 & mesh,
        const VectorVertexindex_3 sources_1,
        const VectorVertexindex_3 sources_2,
        PairString & heat_method_meta,
        const string sources_name="v:heat_method"

    ){
        heat_method_meta.first = sources_name;
        heat_method_meta.second = IO::VERTEXSCALARMAP;
        return closest_vertex_pair_between_sources(mesh, sources_1, sources_2, sources_name);
    }

    //-----------------------------------------------------------------------------------
    VectorVertexindex_3 shortest_path_between_source_pair(
        const TupleVertexVertexUmapvertex_3 closes_vertex_predecessor_map_tuple,
        PairString & heat_method_meta,
        const string sources_name="v:heat_method"
    ){
        Vertexindex_3 closest_vertex_from_source_1_to_source_2 = get<0>(closes_vertex_predecessor_map_tuple);
        Vertexindex_3 closest_vertex_from_source_2_to_source_1 = get<1>(closes_vertex_predecessor_map_tuple);
        UmapVertexindex_3 predecessor_umap = get<2>(closes_vertex_predecessor_map_tuple);

        return dijkstra::shortest_path_between_two_vertices(
            predecessor_umap, closest_vertex_from_source_1_to_source_2, closest_vertex_from_source_2_to_source_1
        );
    }

    //-----------------------------------------------------------------------------------
    VectorVertexindex_3 shortest_path_between_source_pair(
        const TupleVertexVertexUmapvertex_3 closes_vertex_predecessor_map_tuple,
        const string sources_name="v:heat_method"
    ){

        PairString heat_method_meta;
        return shortest_path_between_source_pair(
            closes_vertex_predecessor_map_tuple,
            heat_method_meta,
            sources_name
        );

    }

    //-----------------------------------------------------------------------------------
    VectorVertexindex_3 shortest_path_between_source_pair(
        Mesh_3 & mesh,
        const VectorVertexindex_3 sources_1,
        const VectorVertexindex_3 sources_2,
        PairString & heat_method_meta,
        const string sources_name="v:heat_method"
    ){

        TupleVertexVertexUmapvertex_3 closes_vertex_predecessor_map_tuple = closest_vertex_pair_between_sources(
            mesh,
            sources_1, sources_2,
            heat_method_meta,
            sources_name
        );

        Vertexindex_3 closest_vertex_from_source_1_to_source_2 = get<0>(closes_vertex_predecessor_map_tuple);
        Vertexindex_3 closest_vertex_from_source_2_to_source_1 = get<1>(closes_vertex_predecessor_map_tuple);

        UmapVertexindex_3 predecessor_umap = get<2>(closes_vertex_predecessor_map_tuple);

        return dijkstra::shortest_path_between_two_vertices(
            predecessor_umap, closest_vertex_from_source_1_to_source_2, closest_vertex_from_source_2_to_source_1
        );
    }

    //-----------------------------------------------------------------------------------
    VectorVertexindex_3 shortest_path_between_source_pair(
        Mesh_3 & mesh,
        const VectorVertexindex_3 sources_1,
        const VectorVertexindex_3 sources_2,
        const string sources_name="v:heat_method"
    ){

        PairString heat_method_meta;

        return shortest_path_between_source_pair(
            mesh,
            sources_1, sources_2,
            heat_method_meta,
            sources_name
        );

    }


}
}
}