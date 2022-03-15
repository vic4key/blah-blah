// gpu-nvml.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// https://developer.nvidia.com/nvidia-management-library-nvml
// https://docs.nvidia.com/deploy/nvml-api/index.html
// https://docs.nvidia.com/deploy/nvml-api/structnvmlMemory__t.html#structnvmlMemory__t
// https://github.com/NVIDIA/nvidia-settings/blob/main/src/nvml.h

#include <vu>

#include <nvml.h> // #pragma comment(lib, "nvml.lib") // required nvml_v1 (nvml.dll)

#define NVML_STRUCT_VERSION(data, ver) (unsigned int)(sizeof(nvml ## data ## _v ## ver ## _t) | (ver << 24U))

#pragma pack(push, 8)
typedef struct nvmlMemory_v2_st
{
  unsigned int version;            //!< Structure format version (must be 2)
  unsigned long long total;        //!< Total physical device memory (in bytes)
  unsigned long long reserved;     //!< Device memory (in bytes) reserved for system use (driver or firmware)
  unsigned long long free;         //!< Unallocated device memory (in bytes)
  unsigned long long used;         //!< Allocated device memory (in bytes). Note that the driver/GPU always sets aside a small amount of memory for bookkeeping
} nvmlMemory_v2_t;
#pragma pack(pop)

#define nvmlMemory_v2 NVML_STRUCT_VERSION(Memory, 2);

struct device_memory_information_t
{
  unsigned long long total;
  unsigned long long free;
  unsigned long long used;
};

class nvml_v2 : public vu::SingletonT<nvml_v2>
{
public:
  void init();
  void shutdown();
  bool ready();

  unsigned int get_device_cout();
  bool use_device(int device);
  std::string get_device_name();
  bool get_device_memory_information(device_memory_information_t& dmi);

private:
  bool m_ready = false;
  HMODULE m_nvml = nullptr;
  nvmlDevice_t m_device = nullptr;
};

bool nvml_v2::ready()
{
  return m_ready;
}

void nvml_v2::init()
{
  const auto nvml_dll = L"nvml.dll";

  m_nvml = GetModuleHandle(nvml_dll);
  if (m_nvml == nullptr)
  {
    m_nvml = LoadLibrary(nvml_dll);
  }

  if (m_nvml == nullptr)
  {
    return;
  }

  typedef nvmlReturn_t DECLDIR(*Pfn_nvmlInit_v2)(void);
  auto nvmlInit_v2 = Pfn_nvmlInit_v2(GetProcAddress(m_nvml, "nvmlInit_v2"));
  if (nvmlInit_v2 != nullptr)
  {
    auto ret = nvmlInit_v2();
    m_ready = ret == NVML_SUCCESS;
  }
}

void nvml_v2::shutdown()
{
  if (!this->ready())
  {
    return;
  }

  typedef nvmlReturn_t DECLDIR(*Pfn_nvmlShutdown_v2)(void);
  auto nvmlShutdown_v2 = Pfn_nvmlShutdown_v2(GetProcAddress(m_nvml, "nvmlShutdown"));
  if (nvmlShutdown_v2 != nullptr)
  {
    nvmlShutdown_v2();
  }
}

unsigned int nvml_v2::get_device_cout()
{
  if (!this->ready())
  {
    return 0;
  }

  typedef nvmlReturn_t DECLDIR (*Pfn_nvmlDeviceGetCount_v2)(unsigned int* deviceCount);
  auto nvmlDeviceGetCount_v2 = Pfn_nvmlDeviceGetCount_v2(GetProcAddress(m_nvml, "nvmlDeviceGetCount_v2"));
  if (nvmlDeviceGetCount_v2 != nullptr)
  {
    unsigned int device_count = 0;
    auto ret = nvmlDeviceGetCount_v2(&device_count);
    if (ret == NVML_SUCCESS)
    {
      return device_count;
    }
  }

  return 0;
}

bool nvml_v2::use_device(int device)
{
  if (!this->ready())
  {
    return false;
  }

  typedef nvmlReturn_t DECLDIR(*Pfn_nvmlDeviceGetHandleByIndex_v2)(unsigned int index, nvmlDevice_t* device);
  auto nvmlDeviceGetHandleByIndex_v2 = Pfn_nvmlDeviceGetHandleByIndex_v2(GetProcAddress(m_nvml, "nvmlDeviceGetHandleByIndex_v2"));
  if (nvmlDeviceGetHandleByIndex_v2 != nullptr)
  {
    auto ret = nvmlDeviceGetHandleByIndex_v2(device, &m_device);
    return ret == NVML_SUCCESS;
  }

  return false;
}

std::string nvml_v2::get_device_name()
{
  if (!this->ready())
  {
    return "";
  }

  typedef nvmlReturn_t DECLDIR(*Pfn_nvmlDeviceGetName_v2)(nvmlDevice_t device, char* name, unsigned int length);
  auto nvmlDeviceGetName_v2 = Pfn_nvmlDeviceGetName_v2(GetProcAddress(m_nvml, "nvmlDeviceGetName"));
  if (nvmlDeviceGetName_v2 != nullptr)
  {
    char name[NVML_DEVICE_NAME_V2_BUFFER_SIZE] = { 0 };
    auto ret = nvmlDeviceGetName_v2(m_device, name, ARRAYSIZE(name));
    if (ret == NVML_SUCCESS)
    {
      return name;
    }
  }

  return "";
}

bool nvml_v2::get_device_memory_information(device_memory_information_t& dmi)
{
  if (!this->ready())
  {
    return false;
  }

  nvmlMemory_v2_t m = { 0 };
  m.version = nvmlMemory_v2;
  typedef nvmlReturn_t DECLDIR(*Pfn_nvmlDeviceGetMemoryInfo_v2)(nvmlDevice_t device, nvmlMemory_v2_t* memory);
  auto nvmlDeviceGetMemoryInfo_v2 = Pfn_nvmlDeviceGetMemoryInfo_v2(GetProcAddress(m_nvml, "nvmlDeviceGetMemoryInfo_v2"));
  if (nvmlDeviceGetMemoryInfo_v2 != nullptr)
  {
    auto ret = nvmlDeviceGetMemoryInfo_v2(m_device, &m);
    if (ret == NVML_SUCCESS)
    {
      dmi.total = m.total;
      dmi.free  = m.free;
      dmi.used  = m.used;
      return true;
    }
  }

  return false;
}

int main()
{
  nvml_v2::instance().init();

  auto count = nvml_v2::instance().get_device_cout();
  vu::print("device count:", count);

  nvml_v2::instance().use_device(0);

  auto name = nvml_v2::instance().get_device_name();
  vu::print("device name:", name);

  device_memory_information_t dmi = { 0 };
  nvml_v2::instance().get_device_memory_information(dmi);

  auto v1 = vu::format_bytes_A(dmi.total);
  auto v2 = vu::format_bytes_A(dmi.used);
  auto v3 = vu::format_bytes_A(dmi.free);
  auto v4 = vu::format_A("%.1f%%", float(dmi.used) / float(dmi.total) * 100);
  vu::print("device memory:", v3, "/", v2, "/", v1, "/", v4);

  nvml_v2::instance().shutdown();

  return 0;
}
 