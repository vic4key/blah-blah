/**
 * @file ProcessAffinity.cpp
 * Implementation file for ProcessAffinity class.
 *
 * Copyright Prowess, Inc. 2023
 *
 * The copyright to the computer program(s) herein is the property
 * of Prowess, Inc. The program(s) may be used and/or copied only
 * with the written permission of Prowess, Inc. or in accordance
 * with the terms and conditions stipulated in the agreement/contract
 * under which the program(s) have been supplied.
 */

#include "ProcessAffinity.h"

#include <thread>
#include <numeric>
#include <windows.h>

#pragma warning(push)
#pragma warning(disable: 4334) // result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)

ProcessAffinity::ProcessAffinity(const unsigned int theProcessID,
  const unsigned int numLogicalProcessors)
{
  if (Initialize(theProcessID) && !Set(numLogicalProcessors == -1 ? m_MaxLogicalProcessors : numLogicalProcessors))
  {
    Reset();
  }
}

ProcessAffinity::ProcessAffinity(const unsigned int theProcessID,
  const std::vector<unsigned int>& theLogicalProcessors)
{
  if (Initialize(theProcessID) && !Set(theLogicalProcessors))
  {
    Reset();
  }
}

ProcessAffinity::~ProcessAffinity()
{
  Reset();
}

bool ProcessAffinity::Initialize(const unsigned int theProcessID)
{
  m_Process = OpenProcess(PROCESS_SET_INFORMATION, FALSE, theProcessID);
  if (m_Process == nullptr)
  {
    return false;
  }

  m_MaxLogicalProcessors = std::thread::hardware_concurrency();

  return true;
}

unsigned int ProcessAffinity::GetMaxLogicalProcessors() const
{
  return m_MaxLogicalProcessors;
}

bool ProcessAffinity::Reset()
{
  std::vector<unsigned int> theLogicalProcessors(m_MaxLogicalProcessors);
  std::iota(theLogicalProcessors.begin(), theLogicalProcessors.end(), 0);

  return SetLogicalProcessors(theLogicalProcessors);
}

bool ProcessAffinity::Set(const unsigned int numLogicalProcessors)
{
  if (numLogicalProcessors > m_MaxLogicalProcessors)
  {
    return false;
  }

  std::vector<unsigned int> theLogicalProcessors(numLogicalProcessors);
  std::iota(theLogicalProcessors.begin(), theLogicalProcessors.end(), 0);

  return SetLogicalProcessors(theLogicalProcessors);
}

bool ProcessAffinity::Set(const std::vector<unsigned int>& theLogicalProcessors)
{
  return SetLogicalProcessors(theLogicalProcessors);
}

bool ProcessAffinity::SetLogicalProcessors(const std::vector<unsigned int>& theLogicalProcessors)
{
  if (theLogicalProcessors.size() > m_MaxLogicalProcessors)
  {
    return false;
  }

  DWORD_PTR mask = 0;

  for (auto i : theLogicalProcessors)
  {
    if (i >= m_MaxLogicalProcessors)
    {
      return false;
    }

    mask |= 1 << i;
  }

  if (mask == 0)
  {
    return false;
  }

  return SetProcessAffinityMask(m_Process, mask) != FALSE;
}

unsigned int ProcessAffinity::Get(std::vector<unsigned int>* pLogicalProcessors)
{
  if (pLogicalProcessors != nullptr)
  {
    pLogicalProcessors->clear();
  }

  DWORD_PTR theProcessAffinityMask = 0;
  DWORD_PTR theSystemAffinityMask = 0;
  if (GetProcessAffinityMask(m_Process, &theProcessAffinityMask, &theSystemAffinityMask) == FALSE)
  {
    return -1;
  }

  unsigned int numLogicalProcessors = 0;

  for (unsigned int i = 0; i < m_MaxLogicalProcessors; i++)
  {
    if (theProcessAffinityMask & (1 << i))
    {
      numLogicalProcessors += 1;

      if (pLogicalProcessors != nullptr)
      {
        pLogicalProcessors->push_back(i);
      }
    }
  }

  return numLogicalProcessors;
}

bool ProcessAffinity::IsProcessRunning(const unsigned int theProcessID)
{
  bool result = false;

  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, theProcessID);
  if (hProcess != nullptr)
  {
    result = true;
    CloseHandle(hProcess);
  }

  return result;
}

#pragma warning(pop)