#pragma once

#include <CGALMethods/types.hpp>

#include <CGAL/Search_traits_3.h>
#include <CGAL/Search_traits_adapter.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Monge_via_jet_fitting.h>
#include <CGAL/mst_orient_normals.h>

namespace CGALMethods{
namespace localDifferentiableProperties{
namespace curvature{

    namespace {
        using namespace types;
    }

    typedef boost::graph_traits<Mesh_3>::vertex_descriptor Vertexdescriptor;
    typedef boost::property_map<Mesh_3, CGAL::vertex_point_t>::type Vertex_point_pmap;

    typedef CGAL::Search_traits_3<Kernel> Traitsbase;
    typedef CGAL::Search_traits_adapter<Vertexdescriptor, Vertex_point_pmap, Traitsbase> Traits;

    typedef CGAL::Orthogonal_k_neighbor_search<Traits> K_neighbor_search;
    typedef K_neighbor_search::Tree Tree;
    typedef Tree::Splitter Splitter;
    typedef K_neighbor_search::Distance Distance;

    typedef CGAL::Monge_via_jet_fitting<Kernel> My_Monge_via_jet_fitting;
    typedef My_Monge_via_jet_fitting::Monge_form My_Monge_form;

namespace monge{

    VectorPairString surface_mesh(Mesh_3 & mesh,
            const int d_fitting=4,
            const int d_monge=4,
            const int n_nearest_neighbours=300,
            const bool point_normals_inwards=true
    ){

        Vertex_point_pmap vertex_point_property_map = get(CGAL::vertex_point, mesh);

        Tree tree(
            CGAL::vertices(mesh).begin(), CGAL::vertices(mesh).end(),
            Splitter(), Traits(vertex_point_property_map)
        );

        Distance tree_distance(vertex_point_property_map);

        My_Monge_form monge_form;
        My_Monge_via_jet_fitting monge_fit;

        MapVertexindexDouble_3 maximum_curvature_pmap =
            mesh.add_property_map<Vertexindex_3, double>("v:maximum_curvature", 0).first;

        MapVertexindexDouble_3 minimum_curvature_pmap =
            mesh.add_property_map<Vertexindex_3, double>("v:minimum_curvature", 0).first;

        MapVertexindexVector_3 vertex_normals =
            mesh.add_property_map<Vertexindex_3, Vector_3>("v:normals_temp", Vector_3(0, 0, 0)).first;

        CGAL::Polygon_mesh_processing::compute_vertex_normals(
            mesh, vertex_normals
        );

        if (point_normals_inwards){
            for_each(vertex_normals.begin(), vertex_normals.end(), [](Vector_3 &normal){normal*=-1;});
        }

        for (Vertexindex_3 vi: mesh.vertices()){

            Point_3 query_point = mesh.point(vi);

            VectorPoint_3 neighbour_points;

            K_neighbor_search search(tree, query_point, n_nearest_neighbours, 0, true, tree_distance);

            // get the nearest neighbours of the query point
            for (K_neighbor_search::iterator it=search.begin(); it!=search.end(); it++){
                neighbour_points.push_back(
                    mesh.point(it->first)
                );
            }

            monge_form = monge_fit(neighbour_points.begin(), neighbour_points.end(), d_fitting, d_monge);

            monge_form.comply_wrt_given_normal(vertex_normals[vi]);

            maximum_curvature_pmap[vi] = monge_form.principal_curvatures(0);
            minimum_curvature_pmap[vi] = monge_form.principal_curvatures(1);
        }

        VectorPairString output = {
                PairString("v:maximum_curvature", IO::VERTEXSCALARMAP),
                PairString("v:minimum_curvature", IO::VERTEXSCALARMAP),
                PairString("v:normals", IO::VERTEXVECTORMAP)
            };

        Mesh_3::Property_map<Vertexindex_3, VectorDouble> test_normals =
            mesh.add_property_map<Vertexindex_3, VectorDouble>("v:normals", VectorDouble(3, 0)).first;

        for (Vertexindex_3 vi: mesh.vertices()){
            test_normals[vi] = VectorDouble(
                {
                    vertex_normals[vi][0],
                    vertex_normals[vi][1],
                    vertex_normals[vi][2]
                }
            );
        }

        return output;

    }

}
}
}
}