Convert Mesh Properties To Buffer
---------------------------------

A CGAL `Surface_mesh <https://doc.cgal.org/latest/Surface_mesh/index.html>`_ is a halfedge datastructure that
represents a polyhedral mesh. When interacting with different libraries it is common to convert properties of the
mesh into a buffer. One such example of when this is necessary is when we interface with a
`nummpy array <https://scipy-lectures.org/advanced/advanced_numpy/index.html>`_ in python.

The following code shows how to extract the CGAL mesh vertex properties as a std::vector<double> which can act
as our buffer,

.. code-block:: cpp

    #include <CGALMethods/IO/vtk.hpp>
    #include <CGALMethods/shortest_distance_and_predecessor_umaps_from_source.hpp>

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

the extracted buffers can now easily be passed into python via pybind11.