import ctypes, ctypes.util

# @refer to `export_c_function.cpp`

import os
file_dir  = os.path.dirname(os.path.abspath(__file__))
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



# invoke a python function with c syntax

@ctypes.CFUNCTYPE(None, ctypes.c_char_p)
def print_message(message):
    msg = message.decode('utf-8')
    print(f"Invoked `print_message('{msg}')`")

pfn_c_print_message = ctypes.cast(ctypes.byref(print_message), ctypes.POINTER(ctypes.c_void_p))
print_message_c_prototype = ctypes.CFUNCTYPE(None, ctypes.c_char_p)
c_print_message = print_message_c_prototype(pfn_c_print_message.contents.value)
c_print_message(b"This is a string from Python code with C syntax")