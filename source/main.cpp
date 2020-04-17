#include <CGALMethods/types.hpp>
#include <CGALMethods/parameterization/cylindrical_mesh_unwrapper.hpp>
#include <CGALMethods/bound/surface_mesh.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace CM = CGALMethods;
namespace tps = CM::types;

auto convert_buffer_to_numpy(
    CM::bound::SurfaceMesh & mesh,
    tps::VectorDouble buffer
){
    int n_rows = mesh.data.num_vertices();
    int n_cols = buffer.size()/n_rows;

    double * cbuffer = &buffer[0];

    return py::array_t<double>{
        {n_rows, n_cols},                         /* Buffer dimensions */
        {sizeof(double)*n_cols, sizeof(double)},  /* Strides (in bytes) for each index */
        cbuffer
    };
}

auto get_property_as_numpy(
    CM::bound::SurfaceMesh & mesh,
    std::string p_name
){
    bool exists(false);

    for (std::string name: mesh.property_names()){
        if (name == p_name){
            exists = true;
        }
    }

    if (!exists){
        throw "the specified property does not exist";
    }

    std::string type = mesh.property_name_type[p_name];
    tps::VectorDouble buffer = CM::propertyBuffer::vertices::property(mesh.data, tps::PairString(p_name, type));
    return convert_buffer_to_numpy(mesh, buffer);
}


// tps::VectorDouble default_vector({std::numeric_limits<double>::max(), 0.0, 0.0});

PYBIND11_MODULE(CGALMethods, m){

    py::class_<CM::bound::SurfaceMesh>(m, "SurfaceMesh")
        .def(py::init<>())
        .def(py::init<std::string, double>(), py::arg("path"), py::arg("scale")=1)
        .def("write_vtk", [](CM::bound::SurfaceMesh & self, std::string path){
            self.write_vtk(path);
        })
        .def("calculate_curvature", [](CM::bound::SurfaceMesh & self, int d_fitting, int d_monge, int knn, bool internal){
            py::gil_scoped_release release;
            self.calculate_curvature(d_fitting, d_monge, knn, internal);
        },
        py::arg("d_fitting") = 4, py::arg("d_monge") = 4, py::arg("knn") = 300, py::arg("internal") = true
        )
        .def("property_names", &CM::bound::SurfaceMesh::property_names)
        .def("points", [](CM::bound::SurfaceMesh & self){
            return convert_buffer_to_numpy(self, self.points());
        })
        .def("get_property", [](CM::bound::SurfaceMesh & self, const std::string property_name){
            return get_property_as_numpy(self, property_name);
        })
        .def("__repr__", &CM::bound::SurfaceMesh::__repr__)
    ;

    m.def("unwrap_cylindrical_surface_mesh", [](
                CM::bound::SurfaceMesh & wrapped_mesh
            ){
                tps::VectorPairString property_name_types = wrapped_mesh.get_vector_property_name_type();
                tps::PairMeshVectorPairString_3 cylinder_mesh_flat_data
                    = CM::parameterization::cylindrical_mesh_parameteriztion_square_authalic(wrapped_mesh.data, property_name_types);
                CM::bound::SurfaceMesh surface_mesh(
                    cylinder_mesh_flat_data.first,
                    cylinder_mesh_flat_data.second
                );
                return surface_mesh;
            }
        )
    ;

    m.def("unwrap_cylindrical_surface_mesh", [](
                CM::bound::SurfaceMesh & wrapped_mesh,
                double x,
                double y,
                double z
            ){
                tps::Point_3 point_origin(x, y, z);

                tps::VectorPairString property_name_types = wrapped_mesh.get_vector_property_name_type();

                tps::PairMeshVectorPairString_3 cylinder_mesh_flat_data
                    = CM::parameterization::cylindrical_mesh_parameteriztion_square_authalic(
                    wrapped_mesh.data, property_name_types, point_origin);

                CM::bound::SurfaceMesh surface_mesh(
                    cylinder_mesh_flat_data.first,
                    cylinder_mesh_flat_data.second
                );
                return surface_mesh;

            }
        )
    ;

}













