Unwrapping Many Cylindrical Meshes
----------------------------------

It might be necessary to bulk unwrap and map to a structured grid a number of meshes. To do so we can apply
the python multiprocessing library,

.. code-block:: python

   import numpy as np
   import CGALMethods as CM
   import pathlib as pt
   import config # this configure file must be supplied by the user
   import pyvista as pv
   import time
   import multiprocessing as mp
   from tqdm import tqdm

The general overview of the program will look as follows,

.. code-block:: python

   if __name__ == "__main__":
      # the total number of processors available on our desktop
      n_processors = 8
      # vtk folder and save folder provided by the user
      vtk_folder = config.vtk_folder
      save_folder = config.save_folder
      # from our folder of vtk cases grab all the subfolders
      folders = list(vtk_folder.glob('*'))
      # chunk our list of folders into the same number of groups as we have processors
      chunks = chunkIt(folders, n_processors)
      # setup the processor
      processors = [
         mp.Process(target=mp_unwrap, args=(files, save_folder))
         for files in chunks
      ]
      # start our processors and then rejoin them
      [p.start() for p in processors]
      [p.join() for p in processors]

To allow for proper multiprocessing within python the `if __name__ == "__main__"` guard is necessary.
Before implementing our multiprocessing scheme we need to divide our list of cases. Bellow we show an example chunkIt method,

.. code-block:: python

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

The folder of an OpenFoam case after exporting to vtk will look like the following,

::

   VTK
   |--INLET
      |--INLET_{time}.vtk
   |--OUTLET
      |--OUTLET_{time}.vtk
   |--WALL
      |--WALL_{time}.vtk
   |--foam_{time}.vtk

For our case we desire to extract the wss and curvature data and map them to grids. Bellow we provide an
example code which extracts our artery data, unwraps the artery and maps it to a grid, before saving it as a .npz file

.. code-block:: python

   # a file which take a path to an openfoam vtk directory and a save path
   def unwrap_foam_vtk(foam_path, savefolder):

      folder_name = foam_path.name

      inlet_mesh = foam_path/"INLET/INLET_400.vtk"
      wall_mesh = foam_path/"WALL/WALL_400.vtk"
      inlet_mesh = pv.read(inlet_mesh)

      wall_mesh = CM.SurfaceMesh(wall_mesh.as_posix())
      wall_mesh.calculate_curvature(4, 4, 300, True)

      inlet_origin = inlet_mesh.points.mean(axis=0)
      unwrapped = CM.unwrap_cylindrical_surface_mesh(wall_mesh, *inlet_origin)
      gridded = CM.map_parameterized_mesh_to_grid(unwrapped)

      output = {}
      # now scale the data such theat the neural network can obtain the data in the correct size
      max_k = gridded['maximum_curvature'] / 1000
      min_k = gridded['minimum_curvature'] / 1000

      mean_k = (max_k+min_k)/2
      gaussian_k = max_k * min_k

      output['wss'] = gridded['magWallShearStress']
      output['curvature'] = np.concatenate(
            [min_k, max_k, mean_k,gaussian_k],
            axis=0)
      output['points'] = gridded['original_points'] * 1000

      np.savez(
            savefolder/folder_name,
            **output
      )

      unwrapped.write_vtk((savefolder/(folder_name+".vtk")).as_posix())

Once we have generate the main method we need a function which acts as the target to our multiprocessing method

.. code-block:: python

   def mp_unwrap(files, savepath):
      for folder in tqdm(files):
         try:
               unwrap_foam_vtk(folder, savepath)
         except:
               print(folder, "failed")
               with open(savepath/'log.txt', "a") as f:
                  f.write(folder.as_posix()+"\n")

we provide a logging method for any cases which fail to unwrap properly