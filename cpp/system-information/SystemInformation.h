#pragma once

#include <vu>

/**
 * The System Information class.
 */

class SystemInformation : public vu::SingletonT<SystemInformation>
{
public:
  struct CPU
  {
    size_t m_NumberOfProcessors;
  };

  struct GPU
  {
    struct
    {
      struct
      {
        size_t m_Total;
        size_t m_Free;
      } m_Dedicated;

      struct
      {
        size_t m_Total;
      } m_Shared;
    } m_Memory;
  };

  struct PagefileInformation
  {
    std::wstring m_FileName;
    unsigned long long m_Usage;
    unsigned long long m_PeakUsage;
    unsigned long long m_TotalSize;
    unsigned long long m_MinimumSize;
    unsigned long long m_MaximumSize;

    struct
    {
      size_t m_Current;
      size_t m_Limit;
    } m_Committed;

    PagefileInformation()
    {
      Reset();
    }

    void Reset()
    {
      m_FileName = L"";
      m_Usage = 0;
      m_PeakUsage = 0;
      m_TotalSize = 0;
      m_MinimumSize = 0;
      m_MaximumSize = 0;
      m_Committed.m_Current = 0;
      m_Committed.m_Limit = 0;
    }
  };

  struct RAM
  {
    struct
    {
      struct
      {
        size_t m_Total;
        size_t m_Free;
      } m_Physical;

      struct
      {
        size_t m_Total;
        size_t m_Free;
        PagefileInformation m_Information;
      } m_Pagefile;

      struct
      {
        size_t m_Total;
        size_t m_Free;
      } m_Virtual;
    } m_Memory;
  };

  /**
   * Initialization.
   */
  void Initialize();

  /**
   * Clean-up.
   */
  void Cleanup();

  /**
   * Get the CPU information of the current system.
   * @return The CPU information of the current system.
   */
  const CPU& GetCPU();

  /**
   * Get the GPU information of the current system.
   * @param[in] index The index of NVIDIA GPU (Default is Device 0).
   * @return The GPU information of the current system.
   */
  const GPU& GetGPU(const int index = 0);

  /**
   * Get the GPU available memory (in bytes).
   * @param[in] index The index of NVIDIA GPU (Default is Device 0).
   * @return The GPU available memory (in bytes).
   */
  const size_t GetGPUAvailableMemory(const int index = 0);

  /**
   * Get the dedicated GPU available memory (in bytes).
   * @param[in] index The index of NVIDIA GPU (Default is Device 0).
   * @return The dedicated GPU available memory (in bytes).
   */
  const size_t GetDedicatedGPUAvailableMemory(const int index = 0);

  /**
   * Get the shared GPU available memory (in bytes).
   * @param[in] index The index of NVIDIA GPU (Default is Device 0).
   * @return The shared GPU available memory (in bytes).
   */
  const size_t GetSharedGPUAvailableMemory(const int index = 0);

  /**
   * Get the lowest dedicated GPUs available memory (in bytes).
   * @return The lowest dedicated GPUs available memory (in bytes).
   */
  const size_t GetLowestDedicatedGPUsAvailableMemory();

  /**
   * Get total memory of GPU (in bytes).
   * @param[in] index The index of NVIDIA GPU (Default is Device 0).
   * @return The total memory of GPU (in bytes).
   */
  const size_t GetGPUTotalMemory(const int index = 0);

  /**
   * Get the memory information of the current system that included both physical and virtual memory.
   * @return The the memory information of the current system.
   */
  const RAM& GetRAM();

  /**
   * Get available memory of RAM (in bytes).
   * @param[in] includePagefile Include memory of Pagefile or not.
   * @return The available memory of RAM (in bytes).
   */
  const size_t GetRAMAvailableMemory(const bool includePagefile = false);

  /**
   * Get total memory of RAM (in bytes).
   * @param[in] includePagefile Include memory of Pagefile or not.
   * @return The total memory of RAM (in bytes).
   */
  const size_t GetRAMTotalMemory(const bool includePagefile = false);

  /**
   * Get number of GPU.
   * @return The number of GPU devices.
   */
  int GetNumGPU();

private:
  CPU m_CPU;
  GPU m_GPU;
  RAM m_RAM;
  bool m_NVMLReady = false;
};
