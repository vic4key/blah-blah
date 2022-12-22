#include "SystemInformation.h"
#include <windows.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <nvml.h>
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "nvml.lib")

#include "Pagefiles.cpp"

void SystemInformation::Initialize()
{
  auto ret = nvmlInit();
  m_NVMLReady = ret == NVML_SUCCESS;
}

void SystemInformation::Cleanup()
{
  if (m_NVMLReady)
  {
    nvmlShutdown();
  }
}

const SystemInformation::CPU& SystemInformation::GetCPU()
{
  ZeroMemory(&m_CPU, sizeof(m_CPU));

  SYSTEM_INFO si = { 0 };
  GetSystemInfo(&si);
  m_CPU.m_NumberOfProcessors = si.dwNumberOfProcessors;

  return m_CPU;
}

const SystemInformation::GPU& SystemInformation::GetGPU(const int index)
{
  ZeroMemory(&m_GPU, sizeof(m_GPU));

  if (this->GetNumGPU() == 0)
  {
    return m_GPU;
  }
 
  auto fnGetSharedMemory = [&]() -> void
  {
    m_GPU.m_Memory.m_Shared.m_Total = 0; // Reset Shared GPU Memory information.
    IDXGIFactory* pDXGIFactory;
    HRESULT result;
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
    if (SUCCEEDED(result))
    {
      IDXGIAdapter* pDXGIAdapter;
      result = pDXGIFactory->EnumAdapters(0, &pDXGIAdapter);
      if (SUCCEEDED(result))
      {
        std::unique_ptr<DXGI_ADAPTER_DESC> pAadapterDescription(new DXGI_ADAPTER_DESC);
        pDXGIAdapter->GetDesc(pAadapterDescription.get());
        m_GPU.m_Memory.m_Shared.m_Total = pAadapterDescription->SharedSystemMemory;
      }
    }
  };

  if (m_NVMLReady)
  {
    fnGetSharedMemory();

    nvmlDevice_t device = nullptr;
    auto ret = nvmlDeviceGetHandleByIndex(index, &device);
    if (ret == NVML_SUCCESS)
    {
      nvmlMemory_t mem = { 0 };
      ret = nvmlDeviceGetMemoryInfo(device, &mem);
      if (ret == NVML_SUCCESS)
      {
        m_GPU.m_Memory.m_Dedicated.m_Total = mem.total;
        m_GPU.m_Memory.m_Dedicated.m_Free = mem.free;
        return m_GPU;
      }
    }
  }

  if (m_GPU.m_Memory.m_Dedicated.m_Total == 0)
  {
    fnGetSharedMemory();

    int indexPrev = -1;
    cudaGetDevice(&indexPrev);
    cudaSetDevice(index);
    cudaMemGetInfo(&m_GPU.m_Memory.m_Dedicated.m_Free, &m_GPU.m_Memory.m_Dedicated.m_Total);
    cudaSetDevice(indexPrev);
  }

  return m_GPU;
}

const size_t SystemInformation::GetGPUAvailableMemory(const int index)
{
  this->GetGPU(index);
  return m_GPU.m_Memory.m_Dedicated.m_Free + (m_GPU.m_Memory.m_Shared.m_Total / GetNumGPU());
}

const size_t SystemInformation::GetSharedGPUAvailableMemory(const int index)
{
  this->GetGPU(index);
  return m_GPU.m_Memory.m_Shared.m_Total / GetNumGPU();
}

const size_t SystemInformation::GetDedicatedGPUAvailableMemory(const int index)
{
  this->GetGPU(index);
  return m_GPU.m_Memory.m_Dedicated.m_Free;
}

const size_t SystemInformation::GetLowestDedicatedGPUsAvailableMemory()
{
  size_t lowestDedicatedGPUsAvailableMemory = std::numeric_limits<size_t>::max();
  for (int i = 0; i < this->GetNumGPU(); i++)
  {
    lowestDedicatedGPUsAvailableMemory = std::min(lowestDedicatedGPUsAvailableMemory,
      GetDedicatedGPUAvailableMemory(i));
  }
  return lowestDedicatedGPUsAvailableMemory;
}

const size_t SystemInformation::GetGPUTotalMemory(const int index)
{
  this->GetGPU(index);
  return m_GPU.m_Memory.m_Dedicated.m_Total;
}

const SystemInformation::RAM& SystemInformation::GetRAM()
{
  ZeroMemory(&m_RAM, sizeof(m_RAM));

  MEMORYSTATUSEX ms = { 0 };
  ms.dwLength = sizeof(ms);
  GlobalMemoryStatusEx(&ms);

  m_RAM.m_Memory.m_Physical.m_Total = ms.ullTotalPhys;
  m_RAM.m_Memory.m_Physical.m_Free  = ms.ullAvailPhys;
  m_RAM.m_Memory.m_Pagefile.m_Total = ms.ullTotalPageFile;
  m_RAM.m_Memory.m_Pagefile.m_Free  = ms.ullAvailPageFile;
  m_RAM.m_Memory.m_Virtual.m_Total  = ms.ullTotalVirtual;
  m_RAM.m_Memory.m_Virtual.m_Free   = ms.ullAvailVirtual;

  m_RAM.m_Memory.m_Pagefile.m_Information.Reset();
  std::vector<SystemInformation::PagefileInformation> pagefiles;
  if (GetPagefiles(pagefiles) && !pagefiles.empty())
  {
    m_RAM.m_Memory.m_Pagefile.m_Information = pagefiles.front();
  }

  return m_RAM;
}

const size_t SystemInformation::GetRAMAvailableMemory(const bool includePagefile)
{
  this->GetRAM();

  auto result = m_RAM.m_Memory.m_Physical.m_Free;

  if (includePagefile)
  {
    result = m_RAM.m_Memory.m_Pagefile.m_Information.m_MaximumSize + m_RAM.m_Memory.m_Physical.m_Total -
      m_RAM.m_Memory.m_Physical.m_Free;
  }

  return result;
}

const size_t SystemInformation::GetRAMTotalMemory(const bool includePagefile)
{
  this->GetRAM();

  auto result = m_RAM.m_Memory.m_Physical.m_Total;

  if (includePagefile)
  {
    result += m_RAM.m_Memory.m_Pagefile.m_Information.m_MaximumSize;
  }

  return result;
}

int SystemInformation::GetNumGPU()
{
  int count = 0;

  auto ret = cudaGetDeviceCount(&count);
  if (ret != cudaSuccess)
  {
    return 0;
  }

  return count;
}
