#include <CGALMethods/IO/vtk.hpp>
#include <CGALMethods/property_buffer_vertices.hpp>

using namespace CGALMethods;
namespace pb = propertyBuffer;

int main(){

    std::string path("resources/WALL_400.vtk");
    std::string write_path("resources/WALL_400_written.vtk");

    types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);

    types::Mesh_3 mesh = mesh_n_property_meta_data.first;

    types::VectorPairString property_meta_data = mesh_n_property_meta_data.second;

    //v:magWallShearStress, VertexScalarMap
    types::VectorDouble test_0 = pb::vertices::property(mesh, property_meta_data[0]);
    //v:p, VertexScalarMap
    types::VectorDouble test_1 = pb::vertices::property(mesh, property_meta_data[1]);
    //v:U_0, VertexVectorMap
    types::VectorDouble test_2 = pb::vertices::property(mesh, property_meta_data[2]);
    //v:U, VertexVectorMap
    types::VectorDouble test_3 = pb::vertices::property(mesh, property_meta_data[3]);

    return 0;
}