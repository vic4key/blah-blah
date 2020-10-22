'''
For cube, convert a point to index and reverse index to a point
'''

def index_to_point(index, nrow, ncol):
	z, _ = divmod(index, nrow * ncol)
	x, y = divmod(_, ncol)
	return (x, y, z)

def point_to_index(p, nrow, ncol):
	x, y, z = p
	return y + x * ncol + z * nrow * ncol

nrow, ncol = 3, 4

index = point_to_index((2, 1, 3), nrow, ncol)
point = index_to_point(index, nrow, ncol)
print(index, point)