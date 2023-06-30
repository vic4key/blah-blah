import sys
import numpy as np
import PyVutils as vu

from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
from matplotlib import cm

def fn_draw_contour(ax, points):
	Xs, Ys, Zs = points[:,0], points[:,1], points[:,2]
	ax.plot3D(Xs, Ys, Zs) # c="#00FF00"

	# XsXs = np.reshape(Xs, -1)
	# YsYs = np.reshape(Ys, -1)
	# ZsZs = np.reshape(Zs, -1)

	# XsXs, YsYs = np.meshgrid(Xs, Ys)
	# Zs, _ = np.meshgrid(Zs, Ys)
	# ax.plot_surface(XsXs, YsYs, ZsZs)

	# Zs = np.reshape(Zs, (-1, 2))
	# Zs = Zs.reshape(Ys.shape)
	# Zs = Zs.reshape((len(Xs), len(Ys)))

def main():
	file_path = R"path\to\rt_struct\dicom\file"
	DS = vu.load_dicom(file_path)

	volume = DS[0x3006, 0x0039][0]
	contours = volume[0x3006, 0x0040]

	list_points = np.empty((0, 3))
	list_contour_points = []
	for contour in contours:
		lpoints = contour[0x3006, 0x0050].value
		points = np.reshape(np.array(lpoints), (-1, 3))
		list_contour_points.append(points)
		list_points = np.concatenate((list_points, points))
	# print(list_points)

	Xs, Ys, Zs = list_points[:,0], list_points[:,1], list_points[:,2]
	# print(Xs); print(Ys); print(Zs)

	fig = plt.figure(figsize=(7, 7))
	ax = plt.axes(projection="3d")
	ax.set_xlabel("X")
	ax.set_ylabel("Y")
	ax.set_zlabel("Z")
	ax.set_title("3D Contour")
	ax.axes.set_aspect("auto")

	for contour_points in list_contour_points: fn_draw_contour(ax, contour_points)
	# ax.scatter3D(Xs, Ys, Zs)
	# ax.plot_surface(Xs, Ys, Zs, cmap='jet')

	# Xs, Ys = np.meshgrid(Xs, Ys)
	# f = lambda x, y: np.sin(np.sqrt(x ** 2 + y ** 2))
	# Zs = f(Xs, Ys)
	# ax.contour3D(Xs, Ys, Zs, cmap='jet')

	# ax.plot_wireframe(X, Y, Z, color='black')

	plt.show()

if __name__ == "__main__":
	try: main()
	except (Exception, KeyboardInterrupt): vu.log_exception(sys.exc_info())
	sys.exit()