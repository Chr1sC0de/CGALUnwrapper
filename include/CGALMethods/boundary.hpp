#pragma once
#include <CGALMethods/types.hpp>

namespace CGALMethods{
namespace boundary{

    namespace {
        using namespace types;
    }
    //----------------------------------------------------------------------
    VectorHalfedgeindex_3 get_boundary_halfedges(const Mesh_3 & mesh){
        VectorHalfedgeindex_3 border_halfedges;
        for (Halfedgeindex_3 hi: mesh.halfedges()){
            if (mesh.face(hi) == Mesh_3::null_face()){
                border_halfedges.push_back(hi);
            }
        }
        return border_halfedges;
    }

    //----------------------------------------------------------------------
    NestedVectorHalfedgeindex_3 get_connected_boundary_halfedges(const Mesh_3 & mesh, const VectorHalfedgeindex_3 & border_halfedges){
        NestedVectorHalfedgeindex_3 connected_halfedges;

        UmapHalfedgeindexBool_3 halfedge_traversal_map;
        for (Halfedgeindex_3 hi: border_halfedges){
            halfedge_traversal_map[hi] = true;
        }

        Halfedgeindex_3 start_halfedge;
        Halfedgeindex_3 current_halfedge;
        Halfedgeindex_3 next_halfedge;

        VectorHalfedgeindex_3 temp_halfedge_vector;

        for (Halfedgeindex_3 hi: border_halfedges){

            if (halfedge_traversal_map[hi]){
                temp_halfedge_vector.push_back(hi);
                halfedge_traversal_map[hi] = false;

                start_halfedge = hi;
                current_halfedge = mesh.next(hi);
                next_halfedge = mesh.next(current_halfedge);

                while (
                    next_halfedge != start_halfedge &&
                    next_halfedge != Mesh_3::null_halfedge() &&
                    next_halfedge != current_halfedge
                ){
                    halfedge_traversal_map[current_halfedge] = false;
                    temp_halfedge_vector.push_back(current_halfedge);
                    current_halfedge = next_halfedge;
                    next_halfedge = mesh.next(current_halfedge);
                }

                halfedge_traversal_map[current_halfedge] = false;
                temp_halfedge_vector.push_back(current_halfedge);
                connected_halfedges.push_back(temp_halfedge_vector);
                temp_halfedge_vector.clear();
            }
        }

        return connected_halfedges;
    }

    //----------------------------------------------------------------------
    NestedVectorHalfedgeindex_3 get_connected_boundary_halfedges(const Mesh_3 & mesh){
        VectorHalfedgeindex_3 boundary_halfedges = get_boundary_halfedges(mesh);
        return get_connected_boundary_halfedges(
            mesh, boundary_halfedges
        );
    }

    //----------------------------------------------------------------------
    VectorVertexindex_3 get_boundary_vertices(const Mesh_3 & mesh, const VectorHalfedgeindex_3 & boundary_halfedges){

        VectorVertexindex_3 vertex_indexes(boundary_halfedges.size());
        for (int i=0; i<boundary_halfedges.size(); i++){
            vertex_indexes[i] = mesh.source(boundary_halfedges[i]);
        }
        return vertex_indexes;
    }

    //----------------------------------------------------------------------
    VectorVertexindex_3 get_boundary_vertices(const Mesh_3 & mesh){
        return get_boundary_vertices(mesh, get_boundary_halfedges(mesh));
    }

    //----------------------------------------------------------------------
    NestedVectorVertexindex_3 get_connected_boundary_vertices(const Mesh_3 & mesh, const NestedVectorHalfedgeindex_3 & connected_halfedges){

        VectorVertexindex_3 temp_vertex_vector_index_3;

        NestedVectorVertexindex_3 nested_vector_vertex_index;

        for (VectorHalfedgeindex_3 vhi: connected_halfedges){
            nested_vector_vertex_index.push_back(
                get_boundary_vertices(mesh, vhi)
            );
        }

        return nested_vector_vertex_index;
    }

    //----------------------------------------------------------------------
    NestedVectorVertexindex_3 get_connected_boundary_vertices(const Mesh_3 & mesh){

        return get_connected_boundary_vertices(
            mesh, get_connected_boundary_halfedges(mesh)
        );

    }

}
}