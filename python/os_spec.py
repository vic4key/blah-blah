import os, sys, platform

POSIX = platform.system() in ["Linux", "Darwin"]
PLATFORM_SHARED_EXTENSION = { "Windows": ".dll", "Linux": ".so", "Darwin": ".dylib" }

def _ifdef_decorator_impl(plat, func, frame):
    if platform.system() == plat:
        return func
    elif func.__name__ in frame.f_locals:
        return frame.f_locals[func.__name__]
    else:
        def _not_implemented(*args, **kwargs):
            raise NotImplementedError(
                f"Function {func.__name__} is not defined "
                f"for platform {platform.system()}.")
        return _not_implemented
            
def windows(func):
    return _ifdef_decorator_impl('Windows', func, sys._getframe().f_back)
    
def linux(func):
    return _ifdef_decorator_impl('Linux', func, sys._getframe().f_back)

def macos(func):
    return _ifdef_decorator_impl('Darwin', func, sys._getframe().f_back)

def load_shared_library(file_name_without_extension):
    '''
    Load a shared library
    '''
    file_ext  = PLATFORM_SHARED_EXTENSION.get(platform.system())
    file_dir  = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(file_dir, file_name_without_extension + file_ext)
    from ctypes import CDLL
    return CDLL(file_path)
