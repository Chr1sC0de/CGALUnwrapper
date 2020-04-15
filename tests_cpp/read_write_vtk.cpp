#include <CGALMethods/IO/vtk.hpp>

using namespace CGALMethods;

int main(){

    std::string path("resources/WALL_400.vtk");
    std::string write_path_not_scaled("resources/WALL_400_written.vtk");
    std::string write_path_scaled("resources/WALL_400_written_scaled.vtk");

    types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);
    types::Mesh_3 mesh = mesh_n_property_meta_data.first;
    types::VectorPairString property_meta_data = mesh_n_property_meta_data.second;
    IO::VTK::write(mesh, write_path_not_scaled, property_meta_data);

    // scale the data on read
    types::PairMeshVectorPairString_3 mesh_n_property_meta_data_scaled = IO::VTK::read(path, 1000);
    types::Mesh_3 mesh_scaled  = mesh_n_property_meta_data_scaled .first;
    types::VectorPairString property_meta_data_scaled  = mesh_n_property_meta_data_scaled .second;
    IO::VTK::write(mesh_scaled, write_path_scaled, property_meta_data_scaled);

    return 0;
}