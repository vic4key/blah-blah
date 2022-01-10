#include <iostream>
#include <cassert>
#include <vector>
#include <vu>
#include <SOIL2/SOIL2.h>

#pragma pack(1)

int main()
{
  struct rgb_t
  {
    vu::byte r, g, b;
  };

  int width = 0;
  int height = 0;
  int channel = 0;
  auto ptr_pixels = SOIL_load_image("data\\test.bmp", &width, &height, &channel, SOIL_LOAD_AUTO);
  assert(ptr_pixels != nullptr);

  auto ptr_rgb_pixels = reinterpret_cast<rgb_t*>(ptr_pixels);
  int  num_pixels = width * height;
  std::vector<vu::byte> gray_pixels(num_pixels);

  for (int i = 0; i < num_pixels; i++)
  {
    auto rgb_pixel = ptr_rgb_pixels[i];
    gray_pixels[i] = vu::byte((0.3 * rgb_pixel.r) + (0.59 * rgb_pixel.g) + (0.11 * rgb_pixel.b));
  }

  SOIL_save_image("data\\test_gray.bmp", SOIL_SAVE_TYPE_BMP, width, height, 1, gray_pixels.data());

  SOIL_free_image_data(ptr_pixels);
  ptr_pixels = nullptr;

  return 0;
}
