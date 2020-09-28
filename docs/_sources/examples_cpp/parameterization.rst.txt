Parameterizating a Mesh
-----------------------

For visualizing the insides of the coronart artery it might be of interest to unwrap the vessel for easy visualization.
To parameterize the mesh as a square,

.. code-block:: cpp

   #include <CGALMethods/parameterization/surfacemesh/square.hpp>
   #include <CGALMethods/parameterization/seam_mesh/square.hpp>
   #include <CGALMethods/IO/vtk.hpp>
   #include <CGALMethods/parameterization/cylindrical_mesh_unwrapper.hpp>

   using namespace CGALMethods;

   int main () {

      std::string path("resources/WALL_400.vtk");
      std::string write_path_0("resources/WALL_400_surface_mesh_parameterization_no_corners.vtk");
      std::string write_path_1("resources/WALL_400_surface_mesh_parameterization_with_corners.vtk");

      types::PairMeshVectorPairString_3 mesh_n_property_meta_data = IO::VTK::read(path);
      types::Mesh_3 mesh = mesh_n_property_meta_data.first;
      types::VectorPairString property_meta_data = mesh_n_property_meta_data.second;

      // parameterize a mesh without any specified corners
      // observe that we cannot get the desire parameterization thus we must must define a seam

      types::PairString meta_uv = parameterization::surfacemesh::square::authalic::parameterize(mesh);
      std::pair<types::Mesh_3, types::VectorPairString> parameterized_mesh_n_meta =
         parameterization::surfacemesh::make_mesh(mesh, meta_uv.first, property_meta_data);

      IO::VTK::write(
         parameterized_mesh_n_meta.first,
         write_path_0,
         parameterized_mesh_n_meta.second);
    ...

nonetheless we observe that the parameterization is less than ideal. Thus we must generate a seam along the mesh and alog the seam
specify a set of corners such that we can perform an unwrapping. We have currently implemented an automatic method for cylindrical surfacemeshes,

.. code-block:: cpp

   // parameterize a mesh using pre-defined cylindrical mesh parameterizer, in this method a seam mesh is generated
   // and along the seam mesh the coner vertices are specified. From there the surface is parameterized
    types::PairMeshVectorPairString_3 cylinder_mesh_flat_data =
        parameterization::cylindrical_mesh_parameteriztion_square_authalic(mesh, property_meta_data);

    types::Mesh_3 cylinder_mesh_flat = cylinder_mesh_flat_data.first;
    types::VectorPairString  meta_flat = cylinder_mesh_flat_data.second;

    IO::VTK::write(
        cylinder_mesh_flat,
        write_path_1,
        meta_flat);;

We observe that the mesh is now parameterized over a square where the inlet and the outlet take one of the edges


