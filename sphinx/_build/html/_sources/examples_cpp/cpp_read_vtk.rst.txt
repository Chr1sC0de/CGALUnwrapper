Reading and Writing VTK to/from CGALMesh
----------------------------------------

Data from openfoam can be exported into multiple file formats. One such format is the VTK (legacy) format.
The following code shows how we can read data from the resulting vtk in CGAL using c++,

.. code-block:: cpp

    #include <CGALMethods/IO/vtk.hpp>

    using namespace CGALMethods;

    int main(){

        std::string path("resources/WALL_400.vtk");
        std::string write_path("resources/WALL_400_written.vtk");

        types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);

        types::Mesh_3 mesh = mesh_n_property_meta_data.first;

        types::VectorPairString property_meta_data = mesh_n_property_meta_data.second;

        IO::VTK::write(mesh, write_path, property_meta_data);

        return 0;
    }

In CGAL properties stored on the vertices and faces are stored as property maps. The 'property_meta_data' is
a VectorPairString, i.e. a vector of pairs of strings where for each pair the first element is the property
name and the second element is the property type contained in IO namespace, and the following types
are curently implemented,

.. code-block:: cpp

    namespace CGALMethods{
    namespace IO {

        string VERTEXSCALARMAP = "VertexScalarMap";
        string VERTEXVECTORMAP = "VertexVectorMap";
        string VERTEXSINGLEPOINTMAP = "VertexSinglePointMap";
        string VERTEXCGALVECTOR3MAP = "VertexCGALVector3Map";
    }
    }

for example to meta data required to extract the wall shear stress along the wall of the surface,

.. code-block::

    types::VectorPairString property_meta_data
    PairString magWallShearStressMeta("v:magWallShearStress", IO::VERTEXSCALARMAP);
    property_meta_data.push_back(magWallShearStressMeta);


some times we may wish to scale the data on input. We can prove a scaling factor to the read method
in order to convert our input data from metres to mm,

.. code-block:: cpp

    std::string write_path_scaled("resources/WALL_400_written_scaled.vtk");
    types::PairMeshVectorPairString_3 mesh_n_property_meta_data_scaled = IO::VTK::read(path, 1000);
    types::Mesh_3 mesh_scaled  = mesh_n_property_meta_data_scaled .first;
    types::VectorPairString property_meta_data_scaled  = mesh_n_property_meta_data_scaled .second;
    IO::VTK::write(mesh_scaled, write_path_scaled, property_meta_data_scaled);


