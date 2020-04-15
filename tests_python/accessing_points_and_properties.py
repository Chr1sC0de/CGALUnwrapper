import CGALMethods as CM
import numpy as np

if __name__ == "__main__":
    surface_mesh = CM.SurfaceMesh("resources/WALL_400.vtk", scale=1000)
    print("the surfacemesh points", surface_mesh.points().shape)
    for name in surface_mesh.property_names():
        prop = surface_mesh.get_property(name)
        print(name, "has shape", prop.shape)