<?php

/**
 * FileCaching class for handling file-based caching operations
 */
class FileCaching {
    private $_directory;
    private $_expired;
    private $_category;

    /**
     * Constructor
     *
     * @param string $category Category prefix for cache files
     * @param int|null $cache_expiry Cache expiration time in seconds (default: null - no expiration)
     * @param string $cache_dir Directory to store cache files (default: 'cache')
     */
    public function __construct($category, $cache_expiry = null, $cache_dir = 'cache') {
        $this->_directory = __DIR__ . '/' . $cache_dir;
        $this->_expired = $cache_expiry;
        $this->_category = $category;
        $this->_ensure_directory();
    }

    /**
     * Ensure cache directory exists
     */
    private function _ensure_directory() {
        if (!file_exists($this->_directory)) {
            mkdir($this->_directory, 0777, true);
        }
    }

    /**
     * Get file path for a key
     *
     * @param string $key The key value
     * @return string The file path
     */
    private function _get_file($key) {
        return $this->_directory . '/' . $this->_category . '_' . $key . '.json';
    }

    /**
     * Get result for a key
     *
     * @param string $key The key value
     * @return array|null The result or null if not found/expired
     */
    public function get($key) {
        if ($key == null || empty($key)) {
            return null;
        }

        $file = $this->_get_file($key);
        if (!file_exists($file)) {
            return null;
        }

        try {
            $content = file_get_contents($file);
            if ($content === false) {
                return null;
            }

            $data = json_decode($content, true);
            if (!$data) {
                return null;
            }

            // Check if expired (skip if _expired is null)
            if ($this->_expired !== null) {
                $time_diff = time() - $data['timestamp'];
                if ($time_diff > $this->_expired) {
                    unlink($file);
                    return null;
                }
            }

            return $data['data'];
        } catch (\Exception $e) {
            // Log error if needed
            return null;
        }
    }

    /**
     * Store result
     *
     * @param string $key The key value
     * @param array $data The data to store
     * @return bool True if successful, false otherwise
     */
    public function store($key, $data) {
        if ($key == null || empty($key) || $data === null) {
            return false;
        }

        try {
            $file = $this->_get_file($key);
            $cache_data = [
                'timestamp' => time(),
                'data' => $data
            ];
            $result = file_put_contents($file, json_encode($cache_data));
            return $result !== false;
        } catch (\Exception $e) {
            // Log error if needed
            return false;
        }
    }
}

/*

$cache = new FileCaching('example');

$data = $cache->get($key); // Get data from the cache file in the next call
if ($data !== null) {
    echo json_encode(['data' => $data]);
    exit;
}

// your code here

$cache->store($key, $data); // Store data in the first call to cache file

*/

?>