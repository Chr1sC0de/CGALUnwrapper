#pragma once
#include <CGALMethods/types.hpp>
#include <CGALMethods/IO/vtk.hpp>
#include <CGALMethods/localDifferentiableProperties/curvature.hpp>
#include <CGALMethods/property_buffer_vertices.hpp>

namespace CGALMethods{
namespace bound{

    namespace {
        using namespace types;
        using namespace std;
    }

    class SurfaceMesh{

        public:

            Mesh_3 data;
            MapStringString property_name_type;

            SurfaceMesh(){}
            SurfaceMesh(Mesh_3 _mesh, VectorPairString vector_name_type): data(_mesh) {
                parse_vector_property_type(vector_name_type);
            }
            SurfaceMesh(Mesh_3 _mesh, MapStringString _property_name_type): data(_mesh), property_name_type(_property_name_type){}
            SurfaceMesh(const string path){read_vtk(path);}
            SurfaceMesh(const string path, const double scale){read_vtk(path, scale);}
            SurfaceMesh(const SurfaceMesh & surf_mesh)
            :
                data(surf_mesh.data), property_name_type(surf_mesh.property_name_type)
            {}
            // ---------------------------------------------------------------------------------------------------------------------------
            void add_point_buffer(const VectorDouble & point_buffer){
                static const int n_cols = 3;
                int n_rows = point_buffer.size()/n_cols;
                for (int i=0; i<n_rows; i++){
                    data.add_vertex(
                        Point_3(
                            point_buffer[i*n_cols],
                            point_buffer[i*n_cols + 1],
                            point_buffer[i*n_cols + 2]
                        )
                    );
                }
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            void add_face_list(const list<VectorInt> & face_list){
                for (VectorInt face_indexes: face_list){
                    VectorVertexindex_3 temp_vertices(face_indexes.size());
                    for (int i=0; i<face_list.size(); i++)
                        temp_vertices[i] = Vertexindex_3(face_indexes[i]);
                    data.add_face(temp_vertices);
                }
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            VectorDouble points(){
                return propertyBuffer::vertices::points_3_property(data);
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            void parse_map_property_type(MapStringString _property_name_type){
                for (MapStringString::iterator it=_property_name_type.begin(); it!=_property_name_type.end(); it++){
                    property_name_type[it->first] = it->second;
                }
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            void parse_vector_property_type(VectorPairString vector_name_type){
                for (PairString name_type: vector_name_type){
                    property_name_type[name_type.first] = name_type.second;
                }
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            void read_vtk(const string path, const double scale=1){
                PairMeshVectorPairString_3 mesh_prop_data = IO::VTK::read(path, scale);
                this->data = mesh_prop_data.first;
                parse_vector_property_type(mesh_prop_data.second);
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            void write_vtk(string path){
                VectorPairString property_type_meta;

                for (MapStringString::iterator it=property_name_type.begin(); it!=property_name_type.end(); it++){
                    property_type_meta.push_back(
                        PairString(it->first, it->second)
                    );
                }

                IO::VTK::write(data, path, property_type_meta);
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            VectorString property_names(){
                VectorString _property_names;
                for (MapStringString::iterator it=property_name_type.begin(); it!=property_name_type.end(); it++){
                    _property_names.push_back(it->first);
                }
                return _property_names;
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            void calculate_curvature(int d_fitting=4, int d_monge=4, int knn=300, bool internal=true){
                VectorPairString curvature_props =
                    localDifferentiableProperties::curvature::monge::surface_mesh(
                        data, d_fitting, d_monge, knn, internal);
                parse_vector_property_type(curvature_props);
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            SurfaceMesh & operator = (const SurfaceMesh & surf_mesh){
                if (this == &surf_mesh){
                    return *this;
                }
                this->data = surf_mesh.data;
                this->property_name_type = surf_mesh.property_name_type;
                return *this;
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            VectorPairString get_vector_property_name_type(){
                VectorPairString output;

                for (MapStringString::iterator it=property_name_type.begin(); it!=property_name_type.end(); it++){
                    output.push_back(
                        PairString(it->first, it->second)
                    );
                }

                return output;
            }
            // ---------------------------------------------------------------------------------------------------------------------------
            string __repr__(){

                string output_string("SurfaceMesh(\n");
                output_string += "  properties: ";

                for (string prop: property_names()){
                    output_string += prop;
                    output_string += ", ";
                }

                output_string += "\n";

                output_string += "  num_vertices  : ";
                output_string += to_string(data.num_vertices());
                output_string += "\n";

                output_string += "  num_halfedges : ";
                output_string += to_string(data.num_halfedges());
                output_string += "\n";

                output_string += "  num_edges     : ";
                output_string += to_string(data.num_edges());
                output_string += "\n";

                output_string += "  num_faces     : ";
                output_string += to_string(data.num_faces());
                output_string += "\n";

                output_string += ")\n";

                return output_string;
            }
    };
}
}