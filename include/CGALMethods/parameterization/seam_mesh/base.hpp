#pragma once
#include <CGALMethods/property_buffer_vertices.hpp>
#include <CGALMethods/parameterization/parameterization.hpp>

#include <CGAL/Eigen_solver_traits.h>
#include <CGAL/Eigen_matrix.h>
#include <CGAL/Eigen_diagonalize_traits.h>
#include <Eigen/IterativeLinearSolvers>

namespace CGALMethods{
namespace parameterization{
namespace seam_mesh{


    namespace {
        using namespace types;
    }

    // graph traits for the seam mesh
    typedef boost::graph_traits<SeamMesh> SeamGraphTraits;
    typedef typename SeamGraphTraits::vertex_descriptor SeamVertexDescriptor;
    typedef typename SeamGraphTraits::vertex_iterator SeamVertexDescriptorIterator;
    typedef typename SeamGraphTraits::halfedge_descriptor SeamHalfedgeDescriptor;
    typedef typename SeamGraphTraits::face_descriptor SeamFaceDescriptor;

    // square parameterizers templated for the seam mesh
    typedef typename SMP::Square_border_uniform_parameterizer_3
        <SeamMesh> SquareParameterizer;

    // typedef CGAL::Eigen_sparse_matrix<double>::EigenType EigenMatrix;

    // typedef CGAL::Eigen_solver_traits<
    //     Eigen::BiCGSTAB<
    //         CGAL::Eigen_sparse_matrix<double>::EigenType,
    //         Eigen::IncompleteLUT< double >
    //     >
    // > Iterative_symmetric_solver;

    // typedef CGAL::Eigen_solver_traits<
    //     Eigen::ConjugateGradient
    //         <
    //             CGAL::Eigen_sparse_matrix<double>::EigenType,
    //             Eigen::Upper|Eigen::Lower
    //             // Eigen::IncompleteLUT<double>
    //         >
    // > Iterative_symmetric_solver;


    typedef CGAL::Eigen_solver_traits<
        Eigen::BiCGSTAB<
            CGAL::Eigen_sparse_matrix<double>::EigenType,
            Eigen::DiagonalPreconditioner<double>
        >
    > Iterative_symmetric_solver;

    // set the solver

    typedef typename SMP::Discrete_conformal_map_parameterizer_3
        <SeamMesh, SquareParameterizer, Iterative_symmetric_solver> SquareConformalParameterizer;

    typedef typename SMP::Discrete_authalic_parameterizer_3
        <SeamMesh, SquareParameterizer, Iterative_symmetric_solver> SquareAuthalicParameterizer;

    typedef typename SMP::Barycentric_mapping_parameterizer_3
        <SeamMesh, SquareParameterizer, Iterative_symmetric_solver> SquareBarycentricParameterizer;

    typedef typename SMP::Mean_value_coordinates_parameterizer_3
        <SeamMesh, SquareParameterizer, Iterative_symmetric_solver> SquareFloaterMeanParameterizer;

