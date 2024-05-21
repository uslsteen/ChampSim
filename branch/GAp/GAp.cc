#include "GAp.hh"

GApPredictor::GApPredictor() : m_global_history(0) {}

void GApPredictor::init() { m_pht.clear(); }

bool GApPredictor::predict(uint64_t ip)
{
  uint64_t index = global_history();

  //! NOTE: init PHT entries to "weakly taken"
  //! I decided make initialization lazy
  if (m_pht.find(ip) == m_pht.end()) {
    m_pht[ip] = std::vector<uint8_t>(kPhtEntries, kWeaklyTakenVal);
  }
  return m_pht[ip][index] >= kWeaklyTakenVal;
}

void GApPredictor::train(uint64_t ip, uint64_t target, bool taken, bool pred_taken)
{
  uint64_t index = global_history();

  //! NOTE: Initialize PHT entries to 'weakly taken'
  //! I decided make initialization lazy
  if (m_pht.find(ip) == m_pht.end())
    m_pht[ip] = std::vector<uint8_t>(kPhtEntries, kWeaklyTakenVal);

  if (taken) {
    if (m_pht[ip][index] < kMaxCounterVal)
      m_pht[ip][index]++;
  } else {
    if (m_pht[ip][index] > kMinCounterVal)
      m_pht[ip][index]--;
  }
  update(taken);
}

uint64_t GApPredictor::global_history() const { return m_global_history; }

void GApPredictor::update(bool taken) { m_global_history = ((m_global_history << 1) | taken) & ((1 << kGhrBits) - 1); }

namespace
{
GApPredictor pred{};
}

void O3_CPU::initialize_branch_predictor() { pred.init(); }

uint8_t O3_CPU::predict_branch(uint64_t ip) { return pred.predict(ip); }

void O3_CPU::last_branch_result(uint64_t ip, uint64_t branch_target, uint8_t taken, uint8_t branch_type)
{
  auto predicted_taken = predict_branch(ip);
  pred.train(ip, branch_target, taken, predicted_taken);
}
