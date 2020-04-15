#pragma once
#include <CGALMethods/property_buffer_vertices.hpp>
#include <CGALMethods/parameterization/parameterization.hpp>


namespace CGALMethods{
namespace parameterization{
namespace surfacemesh{

    typedef typename SMP::Square_border_uniform_parameterizer_3<Mesh_3> SquareParameterizer;
    typedef typename SMP::Discrete_conformal_map_parameterizer_3<Mesh_3, SquareParameterizer> SquareConformalParameterizer;
    typedef typename SMP::Discrete_authalic_parameterizer_3<Mesh_3, SquareParameterizer> SquareAuthalicParameterizer;

    typedef typename MapVertexindex_3_to_Point2 map_type;


    //-------------------------------------------------------------------------------------------------------------
    template <class ParameterizationMapper>
    PairString parameterize_t(Mesh_3 & mesh, const string name="v:parameterized_points"){

        map_type uv_pmap = mesh.add_property_map<Mesh_3::Vertex_index, Point_2>(name).first;
        Mesh_3::Halfedge_index longest_halfedge_indexes = CGAL::Polygon_mesh_processing::longest_border(mesh).first;
        SMP::parameterize(mesh, ParameterizationMapper(), longest_halfedge_indexes, uv_pmap);
        return PairString(name, IO::VERTEXSINGLEPOINT2MAP);

    }
    //-------------------------------------------------------------------------------------------------------------
    template <class BorderParameterizer, class ParameterizationMapper>
    PairString parameterize_t(Mesh_3 & mesh,  const vector<Vertexindex_3> corner_vertices, const string name="v:parameterized_points"){

            map_type uv_pmap = mesh.add_property_map<Vertexindex_3, Point_2>(name).first;
            Mesh_3::Halfedge_index longest_halfedge_indexes = CGAL::Polygon_mesh_processing::longest_border(mesh).first;

            BorderParameterizer border_param(corner_vertices[0], corner_vertices[1], corner_vertices[2], corner_vertices[3]);

            SMP::parameterize(mesh, ParameterizationMapper(border_param), longest_halfedge_indexes, uv_pmap);
            return PairString(name, IO::VERTEXSINGLEPOINT2MAP);

    }
    //-------------------------------------------------------------------------------------------------------------
    pair<Mesh_3, VectorPairString> make_mesh(
            const Mesh_3 & mesh,
            const string name="v:parameterized_points",
            const VectorPairString meta_data=VectorPairString()
    ){
        // from a mesh with parameterized set of points
        // generate a Mesh_3 which can be saved

        // make the parameterized output mesh
        Mesh_3 output_mesh;

        // add the 2d points to the mesh while making the mesh 3d

        auto pmap_vertex_indices_point_2 = mesh.property_map<Vertexindex_3, Point_2>(name).first;

        for (Vertexindex_3 vi: mesh.vertices()){

            output_mesh.add_vertex(
                Point_3(
                    pmap_vertex_indices_point_2[vi].x(),
                    pmap_vertex_indices_point_2[vi].y(),
                    0
                )
            );

        }

        // get the connectivity from the original mesh
        VectorVertexindex_3 vector_vertex_index;
        for (Faceindex_3 fi: faces(mesh)){

            Halfedgeindex_3 start_halfedge = halfedge(fi, mesh);

            for (Halfedgeindex_3 hi: halfedges_around_face(start_halfedge, mesh)){

                vector_vertex_index.push_back(
                    target(hi, mesh)
                );

            }

            // add the mesh
            output_mesh.add_face(vector_vertex_index);

            vector_vertex_index.clear();

        }

        // the output metadata
        VectorPairString output_meta;

        // add the original points to the mesh as a vector of properties
        output_meta.push_back(PairString("v:original_points", IO::VERTEXCGALVECTOR3MAP));

        auto output_original_points_map = output_mesh.add_property_map<Vertexindex_3, Vector_3>("v:original_points").first;

        for (Vertexindex_3 vi: mesh.vertices()){
            output_original_points_map[vi] = Vector_3(
                mesh.point(vi).x(), mesh.point(vi).y(), mesh.point(vi).z());
        }
        // for all the meta data add the data to the mesh

        if (meta_data.size() > 0){

            for (PairString meta: meta_data){

                VectorDouble buffer = propertyBuffer::vertices::property(mesh, meta);
                int dims = buffer.size() / output_mesh.number_of_vertices();

                if (dims == 1){

                    auto pmap = output_mesh.add_property_map<Vertexindex_3, double>(meta.first).first;
                    for (Vertexindex_3 vi: output_mesh.vertices()){
                        pmap[vi] = buffer[vi.idx()];
                    }

                    output_meta.push_back(PairString(meta.first, IO::VERTEXSCALARMAP));

                } else {

                    auto pmap = output_mesh.add_property_map<Vertexindex_3, VectorDouble>(meta.first).first;

                    for (Vertexindex_3 vi: output_mesh.vertices()){
                        VectorDouble temp_vector;
                        for (int i=0; i<dims; i++){
                            temp_vector.push_back(
                                buffer[vi.idx()*dims + i]
                            );
                        }
                        pmap[vi] = temp_vector;
                    }

                    output_meta.push_back(PairString(meta.first, IO::VERTEXVECTORMAP));
                }

            }
        }

        return pair<Mesh_3, VectorPairString>(output_mesh, output_meta);

    }

}
}
}