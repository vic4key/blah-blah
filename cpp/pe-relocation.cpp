// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <tchar.h>
#include <iostream>
#include <vu>

typedef struct _IMAGE_BASE_RELOCATION_ENTRY
{
  USHORT Offset : 12;
  USHORT Type : 4;
} IMAGE_BASE_RELOCATION_ENTRY, *PIMAGE_BASE_RELOCATION_ENTRY;

#define COUNT_RELOCATION_ENTRY(ptr)\
  (ptr == nullptr ? 0 : (PIMAGE_BASE_RELOCATION(ptr)->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(IMAGE_BASE_RELOCATION_ENTRY))

static const char* Types[] =
{
  "IMAGE_REL_BASED_ABSOLUTE",
  "IMAGE_REL_BASED_HIGH",
  "IMAGE_REL_BASED_LOW",
  "IMAGE_REL_BASED_HIGHLOW",
  "IMAGE_REL_BASED_HIGHADJ",
  "IMAGE_REL_BASED_MACHINE_SPECIFIC_5",
  "IMAGE_REL_BASED_RESERVED",
  "IMAGE_REL_BASED_MACHINE_SPECIFIC_7",
  "IMAGE_REL_BASED_MACHINE_SPECIFIC_8",
  "IMAGE_REL_BASED_MACHINE_SPECIFIC_9",
  "IMAGE_REL_BASED_DIR64",
};

int main()
{
#ifdef _WIN64
  const std::tstring PROCESS_NAME = _T("x64dbg.exe");
#else // _WIN32
  const std::tstring PROCESS_NAME = _T("x32dbg.exe");
#endif // _WIN64

  auto PIDs = vu::NameToPID(PROCESS_NAME);
  assert(!PIDs.empty());

  vu::CProcess process;
  process.Attach(PIDs.back());

  auto module = process.GetModuleInformation();

  vu::CPEFileT<vu::peX> pe(module.szExePath);
  pe.Parse();

  auto pPEHeader = pe.GetpPEHeader();
  auto IDD = pPEHeader->Relocation;

  for (DWORD Size = 0; Size < IDD.Size; )
  {
    auto ptr = PUCHAR(vu::ulong64(pe.GetpBase()) + pe.RVA2Offset(IDD.VirtualAddress) + Size);
    assert(ptr != nullptr);

    auto pIBR = PIMAGE_BASE_RELOCATION(ptr);
    assert(pIBR != nullptr);

    auto nEntries = COUNT_RELOCATION_ENTRY(ptr);
    ptr += sizeof(IMAGE_BASE_RELOCATION);

    for (DWORD idx = 0; idx < nEntries; idx++)
    {
      const auto pEntry = PIMAGE_BASE_RELOCATION_ENTRY(ptr + idx * sizeof(IMAGE_BASE_RELOCATION_ENTRY));
      assert(pEntry);

      auto RVA = pIBR->VirtualAddress + pEntry->Offset;
      auto OFS = pe.RVA2Offset(RVA);
      auto VA1 = pe.GetpPEHeader()->ImageBase + RVA;
      auto v1  = *(vu::peX*)(vu::peX(pe.GetpBase()) + OFS);
      auto VA2 = vu::peX(module.modBaseAddr) + RVA;
      auto v2  = vu::peX(0);
      vu::RPM(process.Handle(), LPVOID(VA2), &v2, sizeof(v2));

      std::tcout << vu::Fmt(_T("%llX %llX %llX : %llX -> %llX"), OFS, RVA, VA2, v1, v2) << std::endl;
    }

    Size += pIBR->SizeOfBlock;
  }

  return 0;
}