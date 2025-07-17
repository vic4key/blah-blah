from cachetools import cached, LRUCache
from cachetools.keys import hashkey

cache = LRUCache(maxsize=128)

def my_cache_key(url):
    # Ví dụ: chỉ cache theo domain
    from urllib.parse import urlparse
    domain = urlparse(url).netloc
    return hashkey(domain)

@cached(cache, key=my_cache_key)
def check_url(url: str) -> bool:
    print(f"Checking URL: {url}")
    return url.startswith("https://")

print("Số lượng items trong cache:", len(cache))
print("Các key trong cache:", list(cache.keys()))

print(check_url("https://abc.com/page1"))  # In ra "Checking URL: ..."
print(check_url("https://abc.com/page2"))  # KHÔNG in ra gì, vì domain giống nhau nên lấy từ cache
print(check_url("https://xyz.com"))        # In ra "Checking URL: ...", domain khác

print("Số lượng items trong cache:", len(cache))
print("Các key trong cache:", list(cache.keys()))
