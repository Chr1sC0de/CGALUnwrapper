#pragma once
#include <CGALMethods/IO/io.hpp>
#include <CGALMethods/types.hpp>

namespace CGALMethods{
namespace propertyBuffer{
    // convert a property map to a buffer which can be passed through numpy
namespace vertices{

    namespace{
        using namespace types;
        using namespace std;
    }

    //-----------------------------------------------------------------
    VectorDouble vector_3_property(const Mesh_3 & mesh, const std::string name){

        std::pair<Mesh_3::Property_map<Mesh_3::Vertex_index, Vector_3>, bool>
            mesh_vertex_vector_map_meta = mesh.property_map<Mesh_3::Vertex_index, Vector_3>(
                name);

        Mesh_3::Property_map<Mesh_3::vertex_index, Vector_3>
            mesh_vector_map = mesh_vertex_vector_map_meta.first;

        if (!mesh_vertex_vector_map_meta.second){
            throw "map " + name + " does not exist";
        }

        int num_vertices = mesh.num_vertices();

        VectorDouble buffer(num_vertices*3);

        for (Mesh_3::Vertex_index vi: mesh.vertices()){

            Vector_3 temp_vector = get(mesh_vector_map, vi);

            buffer[vi.idx()*3] = get(mesh_vector_map, vi)[0];
            buffer[vi.idx()*3 + 1] = get(mesh_vector_map, vi)[1];
            buffer[vi.idx()*3 + 2] = get(mesh_vector_map, vi)[2];
        }

        return buffer;

    }

    //-----------------------------------------------------------------
    VectorDouble points_3_property(const Mesh_3 & mesh, const std::string name){

        std::pair<Mesh_3::Property_map<Mesh_3::Vertex_index, Mesh_3::Point>, bool>
            mesh_singlepoint_map_meta = mesh.property_map<Mesh_3::Vertex_index, Mesh_3::Point>(
                name);

        Mesh_3::Property_map<Mesh_3::vertex_index, Mesh_3::Point>
            mesh_point_map = mesh_singlepoint_map_meta.first;

        if (!mesh_singlepoint_map_meta.second){
            throw "map " + name + " does not exist";
        }

        int num_vertices = mesh.num_vertices();
        int num_dims = mesh.point(
            Mesh_3::Vertex_index(0)
        ).dimension();

        VectorDouble buffer(num_vertices*num_dims);

        for (Mesh_3::Vertex_index vi: mesh.vertices()){
            buffer[vi.idx()*num_dims] = get(mesh_point_map, vi).x();
            buffer[vi.idx()*num_dims + 1] = get(mesh_point_map, vi).y();
            buffer[vi.idx()*num_dims + 2] = get(mesh_point_map, vi).z();
        }

        return buffer;

    }

    //-----------------------------------------------------------------
    VectorDouble points_3_property(const Mesh_3 & mesh){

        int num_vertices = mesh.num_vertices();
        int num_dims = mesh.point(
            Mesh_3::Vertex_index(0)
        ).dimension();

        VectorDouble buffer(num_vertices*num_dims);

        for (Mesh_3::Vertex_index vi: mesh.vertices()){
            buffer[vi.idx()*num_dims] = mesh.point(vi).x();
            buffer[vi.idx()*num_dims + 1] = mesh.point(vi).y();
            buffer[vi.idx()*num_dims + 2] = mesh.point(vi).z();
        }

        return buffer;

    }

    //-----------------------------------------------------------------
    VectorDouble points_2_property(const Mesh_3 & mesh, const std::string name){

        std::pair<Mesh_3::Property_map<Mesh_3::Vertex_index, Point_2>, bool>
            mesh_singlepoint_map_meta = mesh.property_map<Mesh_3::Vertex_index, Point_2>(
                name);

        Mesh_3::Property_map<Mesh_3::vertex_index, Point_2>
            mesh_point_map = mesh_singlepoint_map_meta.first;

        if (!mesh_singlepoint_map_meta.second){
            throw "map " + name + " does not exist";
        }

        int num_vertices = mesh.num_vertices();

        VectorDouble buffer(num_vertices*3);

        for (Mesh_3::Vertex_index vi: mesh.vertices()){
            buffer[vi.idx()*3] = get(mesh_point_map, vi).x();
            buffer[vi.idx()*3 + 1] = get(mesh_point_map, vi).y();
            buffer[vi.idx()*3 + 2] = 0;
        }

        return buffer;

    }

    //-----------------------------------------------------------------
    VectorDouble scalar_property(const Mesh_3 & mesh, const string & name){

        pair<Mesh_3::Property_map<Mesh_3::vertex_index, double>, bool>
            mesh_scalar_map_meta = mesh.property_map<Mesh_3::Vertex_index, double>(
                name);

        Mesh_3::Property_map<Mesh_3::vertex_index, double>
            mesh_scalar_map = mesh_scalar_map_meta.first;

        if (!mesh_scalar_map_meta.second){
            throw "map " + name + " does not exist";
        }

        vector<double> buffer(mesh.num_vertices());

        for (Mesh_3::Vertex_index vi: mesh.vertices()){
            buffer[vi.idx()] = get(mesh_scalar_map, vi);
        }

        return buffer;
    }


    //-----------------------------------------------------------------
    VectorDouble vector_property(const Mesh_3 & mesh, const string & name){

        pair<Mesh_3::Property_map<Mesh_3::vertex_index, vector<double>>, bool>
            mesh_scalar_map_meta = mesh.property_map<Mesh_3::Vertex_index, vector<double>>(
                name);

        Mesh_3::Property_map<Mesh_3::vertex_index, vector<double>>
            mesh_scalar_map = mesh_scalar_map_meta.first;

        if (!mesh_scalar_map_meta.second){
            throw "map " + name + " does not exist";
        }

        int vector_size = get(mesh_scalar_map, Mesh_3::Vertex_index(0)).size();

        vector<double> buffer(mesh.num_vertices()*vector_size);

        vector<double> temp_vector;

        for (Mesh_3::Vertex_index vi: mesh.vertices()){
            temp_vector = get(mesh_scalar_map, vi);
            for (int i=0; i<vector_size; i++){
                buffer[vi.idx()*vector_size+i] = temp_vector[i];
            }
        }

        return buffer;
    }

    //-----------------------------------------------------------------
    VectorDouble property(const Mesh_3 & mesh, const PairString name_type){

            VectorDouble buffer;
            if (name_type.second == IO::VERTEXSCALARMAP){
                buffer = scalar_property(mesh, name_type.first);
            }
            else if (name_type.second == IO::VERTEXVECTORMAP){
                buffer = vector_property(mesh, name_type.first);
            }
            else if (name_type.second == IO::VERTEXSINGLEPOINT3MAP){
                buffer = points_3_property(mesh, name_type.first);
            }
            else if (name_type.second == IO::VERTEXSINGLEPOINT2MAP){
                buffer = points_2_property(mesh, name_type.first);
            }
            else if (name_type.second == IO::VERTEXCGALVECTOR3MAP){
                buffer = vector_3_property(mesh, name_type.first);
            }
            return buffer;

    }
}
}
}