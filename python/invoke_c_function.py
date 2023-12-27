import ctypes, ctypes.util

# @refer to `export_c_function.cpp`

import os
file_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(file_dir, "export_c_function.dll")
dll = ctypes.CDLL(ctypes.util.find_library(file_path))
print(dll)

print(dll.change_ptr_var)
print(dll.change_ref_var)

# passing variable pointer 
var = ctypes.c_int(0)
dll.change_ptr_var(ctypes.byref(var))
print(var.value)

# passing variable reference
var = ctypes.c_int(0)
dll.change_ref_var(ctypes.byref(var))
print(var.value)

'''
<CDLL 'export_c_function.dll', handle 7ffd4f9b0000 at 0x1d406f3c788>
<_FuncPtr object at 0x000001D406A08E18>
<_FuncPtr object at 0x000001D4071C3A08>
123
456
'''