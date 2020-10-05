Calculating Surface Curvature
-----------------------------

We can calculate the principal curvatures via methods contained within the `localDifferentiableProperties`,

.. code-block:: cpp

   #include <CGALMethods/IO/vtk.hpp>
   #include <CGALMethods/shortestpaths/dijkstras_shortest_path.hpp>
   #include <CGALMethods/localDifferentiableProperties/curvature.hpp>
   #include <CGALMethods/property_buffer_vertices.hpp>

   using namespace CGALMethods;
   using namespace localDifferentiableProperties;


   int main(){

      // read the mesh
      std::string path("resources/WALL_400.vtk");
      std::string write_path("resources/WALL_400_written_curvature_normals.vtk");

      types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);
      // extract the mesh and meta data
      types::Mesh_3 mesh = mesh_n_property_meta_data.first;
      types::VectorPairString mesh_property_meta_data = mesh_n_property_meta_data.second;

      ...

Curvature is calculated via Monge form jet fitting. After the mesh is input we supplym the degree of the fitted surface polynomial, the degree of
the Monge coefficients and finally me we decide whether the surface normals point either inwards or outwards. By default the normals will point 
inwards, i.e. true.

.. code-block:: cpp

      types::VectorPairString curvature_meta_data = curvature::monge::surface_mesh(mesh, 4, 4, 300, true);

      // extract the curvature and normal data as buffers
      types::VectorDouble maximum_curvature_buffer =
         propertyBuffer::vertices::property(mesh, curvature_meta_data[0]);
      types::VectorDouble minimum_curvature_buffer =
         propertyBuffer::vertices::property(mesh, curvature_meta_data[1]);

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

When the curvature is calculated it is stored as a property map on the mesh.
The meta data is returned such that the curvature can be extracted as a buffer via `propertyBuffer` methods.


