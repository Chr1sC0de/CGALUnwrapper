import CGALMethods as CM
import matplotlib.pyplot as plt
import numpy as np

if __name__ == "__main__":
    surface_mesh = CM.SurfaceMesh("resources/WALL_400.vtk", scale=1000)

    unwrapped = CM.unwrap_cylindrical_surface_mesh(surface_mesh)
    unwrapped.write_vtk("resources/WALL_400_python_write.vtk")

    mapped_to_grid = CM.map_parameterized_mesh_to_grid(unwrapped)

    for key, value in mapped_to_grid.items():
        print("property", key, "has shape", value.shape)

    np.save('resources/structured_grids', mapped_to_grid)

    plt.imshow(mapped_to_grid['magWallShearStress'][0])
    plt.show()


