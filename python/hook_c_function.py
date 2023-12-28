from __future__ import annotations
from dataclasses import dataclass
import ctypes, ctypes.util
import capstone

from os_spec import *

LIBC = "c" if POSIX else "msvcrt"

if POSIX: # linux memory protection
    PROT_NONE  = 0
    PROT_READ  = 1
    PROT_WRITE = 2
    PROT_EXEC  = 4
    # wrapper for linux
    PAGE_NOACCESS           = PROT_NONE
    PAGE_READONLY           = PROT_READ
    PAGE_READWRITE          = PROT_READ | PROT_WRITE
    PAGE_EXECUTE_READ       = PROT_EXEC | PROT_READ
    PAGE_EXECUTE_READWRITE  = PAGE_EXECUTE_READ | PROT_WRITE
else: # windows memory protection
    PAGE_NOACCESS           = 0x01
    PAGE_READONLY           = 0x02
    PAGE_READWRITE          = 0x04
    PAGE_EXECUTE            = 0x10
    PAGE_EXECUTE_READ       = 0x20
    PAGE_EXECUTE_READWRITE  = 0x40

@windows
def mem_protect(ptr: ctypes.c_void_p, size: int, protection: int) -> int | None:
    '''
    Set memory protection of a region of memory
    '''
    kernel32 = ctypes.CDLL(ctypes.util.find_library("kernel32"))
    previous_protection = ctypes.c_long(0)
    ret = kernel32.VirtualProtect(ptr, size, protection, ctypes.byref(previous_protection))
    assert ret != 0, "set memory protection failed"
    return previous_protection.value

@linux
def mem_protect(ptr: ctypes.c_void_p, size: int, protection: int) -> int | None:
    '''
    Set memory protection of a region of memory
    '''
    PAGE_SIZE = 0x1000
    # align on a page boundary 
    addr = ptr.value
    mem_addr = addr & ~(PAGE_SIZE - 1)
    mem_end  = (addr + size) & ~(PAGE_SIZE - 1)
    if (addr + size) > mem_end: mem_end += PAGE_SIZE
    mem_len = mem_end - mem_addr
    ptr  = ctypes.c_void_p(mem_addr)
    size = mem_len
    # set protection to aligned page boundary
    libc = ctypes.CDLL(ctypes.util.find_library("c"))
    ret = libc.mprotect(ptr, size, protection)
    assert ret == 0, "set memory protection failed"
    return protection # TODO: return previous memory protection

@dataclass
class mem_t:
    addr: None
    size: None
    data: None
    def __bytes__(self) -> bytes: return bytes(self.data)
    def __len__(self) -> int: return self.size

def mem_allocate(size: int) -> mem_t:
    '''
    Allocate a data block
    '''
    c_mem_allocator = ctypes.c_uint8 * size
    mem = c_mem_allocator()
    ptr = ctypes.pointer(ctypes.c_void_p(ctypes.addressof(mem)))
    return mem_t(ptr, size, mem)

def mem_write(ptr: ctypes.c_void_p, data: str | bytes):
    '''
    Write a data block to a region of memory
    '''
    size = len(data)
    # change memory protection to writable
    previous_protection = mem_protect(ptr, size, PAGE_EXECUTE_READWRITE)
    # write data block
    libc = ctypes.CDLL(ctypes.util.find_library(LIBC))
    libc.memcpy(ptr, ctypes.create_string_buffer(data), size)
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
    libc = ctypes.CDLL(ctypes.util.find_library(LIBC))
    libc.memcpy(mem.data, ptr, size)
    # restore the previous memory protection
    mem_protect(ptr, size, previous_protection)
    # return read data
    return bytes(mem)



def calculate_actual_instruction_sizes(
        bytes, needed_size, arch = capstone.CS_ARCH_X86, mode = capstone.CS_MODE_64) -> int:
    '''
    Calculate the actual size based on disassembled instructions
    '''
    result = 0
    cs = capstone.Cs(arch, mode)
    for i in cs.disasm(bytes, 0):
        result += i.size
        if result >= needed_size: break
    return result

def print_hexlify(data):
    import binascii
    if type(data) is bytes: print(binascii.hexlify(data))
    elif type(data) is str: print(binascii.hexlify(data.encode()))
    elif type(data) is int: print("0x%016X" % data)
    else: assert False, "unknown data"

@dataclass
class jmp_t: # 64-bit
    inst: bytes = bytes([0xFF, 0x25, 0x00, 0x00, 0x00, 0x00])   # jmp qword ptr ds:[rip]
    addr: bytes = bytes([0] * 8)                                # <8 bytes address>
    def __init__(self, address: int = 0): import struct; self.addr = struct.pack("Q", address)
    def __bytes__(self) -> bytes: return self.inst + self.addr
    def __len__(self) -> int: return 14

# allocate trampoline
JUMP_SIZE  = len(jmp_t())
FREE_SIZE  = 0x100 # reserve for backup instructions
trampoline = mem_allocate(JUMP_SIZE + FREE_SIZE)
mem_protect(trampoline.addr.contents, len(trampoline), PAGE_EXECUTE_READWRITE)

# The C prototype of the `print_message` function
print_message_C_Prototype = ctypes.CFUNCTYPE(None, ctypes.c_char_p)

@print_message_C_Prototype
def hk_print_message(message):
    message = f"Invoked `hk_print_message('{message.decode('utf-8')}')`"
    # invoke the original function
    c_print_message = print_message_C_Prototype(trampoline.addr.contents.value)
    c_print_message(message.encode())

def install_inline_hooking(c_function, py_function):
    pfn_c_print_message = ctypes.cast(ctypes.byref(c_function), ctypes.POINTER(ctypes.c_void_p))     # hold the actual address of `print_message` in memory
    pfn_c_hk_print_message = ctypes.cast(ctypes.byref(py_function), ctypes.POINTER(ctypes.c_void_p)) # hold the actual address of `hk_print_message` in memory
    # print_hexlify(pfn_c_print_message.contents.value)
    # print_hexlify(pfn_c_hk_print_message.contents.value)

    # create trampoline from the beginning of the function
    MAX_INST_SIZE = 0xF # assume this value for all archs and all modes
    temp  = mem_read(pfn_c_print_message.contents, JUMP_SIZE + MAX_INST_SIZE)
    size_of_backup_instructions = calculate_actual_instruction_sizes(temp, JUMP_SIZE)
    temp  = temp[0:size_of_backup_instructions]
    temp += bytes(jmp_t(pfn_c_print_message.contents.value + len(temp)))
    mem_write(trampoline.addr.contents, temp)
    # print_hexlify(temp)

    # write jump instruction to the beginning of the function
    temp = bytes(jmp_t(pfn_c_hk_print_message.contents.value))
    mem_write(pfn_c_print_message.contents, temp)
    # print_hexlify(temp)



# @refer to `export_c_function.cpp`

lib = load_shared_library("export_c_function")
# print(lib)
# print(lib.print_message)
# print(lib.c_invoke_print_message)

install_inline_hooking(lib.print_message, hk_print_message)

lib.print_message(b"This is a string from Python code")
lib.c_invoke_print_message()

'''
Invoked `hk_print_message('This is a string from Python code')`
Invoked `hk_print_message('This is a string from C code')`
'''
