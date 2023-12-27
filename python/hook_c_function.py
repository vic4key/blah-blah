from __future__ import annotations
import ctypes, ctypes.util
from dataclasses import dataclass
import binascii

PAGE_EXECUTE_READ      = 0x20
PAGE_EXECUTE_READWRITE = 0x40

@dataclass
class mem_t:
    addr: None
    size: None
    data: None
    def __bytes__(self) -> bytes: return bytes(self.data)
    def __len__(self) -> int: return self.size

@dataclass
class jmp_t: # 64-bit
    inst: bytes = bytes([0xFF, 0x25, 0x00, 0x00, 0x00, 0x00])   # jmp qword ptr ds:[rip]
    addr: bytes = bytes([0] * 8)                                # <8 bytes address>
    def __init__(self, address: int = 0): import struct; self.addr = struct.pack("Q", address)
    def __bytes__(self) -> bytes: return self.inst + self.addr
    def __len__(self) -> int: return 14

def mem_allocate(size: int) -> mem_t:
    '''
    Allocate a data block
    '''
    c_mem_allocator = ctypes.c_uint8 * size
    mem = c_mem_allocator.from_buffer(bytearray(size), 0)
    ptr = ctypes.pointer(ctypes.c_void_p(ctypes.addressof(mem)))
    return mem_t(ptr, size, mem)

def mem_protect(ptr: ctypes.c_void_p, size: int, protection: int) -> int | None:
    '''
    Set memory protection of a region of memory
    '''
    kernel32 = ctypes.CDLL(ctypes.util.find_library("kernel32"))
    previous_protection = ctypes.c_long(0)
    ret = kernel32.VirtualProtect(ptr, size, protection, ctypes.byref(previous_protection))
    assert ret != 0, "set memory protection failed"
    return previous_protection.value

def mem_write(ptr: ctypes.c_void_p, data: str | bytes):
    '''
    Write a data block to a region of memory
    '''
    size = len(data)
    # change memory protection to writable
    previous_protection = mem_protect(ptr, size, PAGE_EXECUTE_READWRITE)
    # write data block
    msvcrt = ctypes.CDLL(ctypes.util.find_library("msvcrt"))
    msvcrt.memcpy(ptr, ctypes.create_string_buffer(data), size)
    # restore the previous memory protection
    mem_protect(ptr, size, previous_protection)

def mem_read(ptr: ctypes.c_void_p, size: int) -> bytes:
    '''
    Read a data block from a region of memory
    '''
    mem = mem_allocate(size)
    # change memory protection to readable
    previous_protection = mem_protect(ptr, size, PAGE_EXECUTE_READ)
    # read data block
    msvcrt = ctypes.CDLL(ctypes.util.find_library("msvcrt"))
    msvcrt.memcpy(mem.data, ptr, size)
    # restore the previous memory protection
    mem_protect(ptr, size, previous_protection)
    # return read data
    return bytes(mem)

# @refer to `export_c_function.cpp`

import os
file_dir  = os.path.dirname(os.path.abspath(__file__))
os.environ["PATH"] += os.pathsep + file_dir

dll = ctypes.CDLL(ctypes.util.find_library("export_c_function.dll"))
# print(dll)
# print(dll.print_message)
# print(dll.c_invoke_print_message)

JUMP_SIZE = len(jmp_t())
trampoline = mem_allocate(2*JUMP_SIZE)

@ctypes.CFUNCTYPE(None, ctypes.c_char_p)
def hk_print_message(message):
    msg = message.decode('utf-8')
    message = f"Invoked `hk_print_message('{msg}')`"

    print_message_c_prototype = ctypes.CFUNCTYPE(None, ctypes.c_char_p)
    c_print_message = print_message_c_prototype(trampoline.addr.contents.value)
    c_print_message(message.encode())

def install_inline_hooking(c_function, py_function):
    pfn_c_print_message = ctypes.cast(ctypes.byref(c_function), ctypes.POINTER(ctypes.c_void_p))     # hold the actual address of `print_message` in memory
    pfn_c_hk_print_message = ctypes.cast(ctypes.byref(py_function), ctypes.POINTER(ctypes.c_void_p)) # hold the actual address of `hk_print_message` in memory
    # print(hex(pfn_c_print_message.contents.value))
    # print(hex(pfn_c_hk_print_message.contents.value))

    # create trampoline from the beginning of the function
    temp  = mem_read(pfn_c_print_message.contents, JUMP_SIZE)
    temp += bytes(jmp_t(pfn_c_print_message.contents.value + JUMP_SIZE))
    mem_write(trampoline.addr.contents, temp)
    mem_protect(trampoline.addr.contents, len(trampoline), PAGE_EXECUTE_READWRITE)
    # print(binascii.hexlify(bytes(trampoline)))

    # write jump instruction to the beginning of the function
    temp = bytes(jmp_t(pfn_c_hk_print_message.contents.value))
    mem_write(pfn_c_print_message.contents, temp)
    # print(binascii.hexlify(tramp))

install_inline_hooking(dll.print_message, hk_print_message)

dll.print_message(b"This is a string from Python code")
dll.c_invoke_print_message()

'''
Invoked `hk_print_message('This is a string from Python code')`
Invoked `hk_print_message('This is a string from C code')`
'''
