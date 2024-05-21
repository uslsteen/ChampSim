#pragma once

#include <cstdint>
#include <map>
#include <vector>
//
#include "ooo_cpu.h"

/**
 * @brief Two-level adaptive branch predictor using global history for all branches
 */
class GAgPredictor final
{
  // Global branch history register
  uint64_t m_global_history;

  // Pattern history table
  std::vector<uint8_t> m_pht{};

  // Length of the GHR
  static constexpr uint64_t kGhrBits = 14;

  // Number of history table entries
  static constexpr uint64_t kPhtEntries = 1 << kGhrBits;

  static constexpr uint64_t kMinCounterVal = 0;
  static constexpr uint64_t kWeaklyTakenVal = 2;
  static constexpr uint64_t kMaxCounterVal = 3;

public:
  GAgPredictor();

  uint64_t global_history() const { return m_global_history; }

  //
  void init();

  /**
   * @brief Function to provide prediction
              1. Take 2 bit value at index, which eual global branch history register, from pattern history table
              2. The Most Significant Bit of the value indicates about the state of prediction
   */
  bool predict(uint64_t ip = 0) const;

  /**
   * @brief Function to update state of predition
   *          1. Update the 2 bit counter based on the outcome branch (was taken/not taken):
   *          2. Update global branch history by inserting last outcome branch
   */
  void train(uint64_t ip, uint64_t target, bool taken, bool pred_taken);

  /**
   * @brief Function to update global history register
   *
   */
  void update(bool taken);
};
