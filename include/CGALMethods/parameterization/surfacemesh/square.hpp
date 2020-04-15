#pragma once
#include <CGALMethods/parameterization/surfacemesh/base.hpp>


namespace CGALMethods{
namespace parameterization{
namespace surfacemesh{

namespace square{


    namespace authalic{

        PairString parameterize(Mesh_3 & mesh, const string name="v:parameterized_points"){
            return parameterize_t<SquareAuthalicParameterizer>(mesh, name);
        }

        PairString parameterize(
                Mesh_3 & mesh,
                VectorVertexindex_3 corner_vertices,
                string name="v:parameterized_points"
        ){
            return parameterize_t<SquareParameterizer, SquareAuthalicParameterizer>(
                mesh, corner_vertices, name
            );
        }
    }

    namespace conformal{

        PairString parameterize(Mesh_3 & mesh, const string name="v:parameterized_points"){
            return parameterize_t<SquareConformalParameterizer>(mesh, name);
        }

        PairString parameterize(
                Mesh_3 & mesh,
                VectorVertexindex_3 corner_vertices,
                string name="v:parameterized_points"
        ){
            return parameterize_t<SquareParameterizer, SquareConformalParameterizer>(
                mesh, corner_vertices, name
            );
        }

    }

}

}
}
}