#pragma once

#include <CGALMethods/types.hpp>
#include <CGALMethods/IO/io.hpp>
#include <CGALMethods/property_buffer_vertices.hpp>

#include <vtkGenericDataObjectReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDataSet.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>
#include <vtkDataArrayAccessor.h>
#include <vtkAbstractArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>


namespace CGALMethods{
namespace IO {

namespace VTK{

    // anonymous namespace remove clutter
    namespace {
        using namespace std;
        using namespace types;
    }
    // vtk types
    typedef vtkSmartPointer<vtkGenericDataObjectReader> SmartPointerGenericDataReader;
    typedef typename vtkDataArrayAccessor<vtkDataArray>::APIType ValueType;
    typedef vtkDataArrayAccessor<vtkDataArray> DataArrayAccessor;


    vtkPolyData * read_generic_vtk_dataset(SmartPointerGenericDataReader & reader, const string path){

        reader->SetFileName(path.c_str());
        reader->Update();
        return reader->GetPolyDataOutput();

    }

    //---------------------------------------------------------------------------------------------------
    inline vtkPointData * feed_vtk_polydata_into_CGAL_mesh(vtkPolyData * vtk_poly_data, Mesh_3 & mesh, const double scale=1){

        double point_array[3];

        vtkPointData * vtk_point_data = vtk_poly_data->GetPointData();

        for (int i=0; i<vtk_poly_data->GetNumberOfPoints(); i++){

            vtk_poly_data->GetPoint(i, point_array);

            mesh.add_vertex(Mesh_3::Point(
                point_array[0]*scale, point_array[1]*scale, point_array[2]*scale));

        }

        return vtk_point_data;
    }

    //---------------------------------------------------------------------------------------------------
    inline PairString add_scalar_property_map(Mesh_3 & mesh, vtkAbstractArray * vtk_data_array){
        /*
            add a scalar property map to a mesh from a vtk_data_array
        */

        string prop_name("v:" + string(vtk_data_array->GetName()));

        PairString property_name_type(prop_name, string("VertexScalarMap"));

        Mesh_3::Property_map<Mesh_3::vertex_index, double> mesh_scalar_map =
            mesh.add_property_map<Mesh_3::vertex_index, double>
            (prop_name, 0).first;
        //downcast array for operations
        vtkDataArray * downcast_array = vtkArrayDownCast<vtkDataArray>(vtk_data_array);
        DataArrayAccessor accessor(downcast_array);
        // placeholder value
        ValueType value;
        for (Mesh_3::Vertex_index vi: mesh.vertices()){
            value = accessor.Get(vi.idx(), 0);
            put(mesh_scalar_map, vi, value);
        }

        return property_name_type;
    }

    //---------------------------------------------------------------------------------------------------
    inline PairString add_vector_property_map(Mesh_3 &mesh, vtkAbstractArray * vtk_data_array){

        /*
            add a vector field property map to a mesh from a vtk_data_array
        */

        string prop_name("v:" + string(vtk_data_array->GetName()));

        PairString property_name_type(prop_name, string("VertexVectorMap"));

        vector<double> value(vtk_data_array->GetNumberOfComponents(), double(0));

        Mesh_3::Property_map<Mesh_3::vertex_index, vector<double>> mesh_vector_map =
            mesh.add_property_map<Mesh_3::vertex_index, vector<double>>
                (prop_name, value).first;

        vtkDataArray * downcasted_array = vtkArrayDownCast<vtkDataArray>(vtk_data_array);

        vtkDataArrayAccessor<vtkDataArray> accessor(downcasted_array);

        for (Mesh_3::Vertex_index vi: mesh.vertices()){

            for (int j=0; j<vtk_data_array->GetNumberOfComponents(); j++){
                value[j] = accessor.Get(vi.idx(), 0);
            }

            put(mesh_vector_map, vi, value);
        }
        return property_name_type;
    }
    //---------------------------------------------------------------------------------------------------
    inline void add_face_to_mesh(Mesh_3 & mesh, vector<int> & cell_point_ids){

        if (cell_point_ids.size() == 3) {
            mesh.add_face(
                Mesh_3::Vertex_index(cell_point_ids[0]),
                Mesh_3::Vertex_index(cell_point_ids[1]),
                Mesh_3::Vertex_index(cell_point_ids[2])
            );
        }
        if (cell_point_ids.size() == 4) {
            mesh.add_face(
                Mesh_3::Vertex_index(cell_point_ids[0]),
                Mesh_3::Vertex_index(cell_point_ids[1]),
                Mesh_3::Vertex_index(cell_point_ids[2]),
                Mesh_3::Vertex_index(cell_point_ids[3])
            );
        }
    }

