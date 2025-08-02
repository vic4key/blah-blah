#include <stdio.h>

typedef unsigned char __uint8_t;
typedef unsigned int  __uint32_t;

struct __attribute__((__packed__)) __rgb_t
{
  __uint8_t r;
  __uint8_t g;
  __uint8_t b;
};

struct __attribute__((__packed__)) rgb_t
{
    union __attribute__((__packed__))
    {
        struct __rgb_t rgb;
        __uint32_t rgb_3b: 24;
    };
};

struct __attribute__((__packed__)) rgba_t
{
    union __attribute__((__packed__))
    {
        struct __rgb_t rgb;
        __uint32_t rgb_3b: 24;
    };

   __uint8_t a;
};

#define print(o, z) for (int i = 0; i < z; i++) printf("%d ", ((__uint8_t*)(&o))[i]); printf("\n");

int main()
{
    printf("%zu %zu\n", sizeof(rgb_t), sizeof(rgba_t));

    struct rgba_t rgba;
    rgba.rgb.r = 1;
    rgba.rgb.g = 2;
    rgba.rgb.b = 3;
    rgba.a = 4;

    struct rgb_t rgb;
    rgb.rgb_3b = rgba.rgb_3b;

    print(rgba, 4);
    print(rgb,  3);

    return 0;
}
