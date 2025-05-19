import threading
from collections import OrderedDict
from typing import Any, Optional, TypeVar, Generic

K = TypeVar('K')
V = TypeVar('V')

class ThreadSafeDict(Generic[K, V]):
    """
    A thread-safe dictionary implementation that wraps OrderedDict with a lock.
    """
    def __init__(self):
        self.__dict: OrderedDict[K, V] = OrderedDict()
        self.__lock: threading.Lock = threading.Lock()

    def __enter__(self):
        self.__lock.acquire()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.__lock.release()

    def set(self, key: K, value: V) -> None:
        """Thread-safe set operation"""
        with self.__lock:
            self.__dict[key] = value

    def get(self, key: K, default: Optional[V] = None) -> Optional[V]:
        """Thread-safe get operation"""
        with self.__lock:
            return self.__dict.get(key, default)

    def contains(self, key: K) -> bool:
        """Thread-safe contains operation"""
        with self.__lock:
            return key in self.__dict

    def remove(self, key: K) -> None:
        """Thread-safe remove operation"""
        with self.__lock:
            if key in self.__dict:
                del self.__dict[key]

    def clear(self) -> None:
        """Thread-safe clear operation"""
        with self.__lock:
            self.__dict.clear()

    def items(self) -> list[tuple[K, V]]:
        """Thread-safe items operation"""
        with self.__lock:
            return list(self.__dict.items())

    def keys(self) -> list[K]:
        """Thread-safe keys operation"""
        with self.__lock:
            return list(self.__dict.keys())

    def values(self) -> list[V]:
        """Thread-safe values operation"""
        with self.__lock:
            return list(self.__dict.values()) 