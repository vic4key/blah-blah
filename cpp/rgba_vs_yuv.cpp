bool rgba_to_yuv(
    const uint8_t* data, const size_t size, const uint32_t width, const uint32_t height,
    uint8_t* yuv_data, size_t* yuv_size)
{
    if (data == nullptr || size == 0 || width == 0 || height == 0 || yuv_data == nullptr)
    {
        return false;
    }

    *yuv_size = width * height * 3 / 2; // YUV420 requires 3/2 the size of the original
    if (*yuv_size > size)
    {
        return false;
    }

    // Calculate the offset for U and V planes
    const uint32_t uv_width = width / 2;
    const uint32_t uv_height = height / 2;
    (void)uv_height;

    // Convert RGBA to YUV420
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            const uint32_t index = (y * width + x) * 4;
            const uint8_t r = data[index];
            const uint8_t g = data[index + 1];
            const uint8_t b = data[index + 2];

            const uint8_t y_value = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
            yuv_data[y * width + x] = y_value;

            // U and V components (subsampled)
            if (y % 2 == 0 && x % 2 == 0)
            {
                const uint32_t uv_index = ((y / 2) * uv_width + (x / 2)) * 2;
                const uint8_t u_value = (uint8_t)(-0.147 * r - 0.289 * g + 0.436 * b + 128);
                const uint8_t v_value = (uint8_t)(0.615 * r - 0.515 * g - 0.100 * b + 128);
                yuv_data[width * height + uv_index] = u_value;
                yuv_data[width * height + uv_index + 1] = v_value;
            }
        }
    }

    return true;
}

bool yuv_to_rgba(
    const uint8_t* data, const size_t size, const uint32_t width, const uint32_t height,
    uint8_t* rgba_data, const size_t rgba_size)
{
    if (data == nullptr || size == 0 || rgba_data == nullptr || size < width * height * 3 / 2 || rgba_size < width * height * 4)
    {
        return false;
    }

    // Calculate the offset for U and V planes
    const uint32_t uv_width  = width / 2;
    const uint32_t uv_height = height / 2;
    (void)uv_height;

    // Convert YUV420 to RGBA
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            const uint32_t index = (y * width + x) * 4;
            const uint8_t y_value = data[y * width + x];

            // U and V components (subsampled)
            const uint32_t uv_index = ((y / 2) * uv_width + (x / 2)) * 2;
            const uint8_t u_value = data[width * height + uv_index];
            const uint8_t v_value = data[width * height + uv_index + 1];

            // Calculate R, G, B components
            const int r = (int)(y_value + 1.402 * (v_value - 128));
            const int g = (int)(y_value - 0.344 * (u_value - 128) - 0.714 * (v_value - 128));
            const int b = (int)(y_value + 1.772 * (u_value - 128));

            // Clamp values to [0, 255]
            rgba_data[index + 0] = std::clamp(r, 0, 255);
            rgba_data[index + 1] = std::clamp(g, 0, 255);
            rgba_data[index + 2] = std::clamp(b, 0, 255);
            rgba_data[index + 3] = 255; // Set alpha to 255
        }
    }

    return true;
}
