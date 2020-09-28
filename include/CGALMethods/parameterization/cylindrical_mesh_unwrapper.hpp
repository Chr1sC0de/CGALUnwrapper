#pragma once
#include <CGALMethods/parameterization/seam_mesh/square.hpp>
#include <CGALMethods/boundary.hpp>
#include <CGALMethods/shortestpaths/heatmethod.hpp>

#include <algorithm>

#include <CGAL/centroid.h>

namespace CGALMethods {
namespace parameterization {

        namespace {

            using namespace seam_mesh;

            void rotate_inlet_or_outlet_vertices(Vertexindex_3 start, VectorVertexindex_3 & vertices){

                // find the amount to rotate;
                int n_rotate = 0;
                for (Vertexindex_3 vi: vertices){
                    if (vi!=start){
                        n_rotate ++;
                    } else {
                        break;
                    }
                }
                std::rotate(vertices.begin(), vertices.begin()+n_rotate, vertices.end());
            }

            void sort_vertices_seam(
                    VectorVertexindex_3 seam,
                    VectorVertexindex_3 & inlet_vertices,
                    VectorVertexindex_3 & outlet_vertices
            ){
                Vertexindex_3 inlet_start = seam[0];
                Vertexindex_3 outlet_start = seam[seam.size()-1];
                // now with the same ordering sort the points
                rotate_inlet_or_outlet_vertices(inlet_start, inlet_vertices);
                rotate_inlet_or_outlet_vertices(outlet_start, outlet_vertices);
            }

            void center_points(VectorPoint_3 & points){
                Point_3 centroid = CGAL::centroid(points.begin(), points.end());
                for (int i=0; i<points.size(); i++){
                    Point_3 centered_point(
                        points[i].x() - centroid.x(),
                        points[i].y() - centroid.y(),
                        points[i].z() - centroid.y()
                    );
                    points[i] = centered_point;
                }
            }

            Vector_3 calculate_normal(VectorPoint_3 centered_points){
                Point_3 point_1 = centered_points[0];
                Point_3 point_2 = centered_points[(int) centered_points.size()/8];
                Vector_3 vector_1(point_1.x(), point_1.y(), point_1.z());
                Vector_3 vector_2(point_2.x(), point_2.y(), point_2.z());

                Vector_3 crossed = CGAL::cross_product(
                    vector_1, vector_2
                );

                Vector_3 normal = crossed/CGAL::sqrt(crossed.squared_length());

                return normal;
            }

            void set_closest_set_of_vertices(
                const Mesh_3 mesh,
                int & vertex_set_id,
                const Point_3 origin,
                const NestedVectorVertexindex_3 set_of_vertices
            ){
                double closest_distance = std::numeric_limits<double>::max();
                int    counter          = 0;
                for (VectorVertexindex_3 vertices: set_of_vertices){
                    VectorPoint_3 data_points;

                    for (Vertexindex_3 vi: vertices)
                        data_points.push_back(mesh.point(vi));

                    Point_3 centroid = CGAL::centroid(data_points.begin(), data_points.end());
                    double  distance = CGAL:: square(CGAL::squared_distance(origin, centroid));
                    if (distance <= closest_distance){
                        closest_distance = distance;
                        vertex_set_id    = counter;
                    }
                    counter ++;
                }
            }

