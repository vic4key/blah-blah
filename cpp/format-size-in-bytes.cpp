const char* format_size(unsigned long long bytes)
{
    static char buffer[100]; // Buffer to hold the formatted size string
    const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    const int one_unit = 1000; // 1024
    double size = bytes;
    int unit_index = 0;

    // Iterate through units until the size is less than unit-value or until we run out of units
    while (size >= one_unit && unit_index < sizeof(units) / sizeof(units[0]) - 1)
    {
        size /= one_unit;
        unit_index++;
    }

    // Format the string
    snprintf(buffer, sizeof(buffer), "%.2f %s", size, units[unit_index]);

    return buffer;
}