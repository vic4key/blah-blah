# pip install diskcache cachetools

from typing import Callable
from functools import wraps

def custom_cached(func_cache_key: Callable, func_cache_get: Callable, func_cache_set: Callable):
    """
    This function is a decorator that enables flexible and customizable function result caching.
    This design allows you to custom caching logic or backend, making it suitable for
        use cases where you want to control not only how the cache key is generated but also how and
        where caching is performed (e.g., memory, disk, database, or distributed cache).

    Args:
        func_cache_key (Callable): A callable that generates a unique cache key based on the function's arguments.
        func_cache_get (Callable): A callable that retrieves a cached value using the generated cache key.
        func_cache_set (Callable): A callable that stores a value in the cache with the associated cache key.

    Returns:
        Callable: A decorator that can be applied to a function to enable caching of its results.

    Usage:
        This decorator can be used to optimize functions by avoiding repeated calculations for the same inputs.
        It is particularly useful for expensive or frequently called functions where results can be reused.

    Example:
    ```
    def custom_cache_key(url) -> str | any:
        ...
        return hashkey(...)

    @custom_cached(custom_cache_key, lambda k: cache.get(k), lambda k, v: cache.set(k, v))
    def check_url(url: str) -> bool:
        ...
        return result
    ```
    """
    assert type(func_cache_key).__name__ == "function", "invalid function"
    assert type(func_cache_get).__name__ == "function", "invalid function"
    assert type(func_cache_set).__name__ == "function", "invalid function"
    def decorator(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            result = None
            if key := func_cache_key(*args, **kwargs):
                if not (result := func_cache_get(key)):
                    result = func(*args, **kwargs)
                    func_cache_set(key, result)
            if not result: result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

### Testing

from diskcache import Cache
cache = Cache('./mycache')
cache.clear()

def custom_cache_key(url):
    from cachetools.keys import hashkey
    from urllib.parse import urlparse
    domain = urlparse(url).netloc
    return hashkey(domain)

@custom_cached(custom_cache_key, lambda k: cache.get(k), lambda k, v: cache.set(k, v))
def check_url(url: str) -> bool:
    result = url.startswith("https://")
    print(f"Checking URL: {url} => {result}")
    return result

print("Số lượng items trong cache:", len(cache))
print("Các key trong cache:", list(cache.iterkeys()))
print()

check_url("https://abc.com/page1")  # Lấy từ cache, không in ra gì
check_url("https://abc.com/page2")  # Lấy từ cache, không in ra gì
check_url("https://xyz.com")        # In ra "Checking URL: ...", domain khác

print()
print("Số lượng items trong cache:", len(cache))
print("Các key trong cache:", list(cache.iterkeys()))

cache.close()