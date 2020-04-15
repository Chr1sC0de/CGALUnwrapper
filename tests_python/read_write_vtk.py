import CGALMethods as CM

if __name__ == "__main__":
    surface_mesh = CM.SurfaceMesh("resources/WALL_400.vtk")
    surface_mesh.write_vtk("resources/WALL_400_python_write.vtk")
    print(surface_mesh)