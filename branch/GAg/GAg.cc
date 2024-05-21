
#include "GAg.hh"

GAgPredictor::GAgPredictor() : m_global_history(0), m_pht(kPhtEntries, kWeaklyTakenVal) {}

void GAgPredictor::init()
{
  //! NOTE: Initialize GHR to 0 and m_pht entries to weakly taken
  std::fill(m_pht.begin(), m_pht.end(), kWeaklyTakenVal);
}

bool GAgPredictor::predict(uint64_t ip) const
{
  uint64_t index = global_history();
  return m_pht[index] >= kWeaklyTakenVal;
}

void GAgPredictor::train(uint64_t ip, uint64_t target, bool taken, bool pred_taken)
{
  uint64_t index = global_history();

  if (taken) {
    if (m_pht[index] < kMaxCounterVal)
      m_pht[index]++;
  } else {
    if (m_pht[index] > kMinCounterVal)
      m_pht[index]--;
  }

  update(taken);
}

void GAgPredictor::update(bool taken) { m_global_history = ((m_global_history << 1) | taken) & ((1 << kGhrBits) - 1); }

namespace
{
GAgPredictor pred{};
}

void O3_CPU::initialize_branch_predictor() { pred.init(); }

uint8_t O3_CPU::predict_branch(uint64_t ip) { return pred.predict(); }

void O3_CPU::last_branch_result(uint64_t ip, uint64_t branch_target, uint8_t taken, uint8_t branch_type)
{
  auto predicted_taken = predict_branch(ip);
  pred.train(ip, branch_target, taken, predicted_taken);
}
