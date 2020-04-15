#include <CGALMethods/IO/vtk.hpp>
#include <CGALMethods/shortestpaths/dijkstras_shortest_path.hpp>
#include <CGALMethods/localDifferentiableProperties/curvature.hpp>
#include <CGALMethods/property_buffer_vertices.hpp>

using namespace CGALMethods;
using namespace localDifferentiableProperties;


int main(){

    std::string path("resources/WALL_400.vtk");
    std::string write_path("resources/WALL_400_written_curvature_normals.vtk");

    types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);

    types::Mesh_3 mesh = mesh_n_property_meta_data.first;
    types::VectorPairString mesh_property_meta_data = mesh_n_property_meta_data.second;

    types::VectorPairString curvature_meta_data = curvature::monge::surface_mesh(mesh, 4, 4, 300, true);

    // extract the curvature and normal data as buffers
    types::VectorDouble maximum_curvature_buffer = propertyBuffer::vertices::property(mesh, curvature_meta_data[0]);
    types::VectorDouble minimum_curvature_buffer = propertyBuffer::vertices::property(mesh, curvature_meta_data[1]);
    types::VectorDouble normals_buffer = propertyBuffer::vertices::property(mesh, curvature_meta_data[2]);

    // write the curvature and normal data as as vtk
    // extend the vector
    for (auto item: curvature_meta_data){
        mesh_property_meta_data.push_back(item);
    }
    // finally write out the entire data
    IO::VTK::write(mesh, write_path, mesh_property_meta_data);

    return 0;
}