cmake_minimum_required(VERSION 3.16.1)
project(CGALMethods)

# set the policies required to show the mesh
if(POLICY CMP0053)
    # Only set CMP0053 to OLD with CMake<3.10, otherwise there is a warning.
    if(NOT POLICY CMP0070)
        cmake_policy(SET CMP0053 OLD)
    else()
        cmake_policy(SET CMP0053 NEW)
    endif()
endif()
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

# CGAL and components -----------------------------------------------------------------
# find_package(CGAL COMPONENTS Qt5)
# CGAL and its components
find_package( CGAL QUIET)
if ( NOT CGAL_FOUND )
    message(STATUS "This project requires the CGAL library, and will not be compiled.")
    return()
endif()
# include helper file
include( ${CGAL_USE_FILE} )
# Boost and its components
find_package( Boost REQUIRED )
if ( NOT Boost_FOUND )
    message(STATUS "This project requires the Boost library, and will not be compiled.")
    return()
endif()

# Eigen and components -----------------------------------------------------------------
SET( EIGEN3_INCLUDE_DIR "C:/dev/Eigen3" )

if ( EIGEN3_INCLUDE_DIR )
    include( ${EIGEN3_USE_FILE} )
else()
    message(STATUS "This project requires the Eigen library (3.3 or greater), and will not be compiled.")
    return()
endif()

# main components -----------------------------------------------------------------------

set(SOURCE_DIR "source")

include_directories(
    "include"
)
add_subdirectory(pybind11)

# add the pybind module
pybind11_add_module(CGALMethods ${SOURCES} "${SOURCE_DIR}/main.cpp")

# VTK components -----------------------------------------------------------------------
find_package(VTK COMPONENTS
  vtkCommonCore
  vtkCommonDataModel
  vtkIOLegacy QUIET)
if (NOT VTK_FOUND)
  message("Skipping GenericDataObjectReader: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
#   add_executable(GenericDataObjectReader MACOSX_BUNDLE GenericDataObjectReader.cxx )
  message("VTK Libraries: ${VTK_LIBRARIES}")
  target_link_libraries(CGALMethods PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
#   add_executable(GenericDataObjectReader MACOSX_BUNDLE GenericDataObjectReader.cxx )
  target_link_libraries(CGALMethods PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS shortestPath
    MODULES ${VTK_LIBRARIES}
    )
endif ()


# test files

file(GLOB test_files "tests_cpp/*.cpp")

find_package(OpenMP)

foreach(file ${test_files})

    get_filename_component(filename_component ${file} NAME_WE)

    add_executable(${filename_component} ${file})

    target_include_directories(${filename_component} PUBLIC "include")

    target_link_libraries(${filename_component} PRIVATE ${VTK_LIBRARIES})

    if(OpenMP_CXX_FOUND)
        target_link_libraries(${filename_component} PUBLIC OpenMP::OpenMP_CXX)
    endif()

endforeach()

#copy the build files to the  pyton directory
set(CGALBindings_pyd "${PROJECT_SOURCE_DIR}/build/CGALMethods.cp37-win_amd64.pyd")
set(CGALBindings_pdb "${PROJECT_SOURCE_DIR}/build/CGALMethods.pdb")
set(CGALBindings_destination_tests "${PROJECT_SOURCE_DIR}/tests_python/CGALMethods")

add_custom_command(
  TARGET CGALMethods POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CGALBindings_pyd} ${CGALBindings_pdb} ${CGALBindings_destination_tests}
)
