import CGALMethods as CM
import pyvista as pv

if __name__ == "__main__":

    surface_mesh = CM.SurfaceMesh("resources/WALL_Bifurcation_original.vtk", scale=1000)

    # print("the original mesh:\n", surface_mesh)
    # unwrapped = CM.unwrap_cylindrical_surface_mesh(surface_mesh)
    # unwrapped.write_vtk("resources/WALL_bifurcation_write_no_inlet.vtk")
    # print("the unwrapped mesh:\n")

    # now we can specify a the inlet origin where we wish to perform the unwrapping

    inlet_mesh      = pv.read("resources/Inlet_Bifurcation_original.vtk")
    outlet_mesh     = pv.read("resources/Outlet_Bifurcation_original.vtk")
    inlet_centroid  = inlet_mesh.points.mean(0)*1000
    outlet_centroid = outlet_mesh.points.mean(0)*1000

    unwrapped_w_pt = CM.unwrap_cylindrical_surface_mesh(
        surface_mesh, *inlet_centroid, *outlet_centroid)
    unwrapped_w_pt.write_vtk("resources/WALL_Bifurcation_with_inlet.vtk")

    # no we can map the mesh to a grid
    parameterized = CM.map_parameterized_mesh_to_grid(
        unwrapped_w_pt, npts_x=1024, npts_y=1024)

    parameterized_points = unwrapped_w_pt.get_property("v:original_points")

    # grid_1 = pv.PolyData(parameterized_points)
    # grid_1.plot()

    grid_2 = pv.StructuredGrid(
        *parameterized["param_points"]
    )
    grid_2.point_arrays["magWallShearStress"] = parameterized["magWallShearStress"].flatten("F")
    grid_2.save("resources/bifurcation_flattened_and_mapped_to_grid.vtk")
    pass