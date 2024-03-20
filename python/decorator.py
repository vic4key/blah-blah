import functools



def decorator_without_params(func): # without params: wrapper(...) only
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        print(f"\t{func.__name__} invoked with", *args, *kwargs, f"then result is {result}")
        return result
    return wrapper

@decorator_without_params
def test_decorator_without_params(arg_1, arg_2, arg_3):
    '''
    this is `test_decorator_without_params(arg_1, arg_2, arg_3)`
    '''
    print("test_decorator_without_params", arg_1, arg_2, arg_3)
    return arg_1 + arg_2 + arg_3



def decorator_with_params(params): # with params: wrapper(...) inside decorator(...)
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            result = func(*args, **kwargs)
            print(f"\t{func.__name__} invoked with", *args, *kwargs, f"and params is {params}", f"then result is {result}")
            return result
        return wrapper
    return decorator

@decorator_with_params(params=123)
def test_decorator_with_params(arg_1, arg_2, arg_3):
    '''
    this is `test_decorator_with_params(arg_1, arg_2, arg_3)`
    '''
    print("test_decorator_with_params", arg_1, arg_2, arg_3)
    return arg_1 + arg_2 + arg_3



def main():
    print(" test_decorator_without_params ".center(100, "-"))
    test_decorator_without_params(1, 2, 3)
    print("--- help ---\n", test_decorator_without_params.__doc__.strip(), "\n--- help ---\n")

    print(" test_decorator_with_params ".center(100, "-"))
    test_decorator_with_params(1, 2, 3)
    print("--- help ---\n", test_decorator_with_params.__doc__.strip(), "\n--- help ---\n")

if __name__ == "__main__": main()



''' Output:
-----------------------------------test_decorator_without_params------------------------------------  
test_decorator_without_params 1 2 3
        test_decorator_without_params invoked with 1 2 3 then result is 6
--- help ---
 this is `test_decorator_without_params(arg_1, arg_2, arg_3)`
--- help ---

-------------------------------------test_decorator_with_params-------------------------------------  
test_decorator_with_params 1 2 3
        test_decorator_with_params invoked with 1 2 3 and params is 123 then result is 6
--- help ---
 this is `test_decorator_with_params(arg_1, arg_2, arg_3)`
--- help ---
'''