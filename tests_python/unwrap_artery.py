import CGALMethods as CM

if __name__ == "__main__":
    surface_mesh = CM.SurfaceMesh("resources/WALL_400.vtk", scale=1000)
    print("the original mesh:\n", surface_mesh)
    unwrapped = CM.unwrap_cylindrical_surface_mesh(surface_mesh)
    unwrapped.write_vtk("resources/WALL_400_python_write.vtk")
    print("the unwrapped mesh:\n", unwrapped)

    # now we can specify a the inlet origin where we wish to perform the unwrapping
    unwrapped_w_pt = CM.unwrap_cylindrical_surface_mesh(surface_mesh, [0,0,-21])
    unwrapped_w_pt.write_vtk("resources/WALL_400_python_write.vtk")
    print("the unwrapped_w_pt mesh:\n", unwrapped)