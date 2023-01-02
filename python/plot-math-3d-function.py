import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

f_z = lambda x, y: -x**2 - y**2 - 1 # f(x, y, z) = -x^2 - y^2 - z => z = -x^2 - y^2

x_axis = np.arange(-10., 10., 0.1)
y_axis = np.arange(-10., 10., 0.1)
x, y = np.meshgrid(x_axis, y_axis)
z = f_z(x, y)

fig = plt.figure()
ax = plt.axes(projection="3d")

ax.plot_surface(x, y, z)
ax.set_title("Eg. f(x, y, z) = -x^2 - y^2 - z");

ax.set_xlabel("X axis")
ax.set_ylabel("Y axis")
ax.set_zlabel("Z axis")
plt.show()
