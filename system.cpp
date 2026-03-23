#if __STDC_HOSTED__ == 0
#include "system.hpp"
// -----Program entry point/exit logic for the freestanding implementation--

int main();
void init();

extern "C" {
  void _start() {init(); _system::exit(main()); }
}

// ---------------Freestanding stack protector implementation----------
#if defined(__SSP__) || defined(__SSP_STRONG__) || defined(__SSP_ALL__)

#include <tuple>       // For std::ignore
using namespace std::string_view_literals;

extern "C" {
  void __stack_chk_fail(void) {
      std::ignore = _system::write(_system::kSTDERR_FD, "\nStack smashing detected!"sv);
      _system::crash();
 }
 uintptr_t __stack_chk_guard = 0xD100B22CA55DF88D;
}

void init() {
  if (_system::has_randr_support()) {
    // Change the canary value to a random value to be more secure
    __stack_chk_guard = _system::get_randr_value();
  }
}

#else
void init() {
}
#endif
#endif
