#pragma once
#include <CGALMethods/boundary.hpp>
#include <CGALMethods/shortestpaths/heatmethod.hpp>

namespace CGALMethods {
namespace parameterization{


    VectorVertexindex_3 get_cylinder_seam(const Mesh_3 & mesh){

        NestedVectorVertexindex_3 boundaries = boundary::get_connected_boundary_vertices(mesh);

        return shortestpaths::heatmethod::shortest_path_between_source_pair(
            mesh, boundaries[0], boundaries[1]
        );

    }


}
}
