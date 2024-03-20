import functools



def decorator_without_params(func): # without params: wrapper(...) only
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        print("\t", func, args, kwargs, result)
        return result
    return wrapper

def decorator_with_params(params): # with params: wrapper(...) inside decorator(...)
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            result = func(*args, **kwargs)
            print("\t", func, args, kwargs, result, params)
            return result
        return wrapper
    return decorator

@decorator_without_params
def test_decorator_without_params(arg_1, arg_2, arg_3):
    '''
    this is `test_decorator_without_params(arg_1, arg_2, arg_3)`
    '''
    print("test_decorator_without_params", arg_1, arg_2, arg_3)
    return arg_1 + arg_2 + arg_3



@decorator_with_params(params=123)
def test_decorator_with_params(arg_1, arg_2, arg_3):
    '''
    this is `test_decorator_with_params(arg_1, arg_2, arg_3)`
    '''
    print("test_decorator_with_params", arg_1, arg_2, arg_3)
    return arg_1 + arg_2 + arg_3

def main():
    print("test_decorator_without_params".center(100, "-"))
    print(help(test_decorator_without_params))
    test_decorator_without_params(1, 2, 3)

    print("test_decorator_with_params".center(100, "-"))
    print(help(test_decorator_with_params))
    test_decorator_with_params(1, 2, 3)



if __name__ == "__main__": main()



''' Output:
-----------------------------------test_decorator_without_params------------------------------------
Help on function test_decorator_without_params in module __main__:

test_decorator_without_params(arg_1, arg_2, arg_3)
    this is `test_decorator_without_params(arg_1, arg_2, arg_3)`

None
test_decorator_without_params 1 2 3
         <function test_decorator_without_params at 0x0000027C90D3E2A0> (1, 2, 3) {} 6
-------------------------------------test_decorator_with_params-------------------------------------
Help on function test_decorator_with_params in module __main__:

test_decorator_with_params(arg_1, arg_2, arg_3)
    this is `test_decorator_with_params(arg_1, arg_2, arg_3)`

None
test_decorator_with_params 1 2 3
         <function test_decorator_with_params at 0x0000027C90DA60C0> (1, 2, 3) {} 6 123
'''