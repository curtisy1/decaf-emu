#pragma once
#include <cstdint>
#include <functional>
#include "state.h"
#include "wfunc_ptr.h"

struct ThreadState;

namespace cpu
{

   static const uint32_t CALLBACK_ADDR = 0xFBADCDE0;

   enum class JitMode {
      Enabled,
      Disabled,
      Debug
   };

   void setJitMode(JitMode mode);

   void initialise();
   void executeSub(ThreadState *state);

}

template<typename ReturnType, typename... Args>
ReturnType wfunc_ptr<ReturnType, Args...>::operator()(Args... args)
{
   ThreadState *state = GetCurrentFiberState();

   // Push args
   ppctypes::applyArguments(state, args...);

   // Save state
   auto nia = state->nia;

   // Set state
   state->cia = 0;
   state->nia = address;
   cpu::executeSub(state);

   // Restore state
   state->nia = nia;

   // Return the result
   return ppctypes::getResult<ReturnType>(state);
}

