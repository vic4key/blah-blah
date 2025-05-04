import threading

class ThreadSafeDict:
    def __init__(self, *args, **kwargs):
        self._dict = dict(*args, **kwargs)
        self._lock = threading.Lock()

    def __getitem__(self, key):
        with self._lock:
            return self._dict[key]

    def __setitem__(self, key, value):
        with self._lock:
            self._dict[key] = value

    def __delitem__(self, key):
        with self._lock:
            del self._dict[key]

    def __contains__(self, key):
        with self._lock:
            return key in self._dict

    def __len__(self):
        with self._lock:
            return len(self._dict)

    def get(self, key, default=None):
        with self._lock:
            return self._dict.get(key, default)

    def items(self):
        with self._lock:
            return list(self._dict.items())

    def keys(self):
        with self._lock:
            return list(self._dict.keys())

    def values(self):
        with self._lock:
            return list(self._dict.values())

    def update(self, *args, **kwargs):
        with self._lock:
            self._dict.update(*args, **kwargs)

    def pop(self, key, *args):
        with self._lock:
            return self._dict.pop(key, *args)

    def clear(self):
        with self._lock:
            self._dict.clear()

    def copy(self):
        with self._lock:
            return self._dict.copy()

    def __str__(self):
        with self._lock:
            return str(self._dict)

    def __repr__(self):
        with self._lock:
            return repr(self._dict)