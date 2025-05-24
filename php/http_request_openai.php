<?php

require_once 'curl.php';

/**
 * Make a request to OpenAI completions API
 *
 * @param string  $api_url The endpoint of the completions API.
 * @param string  $api_key The API key for authentication.
 * @param string  $model_name The model name.
 * @param array   $messages Array of message objects with 'role' and 'content'.
 * @param int     $timeout The timeout in seconds (default: 30).
 * @return string The content from the API response.
 * @throws Exception If the API request fails or response is invalid.
 */
function http_request_openai_completions_impl($api_url, $api_key, $model_name, $messages, $timeout = 30)
{
    $data = json_encode([
        'model' => $model_name,
        'messages' => $messages,
        'stream' => false
    ]);

    $headers = [
        'Content-Type: application/json',
        'Authorization: Bearer ' . $api_key
    ];

    $resp = cURL('POST', $api_url, $headers, $data, $timeout);
    if ($resp === false)
    {
        throw new Exception('Failed to get response from API');
    }

    $response = json_decode($resp, true);
    if (json_last_error() !== JSON_ERROR_NONE)
    {
        throw new Exception('Invalid JSON response from API: ' . json_last_error_msg());
    }

    if (!isset($response['choices'][0]['message']['content']))
    {
        throw new Exception('Unexpected API response format');
    }

    $content = $response['choices'][0]['message']['content'];

    return $content;
}

/**
 * Make a request to OpenAI completions API with user and system prompt.
 *
 * @param string  $api_url The endpoint of the completions API.
 * @param string  $api_key The API key for authentication.
 * @param string  $model_name The model name.
 * @param string  $usr_prompt The user's prompt message
 * @param string  $sys_prompt The system prompt message (default: 'You are a helpful assistant.')
 * @param int     $timeout The timeout in seconds (default: 30).
 * @return string The content from the API response.
 * @throws Exception If the API request fails or response is invalid.
 */
function http_request_openai_completions($api_url, $api_key, $model_name, $usr_prompt, $sys_prompt = 'You are a helpful assistant.', $timeout = 30)
{
    $messages = [
        ['role' => 'system', 'content' => $sys_prompt],
        ['role' => 'user', 'content' => $usr_prompt]
    ];

    return http_request_openai_completions_impl($api_url, $api_key, $model_name, $messages, $timeout);
}

/*

Usage:

$api_url = 'https://api.deepseek.com/chat/completions';
$api_key = 'your-openai-key';
$user_prompt = 'Who are you?';
$content = http_request_openai_completions($api_url, $api_key, $user_prompt);
print_r($content);

*/

?>