'''
For cube, convert a point to index and reverse index to a point
'''

# 2D

def point_to_index_2d(p, ncol):
	x, y = p
	return y + x * ncol

def index_to_point_2d(index, ncol):
	x, y = divmod(index, ncol)
	return (x, y)

nrow, ncol = 3, 4
index = point_to_index_2d((2, 2), ncol)
point = index_to_point_2d(index, ncol)
print(index, point)

# 3D

def index_to_point_3d(index, nrow, ncol):
	z, _ = divmod(index, nrow * ncol)
	x, y = divmod(_, ncol)
	return (x, y, z)

def point_to_index_3d(p, nrow, ncol):
	x, y, z = p
	return y + x * ncol + z * nrow * ncol

nrow, ncol, ndep = 3, 4, 5

index = point_to_index_3d((1, 2, 3), nrow, ncol)
point = index_to_point_3d(index, nrow, ncol)
print(index, point)
