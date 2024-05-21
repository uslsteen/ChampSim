#pragma once

#include <cstdint>
#include <vector>

#include <unordered_map>
//
#include "ooo_cpu.h"

/**
 * @brief Two-level adaptive branch predictor using global history for each branch
 */
class GApPredictor final
{

private:
  // Global history register
  uint64_t m_global_history = 0;

  // Pattern history table
  std::unordered_map<uint64_t, std::vector<uint8_t>> m_pht;

  // Length of the GHR
  static constexpr uint64_t kGhrBits = 14;

  // Number of history table entries
  static constexpr uint64_t kPhtEntries = 1 << kGhrBits;

  static constexpr uint64_t kMinCounterVal = 0;
  static constexpr uint64_t kWeaklyTakenVal = 2;
  static constexpr uint64_t kMaxCounterVal = 3;

public:
  GApPredictor();

  /// Getter
  uint64_t global_history() const;

  //
  void init();

  /**
   * @brief Function to provide prediction
   *
   */
  bool predict(uint64_t ip);

  /**
   * @brief Function to update state of predition
   *
   */
  void train(uint64_t ip, uint64_t target, bool taken, bool pred_taken);

  /**
   * @brief Function to update global history register
   *
   */
  void update(bool taken);
};
