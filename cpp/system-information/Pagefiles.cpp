#pragma once

#include "SystemInformation.h"

#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <functional>

#include <windows.h>
#include <winternl.h>

#ifndef KiB
const unsigned long KiB = 1024;
#endif // !KiB

// https://learn.microsoft.com/en-us/windows/win32/api/winnt/nf-winnt-uint32x32to64
// https://devblogs.microsoft.com/oldnewthing/20210510-00/?p=105200
// MSDN said: This value is always a multiple of 4,096, which is the page size that is used in Windows.
const ULONG PageSize = 4 * KiB; // 4KB

/**
 * @refer ntstatus.h (MS WDDK - Microsoft Windows Driver Development Kit)
 */
#define NT_SUCCESS(Status)               (((NTSTATUS)(Status)) >= 0)
#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)
#define STATUS_INSUFFICIENT_RESOURCES    ((NTSTATUS)0xC000009AL)

/**
 * @refer https://www.geoffchappell.com/studies/windows/km/ntoskrnl/api/ex/sysinfo/pagefile_ex.htm
 */
#define SystemPageFileInformationEx 0x90

/**
 * @refer https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntquerysysteminformation#remarks
 */
using FnNtQuerySystemInformation = NTSTATUS (WINAPI*)(
  ULONG  SystemInformationClass,
  PVOID  SystemInformation,
  ULONG  SystemInformationLength,
  PULONG ReturnLength
);

static FnNtQuerySystemInformation fnNtQuerySystemInformation = nullptr;

/**
 * @refer https://www.geoffchappell.com/studies/windows/km/ntoskrnl/api/ex/sysinfo/pagefile.htm
 * @refer http://undocumented.ntinternals.net/index.html?page=UserMode/Undocumented Functions/System Information/Structures/SYSTEM_PAGEFILE_INFORMATION.html
 */
struct SYSTEM_PAGEFILE_INFORMATION
{
  ULONG NextEntryOffset;
  ULONG TotalSize;
  ULONG TotalInUse;
  ULONG PeakUsage;
  UNICODE_STRING PageFileName;
};

/**
 * @refer https://www.geoffchappell.com/studies/windows/km/ntoskrnl/api/ex/sysinfo/pagefile_ex.htm
 */
struct SYSTEM_PAGEFILE_INFORMATION_EX
{
  union
  {
    SYSTEM_PAGEFILE_INFORMATION Info;
    struct
    {
      ULONG NextEntryOffset;
      ULONG TotalSize;
      ULONG TotalInUse;
      ULONG PeakUsage;
      UNICODE_STRING PageFileName;
    };
  };

  ULONG MinimumSize;
  ULONG MaximumSize;
};

/**
 * @refer https://ultradefrag.net/doc/man/Windows NT(2000) Native API Reference.pdf
 * @refer https://microsoft.github.io/WindowsDevicePortalWrapper/class_microsoft_1_1_tools_1_1_windows_device_portal_1_1_device_portal_1_1_system_performance_information.html
 */
struct SYSTEM_PERFORMANCE_INFORMATION_EX
{
  LARGE_INTEGER IdleProcessTime;
  LARGE_INTEGER IoReadTransferCount;
  LARGE_INTEGER IoWriteTransferCount;
  LARGE_INTEGER IoOtherTransferCount;
  ULONG IoReadOperationCount;
  ULONG IoWriteOperationCount;
  ULONG IoOtherOperationCount;
  ULONG AvailablePages;
  ULONG CommittedPages;
  ULONG CommitLimit;
  ULONG PeakCommitment;
  ULONG PageFaultCount;
  ULONG CopyOnWriteCount;
  ULONG TransitionCount;
  ULONG CacheTransitionCount;
  ULONG DemandZeroCount;
  ULONG PageReadCount;
  ULONG PageReadIoCount;
  ULONG CacheReadCount;
  ULONG CacheIoCount;
  ULONG DirtyPagesWriteCount;
  ULONG DirtyWriteIoCount;
  ULONG MappedPagesWriteCount;
  ULONG MappedWriteIoCount;
  ULONG PagedPoolPages;
  ULONG NonPagedPoolPages;
  ULONG PagedPoolAllocs;
  ULONG PagedPoolFrees;
  ULONG NonPagedPoolAllocs;
  ULONG NonPagedPoolFrees;
  ULONG FreeSystemPtes;
  ULONG ResidentSystemCodePage;
  ULONG TotalSystemDriverPages;
  ULONG TotalSystemCodePages;
  ULONG NonPagedPoolLookasideHits;
  ULONG PagedPoolLookasideHits;
  ULONG AvailablePagedPoolPages;
  ULONG ResidentSystemCachePage;
  ULONG ResidentPagedPoolPage;
  ULONG ResidentSystemDriverPage;
  ULONG CcFastReadNoWait;
  ULONG CcFastReadWait;
  ULONG CcFastReadResourceMiss;
  ULONG CcFastReadNotPossible;
  ULONG CcFastMdlReadNoWait;
  ULONG CcFastMdlReadWait;
  ULONG CcFastMdlReadResourceMiss;
  ULONG CcFastMdlReadNotPossible;
  ULONG CcMapDataNoWait;
  ULONG CcMapDataWait;
  ULONG CcMapDataNoWaitMiss;
  ULONG CcMapDataWaitMiss;
  ULONG CcPinMappedDataCount;
  ULONG CcPinReadNoWait;
  ULONG CcPinReadWait;
  ULONG CcPinReadNoWaitMiss;
  ULONG CcPinReadWaitMiss;
  ULONG CcCopyReadNoWait;
  ULONG CcCopyReadWait;
  ULONG CcCopyReadNoWaitMiss;
  ULONG CcCopyReadWaitMiss;
  ULONG CcMdlReadNoWait;
  ULONG CcMdlReadWait;
  ULONG CcMdlReadNoWaitMiss;
  ULONG CcMdlReadWaitMiss;
  ULONG CcReadAheadIos;
  ULONG CcLazyWriteIos;
  ULONG CcLazyWritePages;
  ULONG CcDataFlushes;
  ULONG CcDataPages;
  ULONG ContextSwitches;
  ULONG FirstLevelTbFills;
  ULONG SecondLevelTbFills;
  ULONG SystemCalls;
  ULONGLONG CcTotalDirtyPages; // since THRESHOLD
  ULONGLONG CcDirtyPageThreshold; // since THRESHOLD
  LONGLONG ResidentAvailablePages; // since THRESHOLD
  ULONGLONG SharedCommittedPages; // since THRESHOLD
};