    //---------------------------------------------------------------------------------------------------
    VectorPairString
    read (Mesh_3 & mesh, const string path, const double scale=1){

        VectorPairString vector_pair_name_type;

        SmartPointerGenericDataReader reader = SmartPointerGenericDataReader::New();

        vtkPolyData * vtk_poly_data = read_generic_vtk_dataset(reader, path);
        vtkPointData * vtk_point_data = feed_vtk_polydata_into_CGAL_mesh(vtk_poly_data, mesh, scale);

        // read property maps from the vtk into the cgal mesh
        PairString mesh_property_name_type;

        for (int i=0; i<vtk_point_data->GetNumberOfArrays(); i++){

            vtkAbstractArray * vtk_data_array = vtk_point_data->GetArray(
                vtk_point_data->GetArrayName(i));

            if (vtk_data_array->GetNumberOfComponents() == 1){
                mesh_property_name_type = add_scalar_property_map(mesh, vtk_data_array);
            }
            if (vtk_data_array->GetNumberOfComponents() > 1){
                mesh_property_name_type = add_vector_property_map(mesh, vtk_data_array);
            }
            vector_pair_name_type.push_back(mesh_property_name_type);

        }

        // set the connectivity of the cgal mesh from the connectivity of the vtk mesh
        vtkCell * vtk_cell;
        vtkIdList * vtk_id_list;

        vector<int> cell_point_ids;

        for (int i=0; i<vtk_poly_data->GetNumberOfCells(); i++){
            vtk_cell = vtk_poly_data->GetCell(i);
            vtk_id_list = vtk_cell->GetPointIds();
            for (int j=0; j<vtk_id_list->GetNumberOfIds(); j++){
                cell_point_ids.push_back(vtk_id_list->GetId(j));
            }
            add_face_to_mesh(mesh, cell_point_ids);
            cell_point_ids.clear();
        }

        return vector_pair_name_type;
    }

    //---------------------------------------------------------------------------------------------------
    PairMeshVectorPairString_3
    read(const string path, const double scale=1){
        Mesh_3 mesh;
        VectorPairString vector_pair_name_type =
            read(mesh, path, scale);

        return PairMeshVectorPairString_3(mesh, vector_pair_name_type);
    }

    //---------------------------------------------------------------------------------------------------
    void write(const Mesh_3 & mesh, const string path, const VectorPairString properties_name_type){
        // vtkUnstructuredGridWriter
        vtkPoints * const vtk_points = vtkPoints::New();
        vtkCellArray * const vtk_cells = vtkCellArray::New();
        VectorString properties_vector = mesh.properties<Mesh_3::Vertex_index>();

        // set the points of the mesh
        for (Mesh_3::Vertex_index vi: mesh.vertices()){
            Mesh_3::Point point = mesh.point(vi);
            vtk_points->InsertNextPoint(
                point.x(),
                point.y(),
                point.z()
            );
        }

        // set the connectivity of the mesh
        for(Mesh_3::Face_index f : faces(mesh))
        {
            vtkIdList* cell = vtkIdList::New();
            for(Mesh_3::Halfedge_index h :
                            halfedges_around_face(halfedge(f, mesh), mesh))
            {
                cell->InsertNextId(target(h, mesh).idx());
            }

            vtk_cells->InsertNextCell(cell);
            cell->Delete();
        }

        vtkSmartPointer<vtkUnstructuredGrid> usg =
            vtkSmartPointer<vtkUnstructuredGrid>::New();

        usg->SetPoints(vtk_points);
        vtk_points->Delete();
        usg->SetCells(5,vtk_cells);
        vtk_cells->Delete();

        for (PairString ps: properties_name_type){
            vtkDoubleArray * data_array = vtkDoubleArray::New();

            VectorDouble buffer = propertyBuffer::vertices::property(mesh, ps);

            int buffer_dims = buffer.size() / mesh.number_of_vertices();

            if (buffer_dims == 1){
                data_array -> SetName(ps.first.c_str());

                for (Mesh_3::Vertex_index vi: mesh.vertices()){

                    // double testestset = ;
                    data_array->InsertValue(
                        vtkIdType(vi.idx()),
                        buffer[vi.idx()]
                    );

                }
                usg->GetPointData()->AddArray(data_array);
            }

            else if ( buffer_dims > 1 ){

                Mesh_3::Vertex_index test_vertex_index(0);

                data_array -> SetName(ps.first.c_str());
                data_array -> SetNumberOfComponents(buffer_dims);

                for (Mesh_3::Vertex_index vi: mesh.vertices()){

                    double * input_array = new double[buffer_dims];

                    for (int i=0; i<buffer_dims; i++){
                        input_array[i] = buffer[vi.idx()*buffer_dims+i];
                    }

                    data_array->InsertTuple(
                        vtkIdType(vi.idx()),
                        input_array
                    );

                    delete[] input_array;

                }

                usg->GetPointData()->AddArray(data_array);
            }

            data_array->Delete();

        }

        vtkSmartPointer<vtkUnstructuredGridWriter> writer =
            vtkSmartPointer<vtkUnstructuredGridWriter>::New();
        writer->SetFileName(path.c_str());
        writer->SetInputData(usg);
        writer->Write();

    }


}
}
}