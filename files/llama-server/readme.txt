# Eg. load a chat model

llama-server ^
--port 1234 ^
--model path\to\lmstudio-community\Qwen3-1.7B-GGUF\Qwen3-1.7B-Q4_K_M.gguf ^
--verbose

# Eg. load a vision model

llama-server ^
--port 1234 ^
--model path\to\ngxson\Vintern-1B-v3_5-GGUF\Vintern-1B-v3_5-Q8_0.gguf ^
--mmproj path\to\ngxson\Vintern-1B-v3_5-GGUF\mmproj-Vintern-1B-v3_5-Q8_0.gguf ^
--chat-template vicuna ^
--verbose
