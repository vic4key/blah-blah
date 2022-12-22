// view ram
{
  auto& ram = SystemInformation::GetpInstance()->GetRAM();

  vu::print_A("ram pagefile", vu::format_bytes_A(ram.m_Memory.m_Pagefile.m_Free), " - ", vu::format_bytes_A(ram.m_Memory.m_Pagefile.m_Total));
  vu::print_A("ram physic", vu::format_bytes_A(ram.m_Memory.m_Physical.m_Free), " - ", vu::format_bytes_A(ram.m_Memory.m_Physical.m_Total));

  vu::print_A("");

  vu::print_A("ram available", vu::format_bytes_A(SystemInformation::GetpInstance()->GetRAMAvailableMemory()));
  vu::print_A("ram available (pagefile)", vu::format_bytes_A(SystemInformation::GetpInstance()->GetRAMAvailableMemory(true)));

  vu::print_A("");

  vu::print_A("ram total", vu::format_bytes_A(SystemInformation::GetpInstance()->GetRAMTotalMemory()));
  vu::print_A("ram total (pagefile)", vu::format_bytes_A(SystemInformation::GetpInstance()->GetRAMTotalMemory(true)));
};