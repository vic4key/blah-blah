# Curve Fitting

# https://www.scipy-lectures.org/advanced/mathematical_optimization/auto_examples/plot_curve_fit.html#sphx-glr-advanced-mathematical-optimization-auto-examples-plot-curve-fit-py

import numpy as np
from scipy import optimize
import pylab as pl

np.random.seed(0)

# Our test function
def f(t, omega, phi):
    return np.cos(omega * t + phi)

# Our x and y data
x = np.linspace(0, 3, 50)
y = f(x, 1.5, 1) + .1*np.random.normal(size=50)

# Fit the model: the parameters omega and phi can be found in the
# `params` vector
params, params_cov = optimize.curve_fit(f, x, y)

# plot the data and the fitted curve
t = np.linspace(0, 3, 1000)

pl.figure(1)
pl.clf()
pl.plot(x, y, 'bx')
pl.plot(t, f(t, *params), 'r-')
pl.show()