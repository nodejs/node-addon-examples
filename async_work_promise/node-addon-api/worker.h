#include "napi.h"

class DoHeavyMathWorker : public Napi::AsyncWorker {
 public:
  DoHeavyMathWorker(const Napi::Env& env, uint32_t num_1, uint32_t num_2)
      : Napi::AsyncWorker{env, "DoHeavyMathWorker"},
        m_deferred{env},
        m_num_1{num_1},
        m_num_2{num_2} {}

  /**
   * GetPromise associated with _deferred for return to JS
   */
  Napi::Promise GetPromise() { return m_deferred.Promise(); }

 protected:
  void Execute() {
    // Simulate heavy math work
    if (m_num_2 == 0) {
      SetError("Cannot divide by zero");
      return;
    }
    m_result = m_num_1 / m_num_2;
  }
  void OnOK() { m_deferred.Resolve(Napi::Number::New(Env(), m_result)); }
  void OnError(const Napi::Error& err) { m_deferred.Reject(err.Value()); }

 private:
  Napi::Promise::Deferred m_deferred;
  uint32_t m_num_1;
  uint32_t m_num_2;
  uint32_t m_result;
};