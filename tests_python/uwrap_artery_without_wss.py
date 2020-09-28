import numpy as np
import CGALMethods as CM
import pathlib as pt
import config2
import pyvista as pv
import time
import multiprocessing as mp
from tqdm import tqdm

def chunkIt(seq, num):
        avg = len(seq) // num
        out = []

        for i in list(range(num)):
                k = i*avg
                if i < (num-1):
                        out.append(seq[k:(k+avg)])
                else:
                        out.append(seq[k:])
        return out

def unwrap_foam_vtk(foam_path, savefolder):
    folder_name = foam_path.name

    if not (savefolder/(folder_name+".vtk")).exists():
        foam_path = foam_path/"VTK"
        inlet_mesh = foam_path/"INLET/INLET_0.vtk"
        wall_mesh = foam_path/"WALL/WALL_0.vtk"
        inlet_mesh = pv.read(inlet_mesh)

        wall_mesh = CM.SurfaceMesh(wall_mesh.as_posix(), scale=1000)
        wall_mesh.calculate_curvature(4, 4, 300, True)

        inlet_origin = inlet_mesh.points.mean(axis=0) * 1000
        unwrapped = CM.unwrap_cylindrical_surface_mesh(wall_mesh, *inlet_origin)
        gridded = CM.map_parameterized_mesh_to_grid(unwrapped)

        output = {}
        # now scale the data such theat the neural network can obtain the data in the correct size
        max_k = gridded['maximum_curvature']
        min_k = gridded['minimum_curvature']

        mean_k = (max_k+min_k)/2
        gaussian_k = max_k * min_k

        output['curvature'] = np.concatenate(
            [min_k, max_k, mean_k,gaussian_k],
            axis=0)
        output['points'] = gridded['original_points']
        # complete the check and the write
        np.savez(
            savefolder/folder_name,
            **output
        )

        unwrapped.write_vtk((savefolder/(folder_name+".vtk")).as_posix())

        del unwrapped
        del wall_mesh

def mp_unwrap(files, savepath):
    for folder in tqdm(files):
        # if int(folder.name) == 66:
            try:
                unwrap_foam_vtk(folder, savepath)
            except:
                print(folder, "failed")
                with open(savepath/'log.txt', "a") as f:
                    f.write(folder.as_posix()+"\n")


if __name__ == "__main__":

    n_processors = 5

    vtk_folder = config2.vtk_folder
    save_folder = config2.save_folder

    folders = list(vtk_folder.glob('0*'))
    folders = [
        folder for folder in folders if (folder/"VTK").exists()
    ]

    chunks = chunkIt(folders, n_processors)

    mp_unwrap(folders, save_folder)

    # processors = [
    #     mp.Process(target=mp_unwrap, args=(files, save_folder))
    #     for files in chunks
    # ]

    # [p.start() for p in processors]
    # [p.join() for p in processors]