    typedef typename std::vector<SeamVertexDescriptor> SeamVectorVertexdescriptor;
    typedef typename boost::unordered_map<Halfedgeindex_3, Vertexindex_3> UmapHalfedgeindexVertexindex_3;
    //-------------------------------------------------------------------------------------------------------
    PairMeshVectorPairString_3 make_mesh(
        const Mesh_3 & original_mesh,
        const SeamMesh & seam_mesh,
        APMHalfedgeindexPoint_32 & uv_map,
        VectorPairString meta_data=VectorPairString()
    ){
        Mesh_3 output;

        map<Halfedgeindex_3, Vertexindex_3> tm_om_map;

        vector<Vertexindex_3> vector_vertex_index;

        for (Halfedgeindex_3 hi : seam_mesh.vertices()){

            Point_2 temp_point = uv_map[hi];

            tm_om_map[hi] = output.add_vertex(
                Point_3(temp_point.x(), temp_point.y(), 0.0)
            );

        }

        for (SeamFaceDescriptor fd: CGAL::faces(seam_mesh)){

            SeamHalfedgeDescriptor hd = CGAL::halfedge(fd, seam_mesh);

            for (Halfedgeindex_3 hi : CGAL::vertices_around_face(hd, seam_mesh)){
                vector_vertex_index.push_back(
                    tm_om_map[hi]
                );
            }

            output.add_face(vector_vertex_index);
            vector_vertex_index.clear();

        }

        // equip the metadata

        if (meta_data.size() > 0){

            for (PairString name_type: meta_data){

                VectorDouble buffer = propertyBuffer::vertices::property(original_mesh, name_type);

                int dims = buffer.size()/original_mesh.num_vertices();

                if (dims == 1){

                    auto output_scalar_map = output.add_property_map<Vertexindex_3, double>(name_type.first).first;
                    int idx = 0;

                    for (Halfedgeindex_3 hi: seam_mesh.vertices()){
                        int temp_vertex = source(hi, original_mesh).idx();
                        output_scalar_map[Vertexindex_3(idx ++)] = buffer[temp_vertex];
                    }

                } else {

                    auto output_scalar_map = output.add_property_map<Vertexindex_3, VectorDouble>(name_type.first).first;
                    int idx = 0;
                    VectorDouble temp_vector;
                    for (Halfedgeindex_3 hi: seam_mesh.vertices()){
                        int temp_vertex = source(hi, original_mesh).idx();
                        for (int i=0; i<dims; i++){
                            temp_vector.push_back(
                                buffer[temp_vertex*dims + i]
                            );
                        }
                        output_scalar_map[Vertexindex_3(idx ++)] = temp_vector;
                        temp_vector.clear();
                    }

                }

            }

        }

        auto original_point_vector_map =
            output.add_property_map<Vertexindex_3, VectorDouble>("v:original_points", VectorDouble(3,0)).first;

        for (Halfedgeindex_3 hi : seam_mesh.vertices()){
            Vertexindex_3 temp_vi = source(hi, original_mesh);
            Point_3 temp_point = original_mesh.point(temp_vi);
            original_point_vector_map[tm_om_map[hi]][0] = temp_point.x();
            original_point_vector_map[tm_om_map[hi]][1] = temp_point.y();
            original_point_vector_map[tm_om_map[hi]][2] = temp_point.z();
        }

        string orig_point_names = "v:original_points";

        meta_data.push_back(
            PairString(
                orig_point_names,
                IO::VERTEXVECTORMAP
            )
        );

        return PairMeshVectorPairString_3(
            output, meta_data
        );
    }
    //-------------------------------------------------------------------------------------------------------
    template <class Parameterizer>
    void parameterize_t(SeamMesh & sm_mesh, APMHalfedgeindexPoint_32 & uv_pmap)
    {
        // UHMHalfedgeindexPoint_32 uv_uhm;
        // APMHalfedgeindexPoint_32 uv_pmap(uv_uhm);

        SeamHalfedgeDescriptor longest_halfedge_index =
            CGAL::Polygon_mesh_processing::longest_border(sm_mesh).first;

        SMP::parameterize(sm_mesh, Parameterizer(), longest_halfedge_index, uv_pmap);

        // return uv_pmap;
    }
   //-------------------------------------------------------------------------------------------------------
    template <class Parameterizer, class ParameterizerMapper>
    void parameterize_t(
            SeamMesh & sm_mesh,
            const VectorPoint_3 corner_points,
            APMHalfedgeindexPoint_32 & uv_pmap
    ){

        SeamHalfedgeDescriptor longest_halfedge_index =
            CGAL::Polygon_mesh_processing::longest_border(sm_mesh).first;

        VectorDouble min_distances;

        for (int i=0; i < 4; i++){
            min_distances.push_back(numeric_limits<double>::max());
        }

    //    vector<SeamVertexDescriptor> corner_indices(4);
        vector<SeamVertexDescriptor> corner_indices(4);
        Mesh_3 tm_mesh = sm_mesh.mesh();
        for (SeamHalfedgeDescriptor hd: CGAL::halfedges_around_face(longest_halfedge_index, sm_mesh)){

            Point_3 temp_point = tm_mesh.point(
                source(Halfedgeindex_3(hd), tm_mesh)
            );

            for (int i=0; i < corner_points.size(); i++){
                double distance = CGAL::sqrt(CGAL::squared_distance(corner_points[i], temp_point));
                if (distance < min_distances[i]){
                    min_distances[i] = distance;
                    corner_indices[i] = sm_mesh.source(hd);
                }
            }

        }

        while (sm_mesh.source(longest_halfedge_index) != corner_indices[0]){
            longest_halfedge_index = next(longest_halfedge_index, sm_mesh);
        }

        Parameterizer border_param(
            corner_indices[0], corner_indices[1],
            corner_indices[2], corner_indices[3]
        );

        SMP::parameterize(sm_mesh, ParameterizerMapper(border_param), longest_halfedge_index, uv_pmap);

        // return uv_pmap;
    }
   //-------------------------------------------------------------------------------------------------------


}
}
}

