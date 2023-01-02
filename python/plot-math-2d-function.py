import numpy as np
import matplotlib.pyplot as plt

# Math Equation Example
'''
f = lambda x, y: 2*x**3 - 3*y**2 - 8

x_axis = np.arange(0., 10., 0.1)
y_axis = np.arange(-10., 10., 0.1)
x, y = np.meshgrid(x_axis, y_axis)

plt.contour(x_axis, y_axis, f(x, y), [0])
plt.title("Eg. f(x, y) = 2x^3 + 3y^2 - 8")
'''

# Elliptic Curve

f = lambda x, y: x**3 -3*x +3 - y**2 # y^2 = x^3 - 3x - 3 => f(x, y) = x^3 - 3x - 3 - y^2

x_axis = np.arange(-10., 10., 0.1)
y_axis = np.arange(-10., 10., 0.1)
x, y = np.meshgrid(x_axis, y_axis)

plt.contour(x_axis, y_axis, f(x, y), [0])
plt.title("Elliptic Curve. Eg. y^2 = x^3 - 3x - 3")

# View Configurations

plt.grid(alpha=1.0, linestyle ="--")
plt.xlabel("X axis")
plt.ylabel("Y axis")
plt.show()
