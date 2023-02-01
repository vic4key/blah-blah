#include <vu>
#include <vector>

// Unikey NT

// RC5 64-bit
// $+0  | 48:83EC 28       | sub rsp,28
// $+4  | 48:8D0D 9D801000 | lea rcx,qword ptr ds:[140125F28]
// $+B  | FF15 DF760A00    | call qword ptr ds:[<&RtlEnterCriticalSection>]
// $+11 | 48:8B05 E0881000 | mov rax,qword ptr ds:[140126778]
// $+18 | 33D2             | xor edx,edx
// $+1A | 3910             | cmp dword ptr ds:[rax],edx
// $+1C | 0F94C2           | sete dl
// $+1F | 8910             | mov dword ptr ds:[rax],edx
// $+21 | 48:8B05 D0881000 | mov rax,qword ptr ds:[140126778]
// $+28 | 8378 04 00       | cmp dword ptr ds:[rax+4],0
// $+2C | 74 05            | je unikeynt.14001DEB3
// $+2E | E8 CDEBFFFF      | call <unikeynt.sub_14001CA80>
// $+33 | E8 18F7FFFF      | call <unikeynt.sub_14001D5D0>
// $+38 | 48:8D0D 69801000 | lea rcx,qword ptr ds:[140125F28]
// $+3F | 48:83C4 28       | add rsp,28
// $+43 | 48:FF25 9E760A00 | jmp qword ptr ds:[<&RtlLeaveCriticalSection>]
// $+4A | CC               | int3

// RC5 32-bit
// $+0  | 68 08F64D00      | push unikeynt.4DF608
// $+5  | FF15 50C24900    | call dword ptr ds:[<&RtlEnterCriticalSection>]
// $+B  | A1 38FE4D00      | mov eax,dword ptr ds:[4DFE38]
// $+10 | 33C9             | xor ecx,ecx
// $+12 | 3908             | cmp dword ptr ds:[eax],ecx
// $+14 | 0F94C1           | sete cl
// $+17 | 8908             | mov dword ptr ds:[eax],ecx
// $+19 | A1 38FE4D00      | mov eax,dword ptr ds:[4DFE38]
// $+1E | 8378 04 00       | cmp dword ptr ds:[eax+4],0
// $+22 | 74 05            | je unikeynt.419839
// $+24 | E8 67EEFFFF      | call <unikeynt.sub_4186A0>
// $+29 | E8 92F8FFFF      | call <unikeynt.sub_4190D0>
// $+2E | 68 08F64D00      | push unikeynt.4DF608
// $+33 | FF15 4CC24900    | call dword ptr ds:[<&RtlLeaveCriticalSection>]
// $+39 | C3               | ret
// $+3A | CC               | int3

struct pattern_t
{
  vu::arch arch;
  std::string pattern;
  vu::ulong position;
  vu::ulong offset;
};

static struct
{
  vu::ProcessW process;
  bool         initialized = false;
  size_t       address_mode_state = 0;
  size_t       address_change_mode_function = 0;
  pattern_t    pattern;
} g_unikey_nt;

vu::VUResult __cdecl unikey_nt_initialize(vu::ulong process_id)
{
  if (g_unikey_nt.initialized)
  {
    return vu::VU_OK;
  }

  if (!g_unikey_nt.process.attach(process_id))
  {
    return __LINE__;
  }

  auto modules = g_unikey_nt.process.get_modules();
  if (modules.empty())
  {
    return __LINE__;
  }

  auto& module_exe = modules.at(0); // the first module is always EXE
  vu::Buffer data(module_exe.modBaseSize);
  if (!g_unikey_nt.process.read_memory(vu::ulongptr(module_exe.modBaseAddr), data))
  {
    return __LINE__;
  }

  std::vector<pattern_t> patterns;
  {
    patterns.push_back({ vu::arch::x86, "68 ?? ?? ?? ?? FF 15 ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C9 39 08 0F 94 C1 89 08", 0xC, 0x0 });
    patterns.push_back({ vu::arch::x64, "48 83 EC ?? 48 8D 0D ?? ?? ?? ?? FF 15 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 33 D2 39 10 0F 94 C2 89 10", 0x14, 0x0 });
    patterns.push_back({ vu::arch::x64, "48 83 EC ?? 48 8B 0D ?? ?? ?? ?? 33 C0 39 41 ?? 0F 94 C0 89 41 ??", 0x7, 0x4 });
  }

  std::vector<size_t> result;

  auto it = std::find_if(patterns.cbegin(), patterns.cend(), [&](const pattern_t& pattern) -> bool
  {
    if (pattern.arch != g_unikey_nt.process.bit())
    {
      return false;
    }

    result = vu::find_pattern_A(data, pattern.pattern, true);

    return !result.empty();
  });

  if (it == patterns.cend() || result.empty())
  {
    return __LINE__;
  }

  auto found_offset = result.at(0);

  g_unikey_nt.pattern = *it;

  g_unikey_nt.address_change_mode_function = ULONG_PTR(module_exe.modBaseAddr) + found_offset;

  if (g_unikey_nt.pattern.arch == vu::arch::x64) // 64-bit
  {
    g_unikey_nt.address_mode_state = *(DWORD*)(data.get_ptr_bytes() + found_offset + g_unikey_nt.pattern.position);
    g_unikey_nt.address_mode_state += g_unikey_nt.pattern.position  + 4; // 64-bit relative address
    g_unikey_nt.address_mode_state += g_unikey_nt.address_change_mode_function;
  }
  else // 32-bit
  {
    g_unikey_nt.address_mode_state = *(DWORD*)(data.get_ptr_bytes() + found_offset + g_unikey_nt.pattern.position);
  }

  g_unikey_nt.initialized = true;

  return vu::VU_OK;
}

int unikey_nt_get_current_mode()
{
  if (!g_unikey_nt.initialized)
  {
    vu::print_A("Unikey NT:error that did not initialize");
    return -1;
  }

  int mode = -1;

  auto succeed = vu::read_memory_ex(
    g_unikey_nt.process.bit(),
    g_unikey_nt.process.handle(),
    LPCVOID(g_unikey_nt.address_mode_state),
    &mode, sizeof(mode), true, 1, g_unikey_nt.pattern.offset);

  if (!succeed)
  {
    vu::print_A("Unikey NT:read mode failed");
    return -1;
  }

  return mode;
}

// MODE_EN = 0
// MODE_VN = 1

void __cdecl unikey_nt_set_current_mode(int mode)
{
  if (!g_unikey_nt.initialized)
  {
    vu::print_A("Unikey NT:error that did not initialize");
    return;
  }

  if (unikey_nt_get_current_mode() == mode)
  {
    return;
  }

  g_unikey_nt.process.execute_code_at(g_unikey_nt.address_change_mode_function);
}
