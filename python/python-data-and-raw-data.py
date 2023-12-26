# https://docs.python.org/3/library/ctypes.html
# https://docs.python.org/3/library/ctypes.html#fundamental-data-types

# https://docs.python.org/3/library/struct.html
# https://docs.python.org/3/library/struct.html#format-characters



# Pack/Unpack between Python Data Types & Raw Data Bytes

import struct, binascii

b  = b""
b += struct.pack("Q", 0x1122334455667788) # unsigned long long
b += struct.pack("L", 0xAABBCCDD) # unsigned long
b += struct.pack("H", 0xEEFF) # unsigned long
print(b) # <class 'bytes'>
print(binascii.hexlify(b))

'''
b'\x88wfUD3"\x11\xdd\xcc\xbb\xaa\xff\xee'
b'8877665544332211ddccbbaaffee'
'''

l = list(struct.unpack("QLH", b))
print([hex(e) for e in l])

'''
['0x1122334455667788', '0xaabbccdd', '0xeeff']
'''



import ctypes, ctypes.util

# Create a raw data buffer (C's bytes)
s = ctypes.create_string_buffer(b) # <class 'ctypes.c_char_Array_15'>
print(s) # <class 'bytes'>

'''
<ctypes.c_char_Array_15 object at 0x000001F4049DF9C8>
'''

# Calculate size in bytes of a C data type
print("ctypes.c_void_p =", ctypes.sizeof(ctypes.c_void_p))

'''
ctypes.c_void_p = 8
'''

# Find the raw address of an exported C-function 

user32 = ctypes.CDLL(ctypes.util.find_library("user32"))

temp_1 = user32.MessageBoxA # <class 'ctypes.CDLL.__init__.<locals>._FuncPtr'>
print("Python Object (_FuncPtr):", temp_1)

temp_2 = ctypes.byref(temp_1) # <class 'CArgObject'> # Convert Python variable to Python pointer

temp_3 = ctypes.cast(temp_2, ctypes.POINTER(ctypes.c_void_p)) # <class 'LP_c_void_p'>   Cast Python pointer to C pointer (Eg. void* temp_3 = &temp_1)
print("Python Object (LP_c_void_p):", temp_3)

temp_4 = temp_3.contents.value # <class 'int'>  Get C pointer to C value (Eg. int temp_4 = *temp_3)
print("Raw Function Pointer of `MessageBoxA` at", hex(temp_4))

'''
Python Object (_FuncPtr): <_FuncPtr object at 0x00000141381C8D48>
Python Object (LP_c_void_p): <__main__.LP_c_void_p object at 0x00000141381EF6C8>
Raw Function Pointer of `MessageBoxA` at 0x7ffd5aec1d40 # This is actual address of MessageBoxA function in memory
'''

# Invoke a C-function by raw-address of the function from Python
MessageBoxA_C_Prototype = ctypes.CFUNCTYPE( # <class '_ctypes.PyCFuncPtrType'>
    ctypes.c_int, # return
    ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_ulong # arguments
)
MessageBoxA = MessageBoxA_C_Prototype(temp_4) # <class 'ctypes.CFUNCTYPE.<locals>.CFunctionType'> # Cast raw-address to Python ctypes 
MessageBoxA(0, b"text", b"title", 0) # Invoke the function
