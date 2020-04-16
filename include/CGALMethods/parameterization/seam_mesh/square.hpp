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

    namespace conformal{

        void parameterize(SeamMesh & mesh, APMHalfedgeindexPoint_32 & uv_pmap){
            parameterize_t<SquareConformalParameterizer>(mesh, uv_pmap);
        }

        void parameterize(
                SeamMesh & mesh,
                const VectorPoint_3 corner_points,
                APMHalfedgeindexPoint_32 & uv_pmap
        ){
            parameterize_t<SquareParameterizer, SquareConformalParameterizer>(
                mesh, corner_points, uv_pmap
            );
        }

    }

    namespace barycentric{

        void parameterize(SeamMesh & mesh, APMHalfedgeindexPoint_32 & uv_pmap){
            parameterize_t<SquareBarycentricParameterizer>(mesh, uv_pmap);
        }

        void parameterize(
                SeamMesh & mesh,
                const VectorPoint_3 corner_points,
                APMHalfedgeindexPoint_32 & uv_pmap
        ){
            parameterize_t<SquareParameterizer, SquareBarycentricParameterizer>(
                mesh, corner_points, uv_pmap
            );
        }

    }

    namespace floatermean{

        void parameterize(SeamMesh & mesh, APMHalfedgeindexPoint_32 & uv_pmap){
            parameterize_t<SquareFloaterMeanParameterizer>(mesh, uv_pmap);
        }

        void parameterize(
                SeamMesh & mesh,
                const VectorPoint_3 corner_points,
                APMHalfedgeindexPoint_32 & uv_pmap
        ){
            parameterize_t<SquareParameterizer, SquareFloaterMeanParameterizer>(
                mesh, corner_points, uv_pmap
            );
        }

    }

}



}
}
}


