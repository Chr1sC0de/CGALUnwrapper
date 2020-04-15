#include <CGALMethods/boundary.hpp>
#include <CGALMethods/IO/vtk.hpp>

using namespace CGALMethods;


int main() {
    std::string path("resources/WALL_400.vtk");

    types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);
    types::Mesh_3 mesh = mesh_n_property_meta_data.first;
    types::VectorPairString property_meta_data = mesh_n_property_meta_data.second;

    // to get the halfedges on the boundary
    types::VectorHalfedgeindex_3 boundary_halfedges = boundary::get_boundary_halfedges(mesh);

    // from the boundary halfedges obtain sets of connected halfedges
    types::NestedVectorHalfedgeindex_3 connected_boundary_halfedges =
        boundary::get_connected_boundary_halfedges(mesh, boundary_halfedges);

    // obtain the connected halfedges without knowing the boundary edges
    types::NestedVectorHalfedgeindex_3 connected_boundary_halfedges_no_boundary_edges =
        boundary::get_connected_boundary_halfedges(mesh);

    // obtain the vertices along the boundary from the halfedges
    types:: VectorVertexindex_3 boundary_vertices_from_halfedges =  boundary::get_boundary_vertices(mesh, boundary_halfedges);

    // obtain the vertices along the boundary
    types:: VectorVertexindex_3 boundary_vertices =  boundary::get_boundary_vertices(mesh);

    // to obtain the boundary vertices connected by halfedges
    types:: NestedVectorVertexindex_3 connected_boundary_vertices =
        boundary::get_connected_boundary_vertices(mesh, connected_boundary_halfedges);

    //without explicitly calculating the connected boundary halfedges
    connected_boundary_vertices =  boundary::get_connected_boundary_vertices(mesh);

    return 0;
}