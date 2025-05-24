<?php

error_reporting(E_ALL);
ini_set('display_errors', 1);

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type');

require_once 'http_request_openai.php';
require_once 'file_caching.php';

$cache = new FileCaching('compute_capability_and_cuda_versions');

try {
    $compute_capability = $_GET['compute_capability'] ?? '';
    if (empty($compute_capability)) {
        $items = [];
        echo json_encode(['items' => $items]);
        exit;
    }

    $items = $cache->get($compute_capability);
    if ($items !== null) {
        echo json_encode(['items' => $items]);
        exit;
    }

    $api_url = 'https://api.deepseek.com/chat/completions';
    $api_key = 'your-openai-key';
    $model_name  = 'deepseek-chat';
    $user_prompt = "Help me to list all CUDA versions that supported GPU card with {$compute_capability} Compute Capability.
    The response return only a JSON list, and without any description or detail as the below:
    {
        'items': [
            '11.1',
            '11.2',
            '11.3',
            '11.4',
            '11.4',
            '...',
            '12.0 and later versions'
        ]
    }";

    $content = http_request_openai_completions($api_url, $api_key, $model_name, $user_prompt);
    $content = substr($content, 7, -3);
    $content = json_decode($content, true);

    if (json_last_error() !== JSON_ERROR_NONE) {
        throw new Exception('Invalid JSON content in API response: ' . json_last_error_msg());
    }

    $items = $content['items'] ?? [];

    $cache->store($compute_capability, $items);

    echo json_encode(['items' => $items]);

} catch (Exception $e) {
    http_response_code(500);
    echo json_encode([
        'error' => $e->getMessage()
    ]);
}

?>