import sys
import platform

import platform
POSIX = platform.system() in ["Linux"]

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