            void sort_inlet_outlet_vertices_correct_direction(
                    Mesh_3::Property_map<Vertexindex_3, Point_3> mesh_points,
                    VectorVertexindex_3 & inlet_vertices,
                    VectorVertexindex_3 & outlet_vertices
            ){
                // we want the inlet and the outlet to point in the same direction such that the inlet points away from the
                // centroid of all the points
                Point_3 centroid_all =
                    CGAL::centroid(mesh_points.begin(), mesh_points.end());
                // generate a vector of the inlet and outlet points
                VectorPoint_3 inlet_points;
                VectorPoint_3 outlet_points;
                for (Vertexindex_3 vi: inlet_vertices){
                    inlet_points.push_back(mesh_points[vi]);
                }
                for(Vertexindex_3 vi: outlet_vertices){
                    outlet_points.push_back(mesh_points[vi]);
                }
                Point_3 inlet_centroid = CGAL::centroid(inlet_points.begin(), inlet_points.end());
                Vector_3 centroid_to_inlet_vector(
                    inlet_centroid.x() - centroid_all.x(),
                    inlet_centroid.y() - centroid_all.y(),
                    inlet_centroid.z() - centroid_all.z()
                );
                // center the inlet and outlet points
                center_points(inlet_points);
                center_points(outlet_points);
                //inlet and outlet normals;
                Vector_3 inlet_normal = calculate_normal(inlet_points);
                Vector_3 outlet_normal = calculate_normal(outlet_points);
                // check that the inlet normal points away from the mesh centroid
                // if the inlet vector is pointing towards the centroid then reverese the order of the vertices
                if ((centroid_to_inlet_vector * inlet_normal) < (centroid_to_inlet_vector * (-inlet_normal))){
                    std::reverse(inlet_vertices.begin()+1, inlet_vertices.end());
                    std::reverse(inlet_points.begin()+1, inlet_points.end());
                    inlet_normal = calculate_normal(inlet_points);
                }
                //check that the inlet and outlet normals point in the same direction
                if ((inlet_normal * outlet_normal) < (inlet_normal * (-outlet_normal))){
                    std::reverse(outlet_vertices.begin()+1, outlet_vertices.end());
                }
            }
    }
/* -------------------------------------------------------------------------- */
    template <class _F>
    PairMeshVectorPairString_3 cylindrical_mesh_parameteriztion_t(
            _F F,
            Mesh_3 & mesh,
            VectorPairString meta_data=VectorPairString(),
            Point_3 inlet_origin=Point_3(
                std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max()
            ),
            Point_3 outlet_origin=Point_3(
                std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max()
            )
        ){
            // edge bool property maps
            UHMEdgeindexBool_3 edge_uhm(false);
            APMEdgeindexBool_3 edge_pmap(edge_uhm);
            // vertex bool property maps
            UHMVertexindexBool_3 vertex_uhm(false);
            APMVertexindexBool_3 vertex_pmap(vertex_uhm);
            // to do so we need to create a seam mesh out of our cylinder
            SeamMesh sm_mesh(mesh, edge_pmap, vertex_pmap);
            // obtain the boundaries of the original mesh
            NestedVectorVertexindex_3 boundary_vertices = boundary::get_connected_boundary_vertices(mesh);
            // now we must ensure that the inlet and outlet points are in the same direction
            // given the inlet and outlet points
            VectorVertexindex_3 inlet_vertices;
            VectorVertexindex_3 outlet_vertices;
            int inlet_vertex_set_id  = 0;
            int outlet_vertex_set_id = 0;

            if (inlet_origin.x() != std::numeric_limits<double>::max())
                set_closest_set_of_vertices(
                    mesh,
                    inlet_vertex_set_id,
                    inlet_origin,
                    boundary_vertices
                );
            if (outlet_origin.x() != std::numeric_limits<double>::max())
                set_closest_set_of_vertices(
                    mesh,
                    outlet_vertex_set_id,
                    outlet_origin,
                    boundary_vertices
                );

            while (outlet_vertex_set_id == inlet_vertex_set_id)
                outlet_vertex_set_id ++;

            inlet_vertices  = boundary_vertices[inlet_vertex_set_id];
            outlet_vertices = boundary_vertices[outlet_vertex_set_id];

            // find the shortest path from the inlet to the outlet vertices
            VectorVertexindex_3 shortest_path_from_inlet_to_outlet =
                shortestpaths::heatmethod::shortest_path_between_source_pair(mesh, inlet_vertices, outlet_vertices);
            // sort the vertices such that they start from the provided seam
            sort_vertices_seam(shortest_path_from_inlet_to_outlet, inlet_vertices, outlet_vertices);
            // sort the vertices such that the inlet normal points to the centroid while the  outlet normal
            // points away from the centroid
            sort_inlet_outlet_vertices_correct_direction(mesh.points(), inlet_vertices, outlet_vertices);
            // add the seam to the seam mesh
            for (int i=0; i<(shortest_path_from_inlet_to_outlet.size()-1); i++){
                sm_mesh.add_seam(
                    shortest_path_from_inlet_to_outlet[i],
                    shortest_path_from_inlet_to_outlet[i+1]
                );
            }
            UHMHalfedgeindexPoint_32 uv_uhm;
            APMHalfedgeindexPoint_32 uv_pmap(uv_uhm);
            // now that the inlet and outlet vertices are sorted in the correct directions we can set the corners of the parameterization
            VectorPoint_3 corner_points = {
                mesh.point(inlet_vertices[3]),
                mesh.point(outlet_vertices[3]),
                mesh.point(outlet_vertices[outlet_vertices.size()-4]),
                mesh.point(inlet_vertices[inlet_vertices.size()-4])
            };
            // parameterize the seam mesh
            // seam_mesh::square::authalic::parameterize(sm_mesh, corner_points, uv_pmap);
            F(sm_mesh, corner_points, uv_pmap);
            return seam_mesh::make_mesh(mesh, sm_mesh, uv_pmap, meta_data);

        }
    //--------------------------------------------------------------------------------------------------
    PairMeshVectorPairString_3 cylindrical_mesh_parameteriztion_square_authalic(
        Mesh_3 & mesh,
        VectorPairString meta_data=VectorPairString(),
        Point_3 inlet_origin=Point_3(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        ),
        Point_3 outlet_origin=Point_3(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        )
    ){
        auto F = [](
            SeamMesh & mesh,
            VectorPoint_3 corner_points,
            APMHalfedgeindexPoint_32 & uv_pmap
        ){seam_mesh::square::authalic::parameterize(mesh, corner_points, uv_pmap);};

        return cylindrical_mesh_parameteriztion_t(
            F , mesh, meta_data, inlet_origin, outlet_origin
        );
    }
    //--------------------------------------------------------------------------------------------------
    PairMeshVectorPairString_3 cylindrical_mesh_parameteriztion_square_conformal(
        Mesh_3 & mesh,
        VectorPairString meta_data=VectorPairString(),
        Point_3 inlet_origin=Point_3(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        ),
        Point_3 outlet_origin=Point_3(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        )
    ){
        auto F = [](
            SeamMesh & mesh,
            VectorPoint_3 corner_points,
            APMHalfedgeindexPoint_32 & uv_pmap
        ){seam_mesh::square::conformal::parameterize(mesh, corner_points, uv_pmap);};

        return cylindrical_mesh_parameteriztion_t(
            F , mesh, meta_data, inlet_origin, outlet_origin
        );
    }
    //--------------------------------------------------------------------------------------------------
    PairMeshVectorPairString_3 cylindrical_mesh_parameteriztion_square_barycentric(
        Mesh_3 & mesh,
        VectorPairString meta_data=VectorPairString(),
        Point_3 inlet_origin=Point_3(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        ),
        Point_3 outlet_origin=Point_3(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        )
    ){
        auto F = [](
            SeamMesh & mesh,
            VectorPoint_3 corner_points,
            APMHalfedgeindexPoint_32 & uv_pmap
        ){seam_mesh::square::barycentric::parameterize(mesh, corner_points, uv_pmap);};

        return cylindrical_mesh_parameteriztion_t(
            F , mesh, meta_data, inlet_origin, outlet_origin
        );
   }
    //--------------------------------------------------------------------------------------------------
    PairMeshVectorPairString_3 cylindrical_mesh_parameteriztion_square_floatermean(
        Mesh_3 & mesh,
        VectorPairString meta_data=VectorPairString(),
        Point_3 inlet_origin=Point_3(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        ),
        Point_3 outlet_origin=Point_3(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        )
    ){
        auto F = [](
            SeamMesh & mesh,
            VectorPoint_3 corner_points,
            APMHalfedgeindexPoint_32 & uv_pmap
        ){seam_mesh::square::floatermean::parameterize(mesh, corner_points, uv_pmap);};

        return cylindrical_mesh_parameteriztion_t(
            F , mesh, meta_data, inlet_origin, outlet_origin
        );
    }

}
}