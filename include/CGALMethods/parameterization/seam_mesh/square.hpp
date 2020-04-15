#pragma once
#include <CGALMethods/parameterization/seam_mesh/base.hpp>


namespace CGALMethods{
namespace parameterization{
namespace seam_mesh{


namespace square{

    namespace authalic{

        void parameterize(SeamMesh & mesh, APMHalfedgeindexPoint_32 & uv_pmap){
            parameterize_t<SquareAuthalicParameterizer>(mesh, uv_pmap);
        }

        void parameterize(
                SeamMesh & mesh,
                const VectorPoint_3 corner_points,
                APMHalfedgeindexPoint_32 & uv_pmap
        ){
            parameterize_t<SquareParameterizer, SquareAuthalicParameterizer>(
                mesh, corner_points, uv_pmap
            );
        }

    }

    // namespace conformal{

    //     PairString parameterize(SeamMesh & mesh){
    //         return parameterize_t<SquareConformalParameterizer>(mesh);
    //     }

    //     PairString parameterize(
    //             SeamMesh & mesh,
    //             VectorVertexindex_3 corner_vertices,
    //             string name="v:parameterized_points"
    //     ){
    //         return parameterize_t<SquareParameterizer, SquareConformalParameterizer>(
    //             mesh, corner_vertices
    //         );
    //     }

    // }

}



}
}
}


