#include <stdio.h>

#define stringify(s) #s
#define hexdump_auto_width 16
#define hexdump(d, n) printf("[%s]\n", stringify(d)); hexdump_impl(d, n, hexdump_auto_width);

void hexdump_impl(void* data, size_t size, uint max_width) // max_width should be 8, 16, 32, or 64
{
  if (data == NULL || size == 0 || max_width == 0)
  {
    return;
  }
  
  #define print_line(n) printf("--------|"); for (uint i = 0; i < n; i++) printf("---"); printf("\n");

  print_line(max_width);
  printf("Offset  |"); for (uint i = 0; i < max_width; i++) printf("%02X ", i); printf("\n");
  print_line(max_width);

  unsigned char* bytes = (unsigned char*)data;
  for (uint i = 0; i < (uint)size; i++)
  {
    if (i % max_width == 0)
    {
      if (i != 0 && i % max_width == 0) printf("\n");
      printf("%08X|%02X ", i, bytes[i]);
    }
    else
    {
      printf("%02X ", bytes[i]);
    }
  }

  printf("\n");
  print_line(max_width);
}