#pragma once

#include <CGALMethods/types.hpp>
#include <CGALMethods/IO/io.hpp>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Surface_mesh_parameterization/Square_border_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/Discrete_authalic_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/Discrete_conformal_map_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/Mean_value_coordinates_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/Barycentric_mapping_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/parameterize.h>

namespace CGALMethods{
namespace parameterization{

    namespace {
        using namespace types;
        using namespace std;
    }

    namespace SMP = CGAL::Surface_mesh_parameterization;

}
}
