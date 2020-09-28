import numpy as np
from scipy.interpolate import LinearNDInterpolator

def map_parameterized_mesh_to_grid(mesh, npts_x=384, npts_y=256):
    """For some parameterized mesh, map the mesh onto a grid. Outputs a dictionary
    containing the parameterized data.

    Args:
        mesh ([type]): [description]
        npts_x (int, optional): [description]. Defaults to 384.
        npts_y (int, optional): [description]. Defaults to 256.

    Returns:
        [type]: [description]
    """
    mesh_points = mesh.points()

    spacing_y = 1/npts_y

    XX, YY = np.meshgrid(
        np.linspace(0, 1, npts_x), np.linspace(spacing_y, 1, npts_y))

    x = XX.flatten()
    y = YY.flatten()

    xy = np.stack([x, y], axis=-1)

    property_maps = [
        mesh.get_property(name) for name in mesh.property_names()]

    name_shape_list = [
        (name, prop.shape[-1]) for name, prop in
        zip(mesh.property_names(), property_maps)
    ]

    property_maps = np.concatenate(property_maps, axis=-1)

    interpolator = LinearNDInterpolator(
            mesh_points[:, 0:2], property_maps
    )

    interpolated = interpolator(xy)

    interpolated = interpolated.reshape(
        npts_x, npts_y, interpolated.shape[-1], order='F')

    interpolated = interpolated.swapaxes(0, -1)

    # close the artery

    output = {}

    counter = 0
    for name, shape in name_shape_list:
        output[name[2:]] = interpolated[counter:(counter+shape), :, :]
        counter += shape

    ZZ = np.zeros_like(XX)

    parameterized_points = np.stack([XX, YY, ZZ], axis=0)
    output['param_points'] = parameterized_points

    return output
