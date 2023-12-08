// Codes

#include <vu>

```
	bool  test_with_final_color = false;
	float gaussian_blur_threshold = 125.F;
	float gaussian_blur_sigma = GaussianBlurKernelSigma;
	int gaussian_blur_kernel_size = GaussianBlurKernelSize.first;
	{
	  std::wstring file_path = vu::get_contain_directory() + L"cfg.ini";
	  vu::INIFile ini(file_path);
	  if (auto section = ini.section(L"cfg"))
	  {
		gaussian_blur_threshold = section->read(L"gaussian_blur_threshold").to_float();
		gaussian_blur_sigma = section->read(L"gaussian_blur_sigma").to_float();
		gaussian_blur_kernel_size = section->read(L"gaussian_blur_kernel_size").to_int();
		test_with_final_color = section->read(L"test_with_final_color").to_int() != 0;
	  }
	}
```

// The `cfg.ini` file

```
[cfg]
gaussian_blur_threshold = 100
gaussian_blur_sigma = 2
gaussian_blur_kernel_size = 2
test_with_final_color = 1
```
