/**
 * @file ProcessAffinity.h
 * Header file for ProcessAffinity class.
 *
 * Copyright Prowess, Inc. 2023
 *
 * The copyright to the computer program(s) herein is the property
 * of Prowess, Inc. The program(s) may be used and/or copied only
 * with the written permission of Prowess, Inc. or in accordance
 * with the terms and conditions stipulated in the agreement/contract
 * under which the program(s) have been supplied.
 */

#pragma once

#include <vector>

/**
 * The class that manage logical processors for a specified process.
 */
class BASEEXPORT ProcessAffinity
{
public:
  /**
   * Set default number of logical processors that using for a specified process (default is all logical processors).
   * @param[in] theProcessID The process ID.
   * @param[in] numLogicalProcessors Default number of logical processors.
   */
  ProcessAffinity(const unsigned int theProcessID, const unsigned int numLogicalProcessors = -1);

  /**
   * Set a list default of logical processors that using for a specified process.
   * @param[in] theProcessID The process ID.
   * @param[in] theLogicalProcessors A list of specified logical processors.
   */
  ProcessAffinity(const unsigned int theProcessID, const std::vector<unsigned int>& theLogicalProcessors);

  virtual ~ProcessAffinity();

  /**
   * Get maximum number of all logical processors in the current machine.
   * @return The maximum number of all logical processors.
   */
  unsigned int GetMaxLogicalProcessors() const;

  /**
   * Reset to use all logical processors in the current machine.
   * @return true if succeed otherwise return false.
   */
  bool Reset();

  /**
   * Set a specified number of logical processors.
   * @param[in] numLogicalProcessors The specified number of logical processors.
   * @return true if succeed otherwise return false.
   */
  bool Set(const unsigned int numLogicalProcessors);

  /**
   * Set a list of specified logical processors.
   * @param[in] theLogicalProcessors A list of specified logical processors.
   * @return true if succeed otherwise return false.
   */
  bool Set(const std::vector<unsigned int>& theLogicalProcessors);

  /**
   * Get in-used logical processors for the current process.
   * @param[out] pLogicalProcessors The list of in-used logical processors.
   * @return The number of logical processors that being in-used.
   */
  unsigned int Get(std::vector<unsigned int>* pLogicalProcessors = nullptr);

  /**
   * Determine that a process is running or not.
   * @param[in] theProcessID The process ID.
   * @return true if the process is running otherwise return false.
   */
  static bool IsProcessRunning(const unsigned int theProcessID);

private:
  bool Initialize(const unsigned int theProcessID);
  bool SetLogicalProcessors(const std::vector<unsigned int>& theLogicalProcessors);

private:
  void* m_Process;
  unsigned int m_MaxLogicalProcessors;
};