import ctypes, ctypes.util

# @refer to `export_c_function.cpp`

def load_shared_library(file_name_without_extension):
    '''
    Load a shared library
    '''
    import os, platform
    platform_shared_exts = { "Windows": ".dll", "Linux": ".so", "Darwin": ".dylib" }
    file_ext  = platform_shared_exts.get(platform.system())
    file_dir  = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(file_dir, file_name_without_extension + file_ext)
    return ctypes.CDLL(file_path)

lib = load_shared_library("export_c_function")
# print(lib)
# print(lib.change_ptr_var)
# print(lib.change_ref_var)

# passing variable pointer 
var = ctypes.c_int(0)
lib.change_ptr_var(ctypes.byref(var))
print(var.value)

# passing variable reference
var = ctypes.c_int(0)
lib.change_ref_var(ctypes.byref(var))
print(var.value)

'''
<CDLL 'export_c_function.dll', handle 7ffd4f9b0000 at 0x1d406f3c788>
123
456
'''



lib.print_message(b"This is a string from Python code")
lib.c_invoke_print_message()

'''
This is a string from Python code
This is a string from C code
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

'''
Invoked `print_message('This is a string from Python code with C syntax')`
'''