inline NTSTATUS EnumPagefiles(std::function<void(SYSTEM_PAGEFILE_INFORMATION_EX*)> fn)
{
  assert(fnNtQuerySystemInformation != nullptr);

  ULONG size = 1*KiB;
  std::unique_ptr<unsigned char> pPagefiles(new unsigned char[size]);
  ZeroMemory(pPagefiles.get(), size);

  NTSTATUS status = STATUS_SUCCESS;
  while ((status = fnNtQuerySystemInformation(
    SystemPageFileInformationEx, pPagefiles.get(), size, nullptr)) == STATUS_INFO_LENGTH_MISMATCH)
  {
    size *= 2;
    if (size > KiB*KiB) // maximum buffer size for the list of page files
    {
      return STATUS_INSUFFICIENT_RESOURCES;
    }

    pPagefiles.reset(new unsigned char[size]);
    ZeroMemory(pPagefiles.get(), size);
  }

  if (!NT_SUCCESS(status))
  {
    return status;
  }

  auto fnNextPagefile = [](SYSTEM_PAGEFILE_INFORMATION_EX* pPagefile) -> SYSTEM_PAGEFILE_INFORMATION_EX*
  {
    if (pPagefile == nullptr || pPagefile->NextEntryOffset == 0)
    {
      return nullptr;
    }

    return reinterpret_cast<SYSTEM_PAGEFILE_INFORMATION_EX*>(PVOID(ULONG_PTR(pPagefile) + ULONG_PTR(pPagefile)));
  };

  auto pPagefile = reinterpret_cast<SYSTEM_PAGEFILE_INFORMATION_EX*>(pPagefiles.get());
  while (pPagefile != nullptr)
  {
    if (fn != nullptr)
    {
      fn(pPagefile);
    }

    pPagefile = fnNextPagefile(pPagefile);
  }

  return status;
}

inline bool GetPagefiles(std::vector<SystemInformation::PagefileInformation>& pagefiles)
{
  pagefiles.clear();

  if (fnNtQuerySystemInformation == nullptr)
  {
    auto pModule = GetModuleHandle(L"ntdll");
    if (pModule != nullptr)
    {
      fnNtQuerySystemInformation = reinterpret_cast<FnNtQuerySystemInformation>(
        GetProcAddress(pModule, "NtQuerySystemInformation"));
    }
  }

  assert(fnNtQuerySystemInformation != nullptr);

  // Pagefiles - Commit-charge

  SYSTEM_PERFORMANCE_INFORMATION_EX spi = { 0 };

  NTSTATUS status = fnNtQuerySystemInformation(SystemPerformanceInformation, &spi, sizeof(spi), nullptr);

  if (!NT_SUCCESS(status))
  {
    return status;
  }

  // Pagefiles - Page-files

  auto fnUnicodeStringToStdWString = [](const UNICODE_STRING& s) -> std::wstring
  {
    std::wstring result;
    result.resize(s.Length + 1);
    std::copy_n(s.Buffer, s.Length, result.begin());
    return result;
  };

  status = EnumPagefiles([&](SYSTEM_PAGEFILE_INFORMATION_EX* pPagefile) -> void
  {
    const std::wstring Prefix = L"\\??\\";
    auto fileName = fnUnicodeStringToStdWString(pPagefile->Info.PageFileName);
    if (fileName.find(Prefix) != std::wstring::npos)
    {
      fileName = fileName.substr(Prefix.length());
    }

    SystemInformation::PagefileInformation pi;
    pi.m_FileName = fileName;
    pi.m_Usage = UInt32x32To64(pPagefile->Info.TotalInUse, PageSize);
    pi.m_PeakUsage = UInt32x32To64(pPagefile->Info.PeakUsage, PageSize);
    pi.m_TotalSize = UInt32x32To64(pPagefile->Info.TotalSize, PageSize);
    pi.m_MinimumSize = UInt32x32To64(pPagefile->MinimumSize, PageSize);
    pi.m_MaximumSize = UInt32x32To64(pPagefile->MaximumSize, PageSize);
    pi.m_Committed.m_Current = UInt32x32To64(spi.CommittedPages, PageSize);
    pi.m_Committed.m_Limit = UInt32x32To64(spi.CommitLimit, PageSize);

    pagefiles.emplace_back(std::move(pi));
  });

  return NT_SUCCESS(status);
}
